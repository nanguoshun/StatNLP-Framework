//
// Created by  ngs on 18/10/2018.
//
#include <iostream>
#include "ptb_reader.h"
#include "rule.h"
#include "tree_crf_feature_manager.h"
#include "tree_crf_network_compiler.h"
#include "evaluate.h"
#include "src/hybridnetworks/global_network_param.h"
#include "src/hybridnetworks/feature_manager.h"
#include "src/hybridnetworks/discriminative_network_model.h"

int main(int argc, char **argv) {
    std::vector<std::string> label_vec;
    std::vector<Label *> *ptr_label_vec;
    std::string file_train = "data/ptb-binary.train.sample";
    std::string file_test = "data/ptb-binary.test.sample";
    int max_iterations = 200;
    std::unordered_map<std::string, int> *ptr_word2int_map = new std::unordered_map<std::string, int>;;
    std::pair<InstenceVector *, InstenceVector *> train_data = PTBReader::ReadPTB(file_train,ptr_word2int_map,true);
    std::vector<Instance *> *ptr_inst_vec = train_data.first;
    std::vector<Instance *> *ptr_inst_vec_dup = train_data.second;
    std::pair<InstenceVector *, InstenceVector *> test_data = PTBReader::ReadPTB(file_test,ptr_word2int_map,false);

    CFGRule::GetRules()

    std::vector<Instance *> *ptr_inst_vec_test = test_data.first;
    GlobalNetworkParam *ptr_g_param_ = new GlobalNetworkParam(argc,argv,PTBReader::max_len_,ptr_inst_vec->size(),&label_vec);
    TreeCRFFeatureManager *ptr_fm = new TreeCRFFeatureManager();
    TreeCRFNetworkCompiler *ptr_nc = new TreeCRFNetworkCompiler(ptr_label_vec);
    DiscriminativeNetworkModel *ptr_model = new DiscriminativeNetworkModel(ptr_fm,ptr_nc);
    ptr_model->Train(ptr_inst_vec,ptr_inst_vec_dup,max_iterations);
    ptr_model->Decode(ptr_inst_vec_test, true);
    Evaluate::EvaluateResult();
    //todo: 1.capacity
    //todo: 2.node id should be set when initialize data.
    //todo: 3.
}
