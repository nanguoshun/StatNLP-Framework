//
// Created by  ngs on 02/08/2018.
//

#include "local_network_learner_thread.h"

LocalNetworkLearnerThread::LocalNetworkLearnerThread(int threadId, FeatureManager *ptr_fm, std::vector<Instance *> *ptr_ins_vector,
                                                     NetworkCompiler *ptr_nc, int it) {
    this->thread_id_ = threadId;
    this->ptr_param_l_ = new LocalNetworkParam(threadId,ptr_fm, ptr_ins_vector->size());
    this->ptr_inst_vec_ = ptr_ins_vector;
}

LocalNetworkLearnerThread::~LocalNetworkLearnerThread() {
    delete ptr_param_l_;
}

void LocalNetworkLearnerThread::Touch() {
    for(int networkId=0; networkId < this->ptr_inst_vec_->size(); ++networkId){
        this->GetNetwork(networkId).Touch();
    }
    this->ptr_param_l_->FinalizeIt();
}

Network LocalNetworkLearnerThread::GetNetwork(int networkId) {

}