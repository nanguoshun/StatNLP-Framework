//
// Created by  ngs on 02/08/2018.
//

#include "local_network_learner_thread.h"

LocalNetworkLearnerThread::LocalNetworkLearnerThread(int threadId, FeatureManager *ptr_fm, std::vector<Instance *> *ptr_ins_vector,
                                                     NetworkCompiler *ptr_nc, int it) {
    this->thread_id_ = threadId;
    this->ptr_param_l_ = new LocalNetworkParam(threadId,ptr_fm, ptr_ins_vector->size());
    ptr_fm->SetLocalNetworkParams(threadId,ptr_param_l_);
    this->ptr_inst_vec_ = ptr_ins_vector;
    this->it_no_ = it;
    this->network_capcity_ = ComParam::NETWORK_CAPACITY;
    this->cache_networks_ = true;
    if(this->cache_networks_){
        this->ptr_network_ = new Network*[ptr_ins_vector->size()];
        for(int i=0; i<ptr_ins_vector->size(); ++i){
            this->ptr_network_[i] = nullptr;
        }
    }
    this->ptr_nc_ = ptr_nc;
}

LocalNetworkLearnerThread::~LocalNetworkLearnerThread() {
    delete ptr_param_l_;
    for(int i=0; i<ptr_inst_vec_->size(); ++i){
        delete ptr_network_[i];
    }
    delete []ptr_network_;
}

void LocalNetworkLearnerThread::Touch() {
//    int tmp_cout[4] = {0,0,0,0};
    int tmp_coun_value = 0;
    int size = ptr_inst_vec_->size();
    for(int networkId=0; networkId < this->ptr_inst_vec_->size(); ++networkId){
        this->GetNetwork(networkId)->Touch();
//        tmp_cout[networkId] = this->GetNetwork(networkId)->tmp_count_;
//        tmp_cout[networkId] = ptr_param_l_->GetFeatureManager()->temp_count_;
  //      tmp_cout[networkId] = ptr_param_l_->GetFeatureManager()->GetGlobalParam()->tmp_count_;
  //      tmp_coun_value += tmp_cout[networkId];
    }
    this->ptr_param_l_->FinalizeIt();
}

Network* LocalNetworkLearnerThread::GetNetwork(int networkId) {
    if(this->cache_networks_ && nullptr != this->ptr_network_[networkId]){
        return this-> ptr_network_[networkId];
    }
    Network *ptr_network = this->ptr_nc_->Compile(networkId,(*(this->ptr_inst_vec_))[networkId],this->ptr_param_l_);
    if(this->cache_networks_){
        this->ptr_network_[networkId] = ptr_network;
    }
    //NEED further observation for below code about capacity.
    if(ptr_network->CountNodes() > ComParam::NETWORK_CAPACITY){
        this->network_capcity_ = ptr_network->CountNodes();
    }
    return ptr_network;
}

void LocalNetworkLearnerThread::Train(){
    for(int networkId = 0; networkId < ptr_inst_vec_->size(); ++networkId){
        Network *ptr_network = this->GetNetwork(networkId);
        ptr_network->Train();
    }
}

void LocalNetworkLearnerThread::Run() {
    int size = this->ptr_inst_vec_->size();
    for(int networkId = 0; networkId < size ; ++networkId){
        Network *ptr_network = this->GetNetwork(networkId);
        ptr_network->Train();
    }
}