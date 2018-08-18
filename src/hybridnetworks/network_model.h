//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_NETWORK_MODEL_H
#define STATNLP_NETWORK_MODEL_H

#include <thread>
#include "../common/types/instance.h"
#include "feature_manager.h"
#include "network_compiler.h"
#include "common.h"
#include "local_network_learner_thread.h"
#include <vector>
#include <thread>
class NetworkModel{
public:
NetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc);
~NetworkModel();
void Train(std::vector<Instance *> *ptr_all_instances, std::vector<Instance *> *ptr_all_instances_du, int max_num_interations);
Instance* Decode(Instance *ptr_test_instences);
virtual std::vector<std::vector<Instance*>*>* SplitInstanceForTrain() = 0;
protected:
    FeatureManager *ptr_fm_;
    NetworkCompiler *ptr_nc_;
    int num_threads_;
    std::vector<Instance *> *ptr_inst_all_;
    std::vector<Instance *> *ptr_inst_all_du_;
    LocalNetworkLearnerThread **ptr_local_learner_vector_;
    std::thread *ptr_thread_vector_;
};

#endif //STATNLP_NETWORK_MODEL_H
