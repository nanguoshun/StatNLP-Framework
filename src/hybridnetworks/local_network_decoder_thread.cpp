//
// Created by  ngs on 22/08/2018.
//

#include "local_network_decoder_thread.h"

LocalNetworkDecoderThread::LocalNetworkDecoderThread(int threadid, std::vector<Instance *> *pptr_instance,
                                                     FeatureManager *ptr_fm, NetworkCompiler *ptr_nc) {
    this->thread_id_ = threadid;
    this->pptr_input_inst_vec_ = pptr_instance;
    this->sentence_size_ = pptr_instance->size();
    pptr_network_ = new Network*[sentence_size_];
    this->ptr_nc_ = ptr_nc;
    this->ptr_param_l_ = new LocalNetworkParam(threadid,ptr_fm,sentence_size_);
    ptr_fm->SetLocalNetworkParams(this->thread_id_,this->ptr_param_l_);
    this->ptr_param_l_->SetGlobalMode();
}

LocalNetworkDecoderThread::~LocalNetworkDecoderThread() {
    if(nullptr != ptr_param_l_){
        delete ptr_param_l_;
        ptr_param_l_ = nullptr;
    }
    for(int i = 0; i < sentence_size_; ++i){
        delete pptr_network_[i];
    }
    delete []pptr_network_;

    /*
    for(auto it = pptr_output_inst_vec_->begin(); it != pptr_output_inst_vec_->end(); ++it){
        delete (*it);
    }*/
    /*just release the instance vector that is allocated in this class, the instances are released in main function*/
    delete pptr_output_inst_vec_;
}

void LocalNetworkDecoderThread::Run() {
    for(int networkId = 0; networkId < sentence_size_; ++networkId){
        //build the network
        pptr_network_[networkId] = ptr_nc_->Compile(networkId,(*pptr_input_inst_vec_)[networkId],ptr_param_l_);
        //run cky/vertibi like algorithm
        this->ptr_param_l_->DisableCache();
        pptr_network_[networkId]->Max();
    }
    this->pptr_output_inst_vec_ = new std::vector<Instance *>;
    for(int i = 0; i < sentence_size_; ++i){
        //backtracking to get the result
        Instance *ptr_inst = this->ptr_nc_->Decompile(pptr_network_[i]);
        pptr_output_inst_vec_->push_back(ptr_inst);
    }
}

std::vector<Instance*> *LocalNetworkDecoderThread::GetOutPuts() {
    return this->pptr_output_inst_vec_;
}