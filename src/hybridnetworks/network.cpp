//
// Created by  ngs on 01/08/2018.
//

#include "network.h"
#include "src/common/common.h"

//double **Network::ptr_inside_shared_array_ = new double*[ComParam::Num_Of_Threads];
//double **Network::ptr_outside_shared_array_ = new double*[ComParam::Num_Of_Threads];

Network::Network() {
    std::cout << "no param"<<std::endl;
}

Network::Network(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param_l) {
    this->network_id_ = networkId;
    this->thread_id_ = ptr_param_l->GetThreadId();
    this->ptr_inst_ = ptr_inst;
    this->weight_ = this->ptr_inst_->GetWeight();
    this->ptr_param_l_ = ptr_param_l;
    this->ptr_param_g_ = ptr_param_l_->GetFeatureManager()->GetGlobalParam();
    tmp_count_ = 0;
    neural_feature_inserted_ = false;
}

Network::~Network() {
    delete []ptr_max_;
    //the content of ptr_max_children_no_ are pointers, which are allocated in the class TableLookupNetwork
    delete []ptr_max_children_no_;
}

void Network::Touch() {
#ifdef GLOBAL
    std::lock_guard<std::mutex> mtx_locker(mtx);
#endif
    for (int k = 0; k < this->CountNodes(); ++k) {
        this->Touch(k);
    }
}

void Network::Touch(int node_index) {
    if(this->IsRemovded(node_index)){
        return;
    }
    //ptr_childrens_vec points to all hypedges which are rooted by node_index;
    int **ptr_childrens_vec = this->GetChildren(node_index);
    if(nullptr == ptr_childrens_vec){
        tmp_count_++;
        return;
    }
    //childrens_num is the num of hyperedges which are rooted by node_index;
    int childrens_num  = this->GetChildrens_Size(node_index);
    for(int children_index = 0; children_index < childrens_num; ++children_index){
        //int * ptr_children_num = this->GetChildren_Size(children_index);
        int *ptr_children_k = ptr_childrens_vec[children_index];
        //ptr_children_k is the pointer of a hyperedge.
        this->ptr_param_l_->Extract(this,node_index,ptr_children_k,children_index);
        tmp_count_++;
    }
}

void Network::Train() {
    if(this->weight_ == 0){
        return;
    }
    this->Inside();
    this->Outside();
    this->UpdateGradient();
    this->ptr_param_l_->AddObj(this->GetInside(this->CountNodes()-1) * weight_);
}

/**
 *
 * SharedArray is a temporary array to temporarily store the inside and outside value of a network.
 * It is dynamically allocated and can be extended according to maximum number of the nodes in a thread.
 *
 * @return
 */
double* Network::GetInsideSharedArray(){
    /*
    double *pptr_inside_array_ = ptr_param_g_->GetInsideSharedArray(this->thread_id_);
    //int *ptr_array_size = ptr_param_g_->GetInsideSharedArraySize();
    int num_node = this->CountNodes();
    if(nullptr == pptr_inside_array_[this->thread_id_]){
        //ptr_inside_shared_array_[this->thread_id_] = new double[this->CountNodes()];
        pptr_inside_array_[this->thread_id_] = new double[num_node];
        for(int i=0; i<num_node; ++i){
            pptr_inside_array_[this->thread_id_][i] = 0.0;
        }
        ptr_array_size[this->thread_id_] = num_node;
    }
    return pptr_inside_array_[this->thread_id_];*/
}

double* Network::GetOutsideSharedArray() {
    double **pptr_outside_array = ptr_param_g_->GetOutsideSharedArray();
    int *ptr_array_size = ptr_param_g_->GetOutsideSharedArraySize();
    int num_node = this->CountNodes();
    if(!pptr_outside_array[this->thread_id_] || num_node > ptr_array_size[this->thread_id_] ){
        pptr_outside_array[this->thread_id_] = new double[num_node];
        //init the value of shared array
        for(int i = 0; i < num_node; ++i){
            pptr_outside_array[this->thread_id_][i] = 0.0;
        }
        ptr_array_size[this->thread_id_] = num_node;
    }
    return pptr_outside_array[this->thread_id_];
}

int Network::GetNetworkID() {
    return network_id_;
}

Instance* Network::GetInstance() {
    return ptr_inst_;
}

std::vector<int> Network::GetNodeArray(int nodeIndex) {
    long nodeId = this->GetNode(nodeIndex);
    return NetworkIDManager::ToHybridNodeArray(nodeId);
}

double Network::GetInside(int nodeId) {
    if(nullptr == ptr_inside_){
        std::cerr<< "ERROR: the empty pointer of ptr_inside"<<std::endl;
    }
    return ptr_inside_[nodeId];
}

void Network::Max() {
    int num_count = this->CountNodes();
    ptr_max_ = new double[num_count];
    ptr_max_children_no_ = new int*[num_count];
    //init the value and the pointer.
    for(int nodeid = 0; nodeid < num_count; ++nodeid){
        ptr_max_[nodeid] = 0;
        ptr_max_children_no_[nodeid] = nullptr;
    }
    //
    for(int nodeid = 0; nodeid < num_count; ++nodeid){
        this->Max(nodeid);
    }
}

void Network::Max(int nodeId) {
    if(this->IsRemovded(nodeId)){
        this->ptr_max_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
        return;
    }
    //for the leaf node.
    if(0 == nodeId){
        return;
    }
    if(IsSumNode(nodeId)){
        //TODO:
    } else{
        //get the hyperedge num of nodeId
        int childrens_size = this->GetChildrens_Size(nodeId);
        //this pointer store the size of children for each hyperedge.
        int* ptr_children_size = this->GetChildren_Size(nodeId);
        //Get all hyperedges rooted by nodeId.
        int **ptr_childrens = this->GetChildren(nodeId);
        //Get the max value for each hyperedge rooted by nodeID.
        for(int children_k = 0; children_k < childrens_size; ++children_k){
            int *ptr_children_k = ptr_childrens[children_k];
            int size = ptr_children_size[children_k];
            if(IsIgnored(ptr_children_k, size)){
                continue;
            }
            FeatureArray *ptr_fa = this->ptr_param_l_->Extract(this,nodeId,ptr_children_k,children_k);
            double score = ptr_fa->GetScore(ptr_param_l_);
            for(int i=0; i < size; ++i){
                score += this->ptr_max_[ptr_children_k[i]];
            }
            if(score > ptr_max_[nodeId]){
                ptr_max_[nodeId] = score;
                ptr_max_children_no_[nodeId] = ptr_children_k;
            }
        }
    }
}

bool Network::IsSumNode(int nodeid) {
    return false;
}

bool Network::IsIgnored(int *ptr_children, int size) {
    for(int i = 0; i < size; ++i){
        if(IsRemovded(ptr_children[i])){
            return true;
        }
    }
    return false;
}

int* Network::GetMaxPath(int nodeid) {
    return ptr_max_children_no_[nodeid];
}

int Network::GetThreadId() {
    return thread_id_;
}

bool Network::GetNeuralInserted() {
    return neural_feature_inserted_;
}

void Network::SetNeuralInserted(bool flag) {
    neural_feature_inserted_ = flag;
}

