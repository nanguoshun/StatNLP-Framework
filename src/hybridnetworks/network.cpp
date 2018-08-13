//
// Created by  ngs on 01/08/2018.
//

#include "network.h"
#include "common.h"

Network::Network() {

}

Network::Network(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param) {
    this->network_id_ = networkId;
    this->thread_id_ = ptr_param->GetThreadId();
    this->ptr_inst_ = ptr_inst;
    this->weight_ = this->ptr_inst_->GetWeight();
    this->ptr_param_ = ptr_param;
    ptr_inside_shared_array = new std::vector<std::vector<double>>(Num_Of_Threads);
    ptr_outside_shared_array = new std::vector<std::vector<double>>(Num_Of_Threads);
}

Network::~Network() {
    delete ptr_inside_shared_array;
    delete ptr_outside_shared_array;
}

void Network::Touch() {
    for (int k = 0; k < this->CountNodes(); ++k) {
        this->Touch(k);
    }
}

void Network::Touch(int k) {
    if(this->IsRemovded(k)){
        return;
    }
    int **ptr_children_vec = this->GetChildren(k);
    int size = sizeof(ptr_children_vec);
    for(int children_index = 0; children_index < size; ++children_index){
        int* ptr_childern_k = ptr_children_vec[children_index];
        //TODO: change the vector "childern_k" to a pointer.
        //this->ptr_param_->Extract(this,k,&childern_k,children_index);
    }
}

void Network::Train() {
    if(this->weight_ == 0){
        return;
    }
    this->Inside();
    this->Outside();
    this->UpdateInsideOutside();
}

void Network::Inside() {
    this->ptr_inside_ = GetInsideSharedArray();
    std::fill(ptr_inside_->begin(),ptr_inside_->end(),0);
    for(int nodeId=0; nodeId<this->CountNodes(); ++nodeId){
        this->Inside(nodeId);
    }
}

void Network::Outside() {
    this->ptr_outside_ = GetOutsideSharedArray();
    std::fill(ptr_outside_->begin(), ptr_outside_->end(),0);
    for(int nodeId = 0; nodeId < this->CountNodes(); ++ nodeId){
        this->Outside(nodeId);
    }
}

void Network::UpdateInsideOutside() {

}


void Network::Inside(int nodeId) {
    //TODO: change the vector to a pointer.
    //check if it is removed?
    if(this->IsRemovded(nodeId)){
        (*this->ptr_inside_)[nodeId] = std::numeric_limits<double>::infinity();
        return;;
    }
    //FIXME:
    std::vector<std::vector<int>> *ptr_children; //= this->GetChildren(nodeId);
    if(ptr_children->size() ==0){
        ptr_children = NULL;
    }
    double inside = 0.0;
    //for the 0th hyper edge
    int children_index = 0;
    std::vector<int> children = (*ptr_children)[children_index];
    bool ignore_flag = false;
    //for each node in the 0th hyper edge
    for(int child: children){
        if(this->IsRemovded(child)){
            ignore_flag = true;
        }
    }
    if(ignore_flag){
        inside = std::numeric_limits<double>::infinity();
    } else{
        //FIXME: the children should be a pointer here
        FeatureArray* ptr_fa ; //= this->ptr_param_->Extract(this,nodeId,&children,children_index);
        double score = ptr_fa->GetScore(this->ptr_param_);
        for(int child:children){
            score += (* this->ptr_inside_)[child];
        }
        inside = score;
    }
    //for each hyper edge that index is bigger than 1.
    for(int children_index = 1; children_index < ptr_children->size(); ++children_index){
        std::vector<int> children = (*ptr_children)[children_index];
        //for each node in a hyper edge.
        bool ignore_flag = false;
        for(int child: children){
            if(this->IsRemovded(child)){
                ignore_flag = true;
            }
        }
        if(ignore_flag){
            continue;
        }
        // to be done.
    }
}

void Network::Outside(int nodeId) {
    if(this->IsRemovded(nodeId)){
        (*this->ptr_outside_)[nodeId] = DOUBLE_NEGATIVE_INFINITY;
        return;
    } else{
        (*this->ptr_outside_)[nodeId] = this->IsRoot(nodeId) ? 0.0: (*this->ptr_outside_)[nodeId];
    }
    //to be done....
}

void Network::UpdateInsideOutside(int nodeId) {
}

// the shared array is dynamically allocated for each thread according to the number of the nodes.
std::vector<double>* Network::GetInsideSharedArray() {

}

std::vector<double>* Network::GetOutsideSharedArray() {

}

int Network::GetNetworkID() {
    return network_id_;
}

Instance* Network::GetInstance() {
    return ptr_inst_;
}