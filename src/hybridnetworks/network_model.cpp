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

void NetworkModel::Train(std::vector<Instance *> *ptr_all_instances, std::vector<Instance *> *ptr_all_instances_du, int max_num_interations) {
    this->num_threads_ = Num_Of_Threads;
    this->ptr_inst_all_ = ptr_all_instances;
    this->ptr_inst_all_du_ = ptr_all_instances_du;

    for(int inst_id = 0; inst_id < ptr_all_instances->size(); ++inst_id){
        (*ptr_inst_all_)[inst_id]->SetInstanceId(inst_id+1);
    }

    ptr_local_learner_vector_ = new LocalNetworkLearnerThread*[this->num_threads_];

    std::vector<std::vector<Instance*>*> *ptr_inst = this->SplitInstanceForTrain();

    for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
        ptr_local_learner_vector_[threadId] = new LocalNetworkLearnerThread(threadId, this->ptr_fm_,
                                                                            (*ptr_inst)[threadId], this->ptr_nc_, -1);
        ptr_local_learner_vector_[threadId]->Touch();
    }
    //
    this->ptr_fm_->GetGlobalParam()->LockIt();

    std::cout <<"tmp cout is: "<<this->ptr_fm_->temp_count_<<std::endl;
    std::cout <<"tmp cout is: "<<this->ptr_fm_->GetGlobalParam()->tmp_count_<<std::endl;

    double obj_old = DOUBLE_NEGATIVE_INFINITY;
    //EM algorithm
    long start_time = clock();
    ptr_thread_vector_ = new std::thread[this->num_threads_];
    for (int i = 0; i < max_num_interations; ++i) {
        long time = clock();
        for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
            //this->ptr_local_learner_vector_[threadId]
            ptr_thread_vector_[threadId] = std::thread(&LocalNetworkLearnerThread::Run,ptr_local_learner_vector_[threadId]);
        }
        for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
            //this->ptr_local_learner_vector_[threadId]
            ptr_thread_vector_[threadId].join();
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