//
// Created by  ngs on 01/08/2018.
//

#include "network.h"
#include "common.h"

double **Network::ptr_inside_shared_array_ = new double*[ComParam::Num_Of_Threads];
double **Network::ptr_outside_shared_array_ = new double*[ComParam::Num_Of_Threads];

bool Network::is_initialized_shared_array_ = false;
Network::Network() {
std::cout << "no param"<<std::endl;
}

Network::Network(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param) {
    this->network_id_ = networkId;
    this->thread_id_ = ptr_param->GetThreadId();
    this->ptr_inst_ = ptr_inst;
    this->weight_ = this->ptr_inst_->GetWeight();
    this->ptr_param_ = ptr_param;
    tmp_count_ = 0;
    if(!is_initialized_shared_array_){
        is_initialized_shared_array_ = true;
        for(int threadid = 0; threadid < ComParam::Num_Of_Threads; ++threadid){
            ptr_inside_shared_array_[threadid] = nullptr;
            ptr_outside_shared_array_[threadid] = nullptr;
        }
    }
}

Network::~Network() {

}

void Network::Touch() {
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
        this->ptr_param_->Extract(this,node_index,ptr_children_k,children_index);
        tmp_count_++;
    }
}

void Network::Train() {
    if(this->weight_ == 0){
        return;
    }
    this->Inside();
    this->Outside();
    this->UpdateInsideOutside();
    this->ptr_param_->AddObj(this->GetInside() * weight_);
}

void Network::Inside() {
    this->ptr_inside_ = GetInsideSharedArray();
    //init the inside value of each node.
    for(int nodeId=0; nodeId<this->CountNodes(); ++nodeId){
        ptr_inside_[nodeId] = 0;
    }
    for(int nodeId=0; nodeId<this->CountNodes(); ++nodeId){
        this->Inside(nodeId);
    }
}

void Network::Outside() {
    this->ptr_outside_ = GetOutsideSharedArray();
    //init the outside value of each node.
    for(int nodeId=0; nodeId<this->CountNodes(); ++nodeId){
        ptr_outside_[nodeId] = 0;
    }
    for(int nodeId = 0; nodeId < this->CountNodes(); ++ nodeId){
        this->Outside(nodeId);
    }
}

void Network::UpdateInsideOutside() {
    for(int nodeId = 0; nodeId < this->CountNodes(); ++nodeId){
        this->UpdateGradient(nodeId);
    }
}

void Network::Inside(int nodeId) {
    //check if it is removed?
    if(this->IsRemovded(nodeId)){
        ptr_inside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
        return;;
    }

    //get the number of hyperedges rooted by nodeId
    int children_size = this->GetChildrens_Size(nodeId);
    //get the hyperedges rooted by nodeId.
    int **ptr_childrens_vec_ = this->GetChildren(nodeId);
    //if the number of hyperedges rooted by nodeId is zero, then return;
    if( 0 == children_size){
        return;
    }
    double inside = 0.0;

    //for the 0th hyperedge
    int children_k = 0;
    //the pointer to all nodes of the 0th hyperedge
    int * ptr_children_k = ptr_childrens_vec_[children_k];
    //the num of nodes in the 0th hyperedge that is rootd by nodeID.
    int child_k_size = (this->GetChildren_Size(nodeId))[children_k];
    bool ignore_flag = false;
    //for each node in the 0th hyperedge
    for(int child_no = 0; child_no < child_k_size; ++child_no){
        if(this->IsRemovded(ptr_children_k[child_no])){
            ignore_flag = true;
        }
    }
    if(ignore_flag){
        inside = ComParam::DOUBLE_NEGATIVE_INFINITY;
    } else{
        FeatureArray* ptr_fa = this->ptr_param_->Extract(this,nodeId, ptr_children_k,children_k);
        double score = ptr_fa->GetScore(this->ptr_param_);
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            score += ptr_inside_[child_no];
        }
        inside = score;
    }

    //for each hyper edge that index is bigger than 1.
    for(int children_k = 1; children_k < children_size; ++children_k){
        int *ptr_children_k = ptr_childrens_vec_[children_k];
        int child_k_size = (this->GetChildren_Size(nodeId))[children_k];
        //for each node in a hyper edge.
        bool ignore_flag = false;
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            if(this->IsRemovded(ptr_children_k[child_no])){
                ignore_flag = true;
            }
        }
        if(ignore_flag){
            continue;
        }
        FeatureArray *ptr_fa = this->ptr_param_->Extract(this, nodeId, ptr_children_k, children_k);
        double score = ptr_fa->GetScore(this->ptr_param_);
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            score += this->ptr_inside_[ptr_children_k[child_no]];
        }
        //logsum value;
        double v1 = inside;
        double v2 = score;
        if( v1 == v2 && v2 == ComParam::DOUBLE_NEGATIVE_INFINITY){
            inside = ComParam::DOUBLE_NEGATIVE_INFINITY;
        } else if(v1 == v2 && v2 == ComParam::DOUBLE_NEGATIVE_INFINITY){
            inside = ComParam::DOUBLE_NEGATIVE_INFINITY;
        } else if(v1 > v2){
            inside = std::log(std::exp(score-inside)) + inside;
        } else {
            inside = std::log(std::exp(inside-score)) + score;
        }
        ptr_inside_[nodeId] = inside;
        if(ptr_inside_[nodeId] == ComParam::DOUBLE_NEGATIVE_INFINITY){
            this->Remove(nodeId);
        }
    }
}

