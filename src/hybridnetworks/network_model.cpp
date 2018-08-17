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

    ptr_local_learner_vector_ = new LocalNetworkLearnerThread*[this->num_threads_];

    for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
        ptr_local_learner_vector_[threadId] = new LocalNetworkLearnerThread(threadId, this->ptr_fm_,
                                                                               ptr_all_instances, this->ptr_nc_, -1);
        ptr_local_learner_vector_[threadId]->Touch();
    }
    //
    this->ptr_fm_->GetGlobalParam()->LockIt();

    double obj_old = DOUBLE_NEGATIVE_INFINITY;
    //EM algorithm
    long start_time = clock();
    for (int i = 0; i < max_num_interations; ++i) {
        long time = clock();
        for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
            //this->ptr_local_learner_vector_[threadId]
        }
        for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
            //this->ptr_local_learner_vector_[threadId]
        }

        bool done = false;
        this->ptr_fm_->Update();
        double obj = this->ptr_fm_->GetGlobalParam()->GetOldObj();
        time = clock() - time;
        std::cout << "Iteration: " << i << " Obj: " << obj << " Time: " << time / 1000.0 <<
                  " Convergence: " << obj / obj_old << " Total Time: " << clock() - start_time << std::endl;
        obj_old = obj;
        if (done) {
            std::cout << "Training complete after " << i << " iterations" << std::endl;
            break;
        }
    }
}

Instance* NetworkModel::Decode(Instance *ptr_test_instences) {
    Instance * ptr;

    return  ptr;
}