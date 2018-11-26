//
// Created by  ngs on 25/11/2018.
//

#ifndef STATNLP_TEST_H
#define STATNLP_TEST_H

#include "test_instance.h"
#include "test_featuremgr.h"
#include "test_compiler.h"
#include <iostream>
#include "src/hybridnetworks/global_network_param.h"
#include "src/hybridnetworks/discriminative_network_model.h"

int main(int argc, char **argv) {
    std::cout << "test"<<std::endl;
    std::unordered_map<std::string, int> *ptr_word2int_map = new std::unordered_map<std::string, int>;
    std::vector<Instance *> *ptr_inst_vec_train;// = training_data_pair.first;
    std::vector<Instance *> *ptr_inst_vec_train_dup;// = training_data_pair.second;
    int size = ptr_inst_vec_train->size();
    std::cout << "the vec size is " << size << std::endl;
    std::cout << "the size of dup vec is "<<ptr_inst_vec_train_dup->size()<<std::endl;
    std::vector<Instance *> *ptr_inst_vec_test;//
    //size = ptr_inst_vec_test->size();
    int num_iterations = 1500;
    GlobalNetworkParam *ptr_g_param = nullptr;
    std::vector<std::string> label{"0","1"};
    if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        ptr_g_param = new GlobalNetworkParam(argc, argv, 2, ptr_inst_vec_train->size(),&label,
                                             ComType::NeuralType::LSTM, ptr_word2int_map);
    } else if (ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type) {
        ptr_g_param = new GlobalNetworkParam(argc, argv, 2, ptr_inst_vec_train->size(),&label);
    }
    TestFeatureMgr *ptr_fm = new TestFeatureMgr(ptr_inst_vec_train, ptr_g_param);
    TestCompiler *ptr_nc;/// = new TestCompiler(true, &label);
    NetworkModel *ptr_nm = new DiscriminativeNetworkModel(ptr_fm, ptr_nc);
    ptr_nm->Train(ptr_inst_vec_train, ptr_inst_vec_train_dup, num_iterations);
    std::vector<Instance *> *ptr_predictions = ptr_nm->Decode(ptr_inst_vec_test, false);
    //EvaluateNER::Evaluate(ptr_predictions,"data/conll2003/output/neroutput");

}

#endif //STATNLP_TEST_H
