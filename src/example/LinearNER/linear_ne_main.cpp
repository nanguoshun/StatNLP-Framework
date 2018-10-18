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
#include "evaluate_ner.h"
int main(int argc, char **argv) {
    std::string file_name_train = "data/conll2003/eng.train-1";
    std::string file_name_dev = "data/conll2003/en.testa-1";
    std::string file_name_test = "data/conll2003/eng.testb-1";
    int train_num = -100;
    int test_num = -100;

//    EntityReader::ReadData();

    std::unordered_map<std::string, int> *ptr_word2int_map = new std::unordered_map<std::string, int>;
    std::pair<InstenceVector *, InstenceVector *> training_data_pair = EntityReader::ReadData(file_name_train,
                                                                                              ptr_word2int_map, "IOBES",
                                                                                              train_num, true);
    std::vector<Instance *> *ptr_inst_vec_train = training_data_pair.first;
    std::vector<Instance *> *ptr_inst_vec_train_dup = training_data_pair.second;
    Entity::GenerateEntityVector();
    int size = ptr_inst_vec_train->size();
    std::cout << "the vec size is " << size << std::endl;
    std::cout << "the size of dup vec is "<<ptr_inst_vec_train_dup->size()<<std::endl;
    std::pair<InstenceVector *, InstenceVector *> test_data_pair = EntityReader::ReadData(file_name_test,
                                                                                          ptr_word2int_map, "IOB", test_num,
                                                                                          false,false);
    std::vector<Instance *> *ptr_inst_vec_test = test_data_pair.first;
    size = ptr_inst_vec_test->size();
    int num_iterations = 200;
    GlobalNetworkParam *ptr_g_param = nullptr;
    if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        ptr_g_param = new GlobalNetworkParam(argc, argv, EntityReader::max_len_, ptr_inst_vec_train->size(),
                                             &EntityReader::all_labels,
                                             (NeuralFactory *) NeuralFactory::GetLSTMFactory(), ptr_word2int_map);
    } else if (ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type) {
        ptr_g_param = new GlobalNetworkParam(argc, argv, EntityReader::max_len_, ptr_inst_vec_train->size(),
                                             &EntityReader::all_labels);
    }
    LinearNEFeatureManager *ptr_fm = new LinearNEFeatureManager(ptr_inst_vec_train, ptr_g_param);
    LinearNENetworkCompiler *ptr_nc = new LinearNENetworkCompiler(true, Entity::ptr_entity_);
    NetworkModel *ptr_nm = new DiscriminativeNetworkModel(ptr_fm, ptr_nc);
    ptr_nm->Train(ptr_inst_vec_train, ptr_inst_vec_train_dup, num_iterations);
    std::vector<Instance *> *ptr_predictions = ptr_nm->Decode(ptr_inst_vec_test, false);
    EvaluateNER::Evaluate(ptr_predictions,"data/conll2003/output/neroutput");
    //release memory allocated in main function.
    if(nullptr != ptr_word2int_map){
        delete ptr_word2int_map;
    }
    if(nullptr != ptr_inst_vec_train){
        for(auto it = ptr_inst_vec_train->begin(); it != ptr_inst_vec_train->end(); ++it){if(nullptr != (*it)) delete (*it);}
        delete ptr_inst_vec_test;
    }
    if(nullptr != ptr_inst_vec_train_dup){
        for(auto it = ptr_inst_vec_train_dup->begin(); it != ptr_inst_vec_train_dup->end(); ++it){if(nullptr != (*it)) delete (*it);}
        delete ptr_inst_vec_train_dup;
    }
    /*
    //error for release ptr_inst_vec_test
    if(nullptr != ptr_inst_vec_test){
        for(auto it = ptr_inst_vec_test->begin(); it != ptr_inst_vec_test->end(); ++it){if(nullptr != (*it)) delete (*it);}
        delete ptr_inst_vec_test;
    }*/
    return 0;
}


