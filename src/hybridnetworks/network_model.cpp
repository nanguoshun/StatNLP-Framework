//
// Created by  ngs on 31/07/2018.
//
#include "network_model.h"

NetworkModel::NetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc) {
    this->ptr_fm_ = ptr_fm;
    this->ptr_nc_ = ptr_nc;
    this->num_threads_ = ComParam::Num_Of_Threads;
}

NetworkModel::~NetworkModel() {
  for(int threadId = 0; threadId < this->num_threads_; ++threadId){
      delete pptr_learner_[threadId];
      delete pptr_decoder_[threadId];
  }
  delete []pptr_learner_;
  delete []pptr_decoder_;
  delete []ptr_learn_thread_vector_;
  delete []ptr_decode_thread_vector_;

  for(auto it = ptr_split_inst_test_->begin(); it != ptr_split_inst_test_->end(); ++it){
      delete (*it);
  }
  delete ptr_split_inst_test_;
}

void NetworkModel::Train(std::vector<Instance *> *ptr_all_instances, std::vector<Instance *> *ptr_all_instances_du, int max_num_interations) {
    this->num_threads_ = ComParam::Num_Of_Threads;
    this->ptr_inst_all_ = ptr_all_instances;
    this->ptr_inst_all_du_ = ptr_all_instances_du;
    for(int inst_id = 0; inst_id < ptr_all_instances->size(); ++inst_id){
        (*ptr_inst_all_)[inst_id]->SetInstanceId(inst_id+1);
    }
    pptr_learner_ = new LocalNetworkLearnerThread*[this->num_threads_];
    std::vector<std::vector<Instance*>*> *ptr_inst = this->SplitInstanceForTrain();
    for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
        pptr_learner_[threadId] = new LocalNetworkLearnerThread(threadId, this->ptr_fm_,
                                                                            (*ptr_inst)[threadId], this->ptr_nc_, -1);
        pptr_learner_[threadId]->Touch();
    }
    //
    this->ptr_fm_->GetGlobalParam()->LockIt();
    std::cout <<"tmp cout is: "<<this->ptr_fm_->temp_count_<<std::endl;
    std::cout <<"tmp cout is: "<<this->ptr_fm_->GetGlobalParam()->tmp_count_<<std::endl;
    double obj_old = ComParam::DOUBLE_NEGATIVE_INFINITY;
    //EM algorithm
    long start_time = clock();
    ptr_learn_thread_vector_ = new std::thread[this->num_threads_];
    for (int i = 0; i < max_num_interations; ++i) {
        long time = clock();
        for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
            //this->pptr_learner_[threadId]
            ptr_learn_thread_vector_[threadId] = std::thread(&LocalNetworkLearnerThread::Run,pptr_learner_[threadId]);
        }
        for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
            //this->pptr_learner_[threadId]
            ptr_learn_thread_vector_[threadId].join();
        }
        bool done = this->ptr_fm_->Update();
        double obj = this->ptr_fm_->GetGlobalParam()->GetOldObj();
        time = clock() - time;
        std::cout << "Iteration: " << i << " Obj: " << obj << " Time: " << (double) time / (double)CLOCKS_PER_SEC <<
                  " Convergence: " << obj / obj_old << " Total Time: " << (double) (clock() - start_time) / (double)CLOCKS_PER_SEC << std::endl;
        obj_old = obj;
        if (done) {
            std::cout << "Training complete after " << i << " iterations" << std::endl;
            break;
        }
    }
}

std::vector<Instance *>* NetworkModel::Decode(std::vector<Instance *> *ptr_test_instences) {
    this->ptr_inst_all_test_ = ptr_test_instences;
    this->num_threads_ = ComParam::Num_Of_Threads;
    this->pptr_decoder_ = new LocalNetworkDecoderThread*[this->num_threads_];
    SplitInstanceForTest();
    for(int threadid = 0; threadid < this->num_threads_; ++threadid){
        this->pptr_decoder_[threadid] = new LocalNetworkDecoderThread(threadid,(*ptr_split_inst_test_)[threadid],ptr_fm_,ptr_nc_);
    }
    this->ptr_decode_thread_vector_ = new std::thread[this->num_threads_];
    for(int threadid = 0; threadid < this->num_threads_; ++threadid){
        ptr_decode_thread_vector_[threadid] = std::thread(&LocalNetworkDecoderThread::Run,pptr_decoder_[threadid]);
    }
    for(int threadid = 0; threadid < this->num_threads_; ++threadid) {
        ptr_decode_thread_vector_[threadid].join();
    }
    std::cout <<"decode done"<<std::endl;
    std::vector<Instance *> *ptr_result = new std::vector<Instance *>;
    for(int threadid = 0; threadid < this->num_threads_; ++threadid){
        std::vector<Instance *> *ptr_outputs = pptr_decoder_[threadid]->GetOutPuts();
        for(auto it = ptr_outputs->begin(); it != ptr_outputs->end(); ++it){
            ptr_result->push_back((*it));
        }
    }
    return ptr_result;
}

/**
 *
 * Allocate training instences to different threads.
 *
 * @return
 */
void NetworkModel::SplitInstanceForTest() {
    int inst_size = ptr_inst_all_test_->size();
    ptr_split_inst_test_ = new std::vector<std::vector<Instance*>*>;
    std::vector<std::vector<int>> thread_inst_vec(this->num_threads_);
    //uniformly allocate instances to different threads by its id.
    int threadId = 0;
    for(int instId = 0; instId < inst_size; ++instId){
       thread_inst_vec[threadId].push_back(instId);
       threadId = (threadId + 1) % this->num_threads_;
    }
    for(int threadId =0; threadId < this->num_threads_; ++threadId){
        int size = thread_inst_vec[threadId].size();
        std::vector<Instance *> *ptr_vec_inst = new std::vector<Instance *>;
        for(int i = 0; i < size; ++i){
            int inst_no = thread_inst_vec[threadId][i];
            Instance *ptr_inst = (*ptr_inst_all_test_)[inst_no];
            ptr_vec_inst->push_back(ptr_inst);
        }
        ptr_split_inst_test_->push_back(ptr_vec_inst);
    }
}