void Network::Outside(int nodeId) {
    if(this->IsRemovded(nodeId)){
        ptr_outside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
        return;
    } else{
        this->ptr_outside_[nodeId] = this->IsRoot(nodeId) ? 0.0: ptr_outside_[nodeId];
    }

    if(this->ptr_inside_[nodeId] == ComParam::DOUBLE_NEGATIVE_INFINITY){
        ptr_outside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
    }
    //get the vector of hyperedges rooted by nodeId.
    int **ptr_children_vec = this->GetChildren(nodeId);
    //get the number of hyperedges rooted by nodeID
    int children_size = this->GetChildrens_Size(nodeId);
    for(int children_k = 0; children_k < children_size; ++children_k){
        int *ptr_children_k = ptr_children_vec[children_k];
        int child_k_size = this->GetChildren_Size(nodeId)[children_k];
        bool ignore_flag = false;
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            if(this->IsRemovded(ptr_children_k[child_no])){
                ignore_flag = true;
                break;
            }
        }
        if(ignore_flag){ continue;}
        FeatureArray *ptr_fa = this->ptr_param_->Extract(this,nodeId,ptr_children_k,children_k);
        double score = ptr_fa->GetScore(ptr_param_);
        score += ptr_outside_[nodeId];
        for( int child_no = 0; child_no < child_k_size; ++child_no){
            score += ptr_inside_[ptr_children_k[child_no]];
        }

        if(score == ComParam::DOUBLE_NEGATIVE_INFINITY){continue;}

        for(int child_no = 0; child_no < child_k_size; ++ child_no){
            double v1 = ptr_outside_[ptr_children_k[child_no]];
            double v2 = score - ptr_inside_[ptr_children_k[child_no]];
            if(v1 > v2){
                ptr_outside_[child_no] = v1 + std::log(std::exp(v2-v1));
            } else{
                ptr_outside_[child_no] = v2 + std::log(std::exp(v1-v2));
            }
        }
    }
    if(ptr_outside_[nodeId] == ComParam::DOUBLE_NEGATIVE_INFINITY){
        this->Remove(nodeId);
    }
}
/**
 *
 *Calc the gradient for each features of a node, which is indexed by nodeId.
 *
 * @param nodeId
 */
void Network::UpdateGradient(int nodeId) {
    if(this->IsRemovded(nodeId)){
        return;
    }
    int **ptr_children_vec = this->GetChildren(nodeId);
    int children_k_size = this->GetChildrens_Size(nodeId);
    //update each hyperedge.
    for(int children_k = 0; children_k < children_k_size; ++children_k){
        int *ptr_children_k = ptr_children_vec[children_k];
        int child_k_size = this->GetChildren_Size(nodeId)[children_k];
        bool ignore_flag = false;
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            if(this->IsRemovded(ptr_children_k[child_no])){
                ignore_flag = true;
                break;
            }
        }
        if(ignore_flag){
            continue;
        }
        FeatureArray *ptr_fa = this->ptr_param_->Extract(this, nodeId, ptr_children_k, children_k);
        double score = ptr_fa->GetScore(this->ptr_param_);
        score += this->ptr_outside_[nodeId];
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            score += this->ptr_inside_[ptr_children_k[child_no]];
        }
        //
        double normalization = this->GetInside();
        double count = std::exp(score - normalization);
        count *= weight_;
        ptr_fa->Update(ptr_param_,count);
    }
}

// the shared array is dynamically allocated for each thread according to the number of the nodes.
double* Network::GetInsideSharedArray() {
    if(!ptr_inside_shared_array_[this->thread_id_] || this->CountNodes() > inside_shared_array_size_){
        ptr_inside_shared_array_[this->thread_id_] = new double[this->CountNodes()];
    }
    return ptr_inside_shared_array_[this->thread_id_];
}

double* Network::GetOutsideSharedArray() {
    if(!ptr_outside_shared_array_[this->thread_id_] || this->CountNodes() > outside_shared_array_size_){
        ptr_outside_shared_array_[this->thread_id_] = new double[this->CountNodes()];
    }
    return ptr_outside_shared_array_[this->thread_id_];
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

double Network::GetInside() {
    if(nullptr == ptr_inside_){
        std::cerr<< "ERROR: the empty pointer of ptr_inside"<<std::endl;
    }
    return ptr_inside_[this->CountNodes()-1];
}