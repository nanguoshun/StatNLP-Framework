//
// Created by  ngs on 01/08/2018.
//

#include "network.h"
#include "common.h"

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
}

Network::~Network() {
    delete []ptr_max_;
    //the content of ptr_max_children_no_ are pointers, which are allocated in the class TableLookupNetwork
    delete []ptr_max_children_no_;
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
    this->UpdateInsideOutside();
    this->ptr_param_l_->AddObj(this->GetInside(this->CountNodes()-1) * weight_);
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
    double inside = 0.0;
    if(nullptr == ptr_childrens_vec_){
        ptr_inside_[nodeId] = inside;
        return;
    }
    /**
     *  ****************Linear CRF***********************
     * summation of all inside values in a hyperedge
     * The probability expression of linear inside(or forward) value is:
     *  \alpha_j(s) = \sum_s' \alpha_{j-1}(s') * \psi_j(X,s,s')
     * Where \psi_j(X,s,s') denotes the score value for a hypeedge from state s' to s.
     *
     * The probability expression of linear inside(or forward) value is:
     * while in log space, the inside(or forward) value is:
     *  \alpha'_j(s) = \log \sum_s' \exp (\alpha'_{j-1}(s') + \log \psi_j(X,s,s'))
     * Actually, we can omit the log symbol before the score expression \psi_j(X,s,s').
     *
     * ****************Tree based CRF*********************
     *  A Hypedge with a parent node and multiple children is considered as a single edge both
     *  in inside and outside calculation.
     *
     *
     */
    //for the 0th hyperedge, we don't need logsum here and hence it is calculated independently.
    int children_k = 0;
    //the pointer to all nodes of the 0th hyperedge
    int *ptr_children_k = ptr_childrens_vec_[children_k];
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
        FeatureArray* ptr_fa = this->ptr_param_l_->Extract(this,nodeId, ptr_children_k,children_k);
        /**
         * calc the score summation of all features in this hyperedge. Each score is the production
         * of feature value and its weight.
         */
        double score = ptr_fa->GetScore(this->ptr_param_l_);
        /**
         * for the first hyperedge, no log sum is required.
         * \alpa'_j(s) = \alpha'_{j-1}(s') +  \psi_j(X,s,s')
         */
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            score += ptr_inside_[child_no];
        }
        inside = score;
    }
    //For each hyper edge that index is bigger than 1. we need logsum here.
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
        FeatureArray *ptr_fa = this->ptr_param_l_->Extract(this, nodeId, ptr_children_k, children_k);
        /**
         * Calc sore summation for all features in a specific hyperedge.
         */
        double score = ptr_fa->GetScore(this->ptr_param_l_);
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
/**
 * Calc the outside value for all children nodes, whose hyperedge is rooted by nodeId.
 *
 * @param nodeId
 */
void Network::Outside(int nodeId) {
    if(this->IsRemovded(nodeId)){
        ptr_outside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
        return;
    } else{
        //check whether the node is root or not.
        this->ptr_outside_[nodeId] = this->IsRoot(nodeId) ? 0.0: ptr_outside_[nodeId];
    }

    if(this->ptr_inside_[nodeId] == ComParam::DOUBLE_NEGATIVE_INFINITY){
        ptr_outside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
    }
    //get the vector of hyperedges rooted by nodeId.
    int **ptr_children_vec = this->GetChildren(nodeId);
    //get the number of hyperedges rooted by nodeID
    int children_size = this->GetChildrens_Size(nodeId);
    //calculate all hyperedges rooted by nodeId.
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
        FeatureArray *ptr_fa = this->ptr_param_l_->Extract(this,nodeId,ptr_children_k,children_k);
        //calculate the score summation of the hyeredge.
        double score = ptr_fa->GetScore(ptr_param_l_);
        //sum the outside value of parent node.
        score += ptr_outside_[nodeId];
        //sum all inside value of children nodes.
        for( int child_no = 0; child_no < child_k_size; ++child_no){
            score += ptr_inside_[ptr_children_k[child_no]];
        }
        if(score == ComParam::DOUBLE_NEGATIVE_INFINITY){continue;}
        //calc the outside values of all children nodes.
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            double v1 = ptr_outside_[ptr_children_k[child_no]];
            /**
             * recall that in linear CRF, there is only one child node
             * and hence the inside value is totally eliminated.
             */
            double v2 = score - ptr_inside_[ptr_children_k[child_no]];
            if(v1 > v2){
                //log sum computation.
                ptr_outside_[ptr_children_k[child_no]] = v1 + std::log(std::exp(v2-v1));
            } else{
                ptr_outside_[ptr_children_k[child_no]] = v2 + std::log(std::exp(v1-v2));
            }
        }
    }
    if(ptr_outside_[nodeId] == ComParam::DOUBLE_NEGATIVE_INFINITY){
        this->Remove(nodeId);
    }
}
/**
 *
 * Calc the gradient the  node, which is indexed by nodeId.
 *
 * Noted that the gradient for all features
 *
 * @param nodeId
 */
