//
// Created by  ngs on 31/07/2018.
//
#include "network_model.h"

NetworkModel::NetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc) {
    this->ptr_fm_ = ptr_fm;
    this->ptr_nc_ = ptr_nc;
    this->num_threads_ = Num_Of_Threads;
}

NetworkModel::~NetworkModel() {

}

void NetworkModel::Train(std::vector<Instance *> *ptr_all_instances, int max_num_interations) {
    this->num_threads_ = Num_Of_Threads;
    this->ptr_inst_all_ = ptr_all_instances;

    ptr_local_learner_vector_ = new std::vector<LocalNetworkLearnerThread *>(this->num_threads_);
    for(int threadId = 0; threadId < this->num_threads_; ++threadId){
        LocalNetworkLearnerThread *ptr_learner = new LocalNetworkLearnerThread(threadId,this->ptr_fm_,ptr_all_instances,this->ptr_nc_,-1);
        ptr_local_learner_vector_->push_back(ptr_learner);
        ptr_learner->Touch();
    }
}

Instance* NetworkModel::Decode(Instance *ptr_test_instences) {
    Instance * ptr;

    return  ptr;
}