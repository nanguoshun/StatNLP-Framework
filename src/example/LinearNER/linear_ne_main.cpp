//
// Created by  ngs on 05/10/2018.
//
#include <iostream>
#include <vector>
#include <src/hybridnetworks/global_network_param.h>
#include <src/hybridnetworks/discriminative_network_model.h>
#include "linear_ne_feature_manager.h"
#include "linear_ne_network_compiler.h"
#include "entity_reader.h"


int main(int argc, char **argv) {
    std::string file_name_train = "data/conll2003/en.train-1";
    std::string file_name_dev = "data/conll2003/en.testa-1";
    std::string file_name_test = "data/conll2003/en.testb-1";
    std::vector<Instance*> *ptr_inst_vec_train = new std::vector<Instance *>;
    std::vector<Instance*> *ptr_inst_vec_train_dup = new std::vector<Instance *>;
    std::vector<Instance*> *ptr_inst_vec_test = new std::vector<Instance*>;
    std::unordered_map<std::string,int> *ptr_word2int_map = new std::unordered_map<std::string,int>;
    EntityReader::ReadData(file_name_train,ptr_inst_vec_train,ptr_inst_vec_train_dup,false,true,true,ptr_word2int_map);
    int size = ptr_inst_vec_train->size();
    std::cout << "the vec size is"<<size<<std::endl;
    EntityReader::ReadData(file_name_test,ptr_inst_vec_test, nullptr, true, true, false,ptr_word2int_map);
    int num_iterations = 200;
    GlobalNetworkParam *ptr_g_param = nullptr;
    if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){
        ptr_g_param = new GlobalNetworkParam(argc,argv,EntityReader::max_len,ptr_inst_vec_train->size(),&EntityReader::all_labels,(NeuralFactory*)NeuralFactory::GetLSTMFactory(),ptr_word2int_map);
    } else if(ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type){
        ptr_g_param = new GlobalNetworkParam(argc,argv,EntityReader::max_len,ptr_inst_vec_train->size(),&EntityReader::all_labels);
    }
    LinearNEFeatureManager *ptr_fm = new LinearNEFeatureManager(ptr_inst_vec_train,ptr_g_param);
    LinearNENetworkCompiler *ptr_nc = new LinearNENetworkCompiler(EntityReader::all_labels);
    NetworkModel *ptr_nm = new DiscriminativeNetworkModel(ptr_fm,ptr_nc);
    ptr_nm->Train(ptr_inst_vec_train, ptr_inst_vec_train_dup,num_iterations);
    std::vector<Instance *> *ptr_predictions = ptr_nm->Decode(ptr_inst_vec_test,false);
    return 0;
}