void Network::UpdateGradient(int nodeId) {
    if(this->IsRemovded(nodeId)){
        return;
    }
    int **ptr_children_vec = this->GetChildren(nodeId);
    int children_k_size = this->GetChildrens_Size(nodeId);
    //update each hyper-edge.
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
        FeatureArray *ptr_fa = this->ptr_param_l_->Extract(this, nodeId, ptr_children_k, children_k);
        double score = ptr_fa->GetScore(this->ptr_param_l_);
        score += this->ptr_outside_[nodeId];
        for(int child_no = 0; child_no < child_k_size; ++child_no){
            score += this->ptr_inside_[ptr_children_k[child_no]];
        }
        double normalization = this->GetInside(this->CountNodes()-1);
        double count = std::exp(score - normalization);
        count *= weight_;
        /**
         * update the gradient of all features in this hyperedge.
         * Noted that the gradient of all features are equivalent in a same hyperedge.
         *
         * Details pls refer to Equation. 61 of Classical Probabilistic Models and Conditional Random Fields. Roman Klinger. 2007
         *
         */
        ptr_fa->Update(ptr_param_l_,count);
    }
}

// the shared array is dynamically allocated for each thread according to the number of the nodes.
double* Network::GetInsideSharedArray() {
    double **pptr_inside_array_ = ptr_param_g_->GetInsideSharedArray();
    int *ptr_array_size = ptr_param_g_->GetInsideSharedArraySize();
    if(nullptr == pptr_inside_array_[this->thread_id_] || this->CountNodes() > ptr_array_size[this->thread_id_]){
        //FIXME:
        //ptr_inside_shared_array_[this->thread_id_] = new double[this->CountNodes()];
        pptr_inside_array_[this->thread_id_] = new double[this->CountNodes()];
        ptr_array_size[this->thread_id_] = this->CountNodes();
    }
    return pptr_inside_array_[this->thread_id_];
}

double* Network::GetOutsideSharedArray() {
    double **pptr_ouside_array = ptr_param_g_->GetOutsideSharedArray();
    int *ptr_array_size = ptr_param_g_->GetOutsideSharedArraySize();
    if(!pptr_ouside_array[this->thread_id_] || this->CountNodes() > ptr_array_size[this->thread_id_] ){
        pptr_ouside_array[this->thread_id_] = new double[this->CountNodes()];
        ptr_array_size[this->thread_id_] = this->CountNodes();
    }
    return pptr_ouside_array[this->thread_id_];
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
            if(IsIngored(ptr_children_k,size)){
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

bool Network::IsIngored(int *ptr_children, int size) {
    for(int i = 0; i < size; ++i){
        if(IsRemovded(ptr_children[i])){
            return true;
        }
    }
    return false;
}

int* Network::GetPath(int nodeid) {
    return ptr_max_children_no_[nodeid];
}