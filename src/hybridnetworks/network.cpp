//
// Created by  ngs on 01/08/2018.
//

#include "network.h"

Network::Network() {

}

Network::Network(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param) {
    this->network_id_ = networkId;
    this->thread_id_ = ptr_param->GetThreadId();
    this->ptr_inst_ = ptr_inst;
    this->weight_ = this->ptr_inst_->GetWeight();
    this->ptr_param_ = ptr_param;
}

Network::~Network() {

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
    std::vector<std::vector<int>> *ptr_children_vec = this->GetChildren(k);
    for(int children_index = 0; children_index < ptr_children_vec->size(); ++children_index){
        std::vector<int> childern_k = (*ptr_children_vec)[children_index];
        this->ptr_param_->Extract(this,k,&childern_k,children_index);
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
}

void Network::Outside() {
}

void Network::UpdateInsideOutside() {
}

void Network::Inside(int nodeId) {
}

void Network::Outside(int nodeId) {
}

void Network::UpdateInsideOutside(int nodeId) {
}