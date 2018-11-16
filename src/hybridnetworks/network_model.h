//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_NETWORK_MODEL_H
#define STATNLP_NETWORK_MODEL_H

#include <thread>
#include "../common/types/instance.h"
#include "feature_manager.h"
#include "network_compiler.h"
#include "src/common/types/common.h"
#include "local_network_learner_thread.h"
#include "local_network_decoder_thread.h"
#include <vector>
#include <iomanip>
#include "src/common/opt/tools.h"
#include "src/common/opt/ctpl.h"

using namespace std::chrono;

class NetworkModel {

public:
    NetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc);

    ~NetworkModel();

    void Train(std::vector<Instance *> *ptr_all_instances, std::vector<Instance *> *ptr_all_instances_du,
               int max_num_interations);

    std::vector<Instance *> *Decode(std::vector<Instance *> *ptr_test_instences, bool is_cache_features);

    virtual std::vector<std::vector<Instance *> *> *SplitInstanceForTrain() = 0;

    void SplitInstanceForTest();

    int GetCurrentMillionSeconds();

    void SetPreMemorySize(long size);
protected:
    FeatureManager *ptr_fm_;
    NetworkCompiler *ptr_nc_;
    int num_threads_;
    //instance for labeled network.
    std::vector<Instance *> *ptr_inst_all_;
    //duplicate instance for unlabeled network.
    std::vector<Instance *> *ptr_inst_all_du_;
    LocalNetworkLearnerThread **pptr_learner_;
    LocalNetworkDecoderThread **pptr_decoder_;
    std::vector<std::thread> *ptr_learn_thread_vector_;
    std::vector<std::thread> *ptr_decode_thread_vector_;
    std::vector<Instance *> *ptr_inst_all_test_;
    std::vector<std::vector<Instance *> *> *ptr_split_inst_test_;
    GlobalNeuralNetworkParam *ptr_nn_g_;
    /*for memory testing*/
    long pre_memory_size_;
    long cur_memory_size_;
};

#endif //STATNLP_NETWORK_MODEL_H
