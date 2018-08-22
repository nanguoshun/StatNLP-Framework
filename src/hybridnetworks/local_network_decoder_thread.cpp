//
// Created by  ngs on 22/08/2018.
//

#include "local_network_decoder_thread.h"

LocalNetworkDecoderThread::LocalNetworkDecoderThread(int threadid, std::vector<Instance *> *pptr_instance,
                                                     FeatureManager *ptr_fm, NetworkCompiler *ptr_nc) {
    this->thread_id_ = threadid;
    this->pptr_input_inst = pptr_instance;
    this->ptr_nc_ = ptr_nc;
    this->ptr_param_l_ = new LocalNetworkParam(threadid,ptr_fm,pptr_instance->size());
    this->ptr_param_l_->SetGlobalMode();
}

LocalNetworkDecoderThread::~LocalNetworkDecoderThread() {
    delete ptr_param_l_;
}

void LocalNetworkDecoderThread::Run() {
    int sentence_size = pptr_input_inst->size();
    this->pptr_output_inst = new std::vector<Instance *>[sentence_size];
    for(int sentenceId = 0; sentenceId < sentence_size; ++sentenceId){
        //get the tag result of each input sentence, i.e., (*pptr_input_inst)[sentenceId]
        (*pptr_output_inst)[sentenceId] = Max((*pptr_input_inst)[sentenceId]);
    }
}

Instance* LocalNetworkDecoderThread::Max(Instance *ptr_inst) {

}

std::vector<Instance*> *LocalNetworkDecoderThread::GetOutPuts() {
    return this->pptr_output_inst;
}