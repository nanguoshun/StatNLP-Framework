//
// Created by  ngs on 22/08/2018.
//

#include "local_network_decoder_thread.h"

LocalNetworkDecoderThread::LocalNetworkDecoderThread(int threadid, std::vector<Instance *> *pptr_instance,
                                                     FeatureManager *ptr_fm, NetworkCompiler *ptr_nc) {
    this->thread_id_ = threadid;
    this->pptr_input_inst = pptr_instance;
    this->sentence_size_ = pptr_instance->size();
    pptr_network_ = new Network*[sentence_size_];
    this->ptr_nc_ = ptr_nc;
    this->ptr_param_l_ = new LocalNetworkParam(threadid,ptr_fm,sentence_size_);
    this->ptr_param_l_->SetGlobalMode();
}

LocalNetworkDecoderThread::~LocalNetworkDecoderThread() {
    delete ptr_param_l_;
    for(int i = 0; i < sentence_size_; ++i){
        delete pptr_network_[i];
    }
    delete []pptr_network_;
}

void LocalNetworkDecoderThread::Run() {
    for(int networkId = 0; networkId < sentence_size_; ++networkId){
        //build the network
        pptr_network_[networkId] = ptr_nc_->Compile(networkId,(*pptr_input_inst)[networkId],ptr_param_l_);
        //run cky/vertibi like algorithm
        pptr_network_[networkId]->Max();
    }
    this->pptr_output_inst = new std::vector<Instance *>[sentence_size_];
    for(int i = 0; i < sentence_size_; ++i){
        //backtracking to get the result
        (*pptr_output_inst)[i] = this->ptr_nc_->Decompile(pptr_network_[i]);
    }
}

std::vector<Instance*> *LocalNetworkDecoderThread::GetOutPuts() {
    return this->pptr_output_inst;
}