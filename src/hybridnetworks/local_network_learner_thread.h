//
// Created by  ngs on 02/08/2018.
//

#ifndef STATNLP_LOCALNETWORKLEARNERTHREAD_H
#define STATNLP_LOCALNETWORKLEARNERTHREAD_H

#include "feature_manager.h"
#include "../common/types/instance.h"
#include "network_compiler.h"

class LocalNetworkLearnerThread{
public:
    LocalNetworkLearnerThread(int threadId, FeatureManager *ptr_fm, std::vector<Instance *> *ptr_ins_vector, NetworkCompiler *ptr_nc, int it);
    ~LocalNetworkLearnerThread();
    void Touch();
    Network* GetNetwork(int networkId);
    void Train();
protected:
    int thread_id_;
    LocalNetworkParam *ptr_param_l_;
    Network *ptr_network_;
    std::vector<Instance *> *ptr_inst_vec_;
    NetworkCompiler *ptr_nc_;
    int network_capcity_;
    int it_no_;
};
#endif //STATNLP_LOCALNETWORKLEARNERTHREAD_H
