//
// Created by  ngs on 02/08/2018.
//

#include "discriminative_network_model.h"

DiscriminativeNetworkModel::DiscriminativeNetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc):NetworkModel(ptr_fm,ptr_nc) {

}

DiscriminativeNetworkModel::~DiscriminativeNetworkModel() {
    for(auto it = pptr_insts_->begin(); it != pptr_insts_->end(); ++it){
        delete (*it);
    }
    delete pptr_insts_;
    for(auto it = ptr_inst_matrix_->begin(); it != ptr_inst_matrix_->end(); ++it){
        delete (*it);
    }
    delete ptr_inst_matrix_;
}

std::vector<std::vector<Instance*>*> * DiscriminativeNetworkModel::SplitInstanceForTrain() {

    pptr_insts_ = new std::vector<std::vector<Instance*>*>;//Instance**[this->num_threads_];
    ptr_inst_matrix_ = new std::vector<std::vector<Instance*>*>;
    int threadId = 0;
    //create instance list for each thread.
    for(threadId = 0; threadId < this->num_threads_; ++threadId){
        std::vector<Instance*> *ptr_inst_vec = new std::vector<Instance*>;
        ptr_inst_matrix_->push_back(ptr_inst_vec);
    }
    threadId = 0;
    //allocate instances to different thread.
    int inst_all_size = this->ptr_inst_all_->size();
    for(int instId = 0; instId < inst_all_size; ++instId){
        //get the instance from instance vector
        Instance *ptr_inst = (*ptr_inst_all_)[instId];
        Instance *ptr_inst_du = (*ptr_inst_all_du_)[instId];
        //add the instance to a thread.
        (*ptr_inst_matrix_)[threadId]->push_back(ptr_inst);
        (*ptr_inst_matrix_)[threadId]->push_back(ptr_inst_du);
        threadId = (threadId + 1) % this->num_threads_;
    }
    //copy the instance pointer to an array pptr_insts_, which will be much faster than STL:vector.
    for(threadId = 0; threadId < this->num_threads_; ++threadId){
        int size = (*ptr_inst_matrix_)[threadId]->size();
        //allocate double space for the thread
        std::vector<Instance *> *ptr_vec = new std::vector<Instance*>[size];
        pptr_insts_->push_back(ptr_vec);
        for(int inst_no =0; inst_no < size; inst_no+=2){
            Instance *ptr_inst = (*(*ptr_inst_matrix_)[threadId])[inst_no];
            (*pptr_insts_)[threadId]->push_back(ptr_inst);
            //set the instance for unlabeled network
            Instance * ptr_ins_du = (*(*ptr_inst_matrix_)[threadId])[inst_no + 1];
            int instanceId = ptr_inst->GetInstanceId();
            double weight = ptr_inst->GetWeight();
            ptr_ins_du->SetInstanceId(-instanceId);
            ptr_ins_du->SetWeight(-weight);
            ptr_ins_du->SetUnlabeled();
            (*pptr_insts_)[threadId]->push_back(ptr_ins_du);
        }
    }
    return pptr_insts_;
}

