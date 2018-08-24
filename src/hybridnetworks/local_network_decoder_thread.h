//
// Created by  ngs on 22/08/2018.
//

#ifndef STATNLP_LOCAL_NETWORK_DECODER_THREAD_H
#define STATNLP_LOCAL_NETWORK_DECODER_THREAD_H

#include "local_network_param.h"
#include "../common/types/instance.h"
#include "network_compiler.h"

class LocalNetworkDecoderThread{
public:
    LocalNetworkDecoderThread(int threadid, std::vector<Instance*> *pptr_instance, FeatureManager *ptr_fm, NetworkCompiler *ptr_nc);
    ~LocalNetworkDecoderThread();
    void Run();
    std::vector<Instance*> * GetOutPuts();
private:
    int thread_id_;
    LocalNetworkParam *ptr_param_l_;
    std::vector<Instance*> *pptr_input_inst_vec_;
    std::vector<Instance*> *pptr_output_inst_vec_;
    NetworkCompiler *ptr_nc_;
    Network **pptr_network_;
    int sentence_size_;
};

#endif //STATNLP_LOCAL_NETWORK_DECODER_THREAD_H
