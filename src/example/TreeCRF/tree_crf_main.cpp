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
#include <unordered_map>
#include "src/common/opt/tools.h"

void ReleaseStaticMemory(){
    /* release memory allocated in rule.h */
    for(auto it = CFGRule::ptr_rule_label_map_->begin(); it != CFGRule::ptr_rule_label_map_->end(); ++it){
        std::set<CFG_Rule> *ptr_rule_set = (*it).second;
        delete ptr_rule_set;
    }
    delete CFGRule::ptr_rule_label_map_;
    /* release memory allocated in label.h */
    for(auto it = Label::ptr_label_->begin(); it != Label::ptr_label_->end(); ++it){
        delete (*it);
    }
    delete Label::ptr_label_;
    delete Label::ptr_labels_map_;
    delete Label::ptr_id2label_map_;
}

int main(int argc, char **argv) {
    std::vector<std::string> label_vec;
    std::string file_train = "data/ptb-binary.train.sample";
    std::string file_test = "data/ptb-binary.test.sample";
    int max_iterations = 200;
    long pre_memory_size = CommonTool::PrintMemoryInfo("at the beginning of program");
    /* read PTB data */
    std::unordered_map<std::string, int> *ptr_word2int_map = new std::unordered_map<std::string, int>;;
    std::pair<InstenceVector *, InstenceVector *> train_data = PTBReader::ReadPTB(file_train,ptr_word2int_map,true);
    std::vector<Instance *> *ptr_inst_vec = train_data.first;
    std::vector<Instance *> *ptr_inst_vec_dup = train_data.second;
    std::cout << "training size is "<<ptr_inst_vec->size()<<std::endl;
    std::pair<InstenceVector *, InstenceVector *> test_data = PTBReader::ReadPTB(file_test,ptr_word2int_map,false);
    std::vector<Instance *> *ptr_inst_vec_test = test_data.first;
    /* build rule based on training and test data */
    CFGRule::BuildCFGRules(ptr_inst_vec);
    CFGRule::BuildCFGRules(ptr_inst_vec_test);
    /* Init parameter body */
    GlobalNetworkParam *ptr_g_param = new GlobalNetworkParam(argc,argv,PTBReader::max_len_,ptr_inst_vec->size(),&label_vec);
    TreeCRFFeatureManager *ptr_fm = new TreeCRFFeatureManager(ptr_inst_vec,ptr_g_param);
    TreeCRFNetworkCompiler *ptr_nc = new TreeCRFNetworkCompiler(Label::ptr_label_);
    DiscriminativeNetworkModel *ptr_model = new DiscriminativeNetworkModel(ptr_fm,ptr_nc);
    ptr_model->SetPreMemorySize(pre_memory_size);
    /* Train and Decode */
    ptr_model->Train(ptr_inst_vec,ptr_inst_vec_dup,max_iterations);
    ptr_model->Decode(ptr_inst_vec_test, false);
    Evaluate::EvaluateResult(ptr_inst_vec_test);
    /* release memory */
    ReleaseStaticMemory();
    {
        /* release global memory */
        /* release training sentences */
        for(auto it = ptr_inst_vec->begin(); it != ptr_inst_vec->end(); ++it){
            TreeCRFInstance *ptr_tree_crf_inst = (TreeCRFInstance *)(*it);
            delete ptr_tree_crf_inst;
        }
        delete ptr_inst_vec;
        /*release training duplicated sentences*/
        for(auto it = ptr_inst_vec_dup->begin(); it != ptr_inst_vec_dup->end(); ++it){
            TreeCRFInstance *ptr_tree_crf_inst = (TreeCRFInstance *)(*it);
            ptr_tree_crf_inst->SetAllPointerNull();
            delete ptr_tree_crf_inst;
        }
        /*release user-defined instances*/
        delete ptr_inst_vec_dup;
        delete ptr_word2int_map;
        delete ptr_g_param;
        delete ptr_fm;
        delete ptr_nc;
        delete ptr_model;
    }
    return 0;
}

