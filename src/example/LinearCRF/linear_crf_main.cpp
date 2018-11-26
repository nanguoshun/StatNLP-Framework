#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <src/example/LinearCRF/linear_crf_lstm.h>
#include <src/neural/neural_factory.h>
#include "src/hybridnetworks/data_manager.h"
#include "src/example/LinearCRF/linear_crf_instance.h"
#include "src/example/LinearCRF/linear_crf_feature_manager.h"
#include "src/hybridnetworks/network_model.h"
#include "src/hybridnetworks/discriminative_network_model.h"
#include "src/example/LinearCRF/linear_crf_nework_compiler.h"
#include "src/neural/neural_network.h"
#include "dynet/dict.h"
//#include "src/common/common.h"
#include "data_reader.h"

void ReleaseStaticPointer(){
    delete FeatureArray::PTR_EMPTY;
//    delete Network::ptr_inside_shared_array_;
//    delete Network::ptr_outside_shared_array_;
}

int main(int argc, char **argv){
    dynet::Dict *ptr_dict = new dynet::Dict();
    std::string train_file_name = "data/conll2000/sample_train_1.txt";
    std::string test_file_name =  "data/conll2000/sample_test_1.txt";
    std::vector<Instance*> *ptr_inst_vec_all = new std::vector<Instance *>;
    std::vector<Instance*> *ptr_inst_vec_all_duplicate_ = new std::vector<Instance *>;
    std::vector<Instance*> *ptr_inst_vec_all_test = new std::vector<Instance*>;
    std::unordered_map<std::string,int> *ptr_word2int_map = new std::unordered_map<std::string,int>;
    DataReader::ReadData(train_file_name,ptr_inst_vec_all,ptr_inst_vec_all_duplicate_,false,true,true,ptr_word2int_map);
    DataReader::ReadData(test_file_name,ptr_inst_vec_all_test, nullptr, true, true, false,ptr_word2int_map);
    int num_iterations = 1500;
    GlobalNetworkParam *ptr_param_g = nullptr;
    if(ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type){
        ptr_param_g = new GlobalNetworkParam(argc,argv,DataReader::max_len_,ptr_inst_vec_all->size(),DataReader::ptr_all_labels_);
    } else if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){
        ptr_param_g = new GlobalNetworkParam(argc,argv,DataReader::max_len_,ptr_inst_vec_all->size(),DataReader::ptr_all_labels_,ComType::NeuralType::LSTM,ptr_word2int_map,ptr_dict);
//        ptr_param_g = new GlobalNetworkParam(argc,argv,max_len,ptr_inst_vec_all->size(),&all_labels, (NeuralFactory*)NeuralFactory::GetCNNFactory(),ptr_word2int_map,ptr_dict);
    }
    //Below is the only hand-crafted features, and there are no parameters in the constructor of GlobalNetworkParam
    // GlobalNetworkParam *ptr_param_g = new GlobalNetworkParam();
    LinearCRFFeatureManager *ptr_fm = new LinearCRFFeatureManager(ptr_param_g, ptr_inst_vec_all);
    LinearCRFNetworkCompiler *ptr_nc = new LinearCRFNetworkCompiler(*(DataReader::ptr_all_labels_));
    NetworkModel *ptr_nm = new DiscriminativeNetworkModel(ptr_fm,ptr_nc);
    ptr_nm->Train(ptr_inst_vec_all, ptr_inst_vec_all_duplicate_,num_iterations);
    std::vector<Instance *> *ptr_predictions = ptr_nm->Decode(ptr_inst_vec_all_test,false);
    int corr =  0;
    int total = 0;
    int count = 0;
    auto itt_gloden = ptr_inst_vec_all_test->begin();
    int index = 0;
    for(auto it_pre = ptr_predictions->begin(); it_pre != ptr_predictions->end(); ++it_pre, ++itt_gloden){
        index++;
        LinearCRFInstance *ptr_predict = (LinearCRFInstance *)(*it_pre);
        LinearCRFInstance *ptr_golden = (LinearCRFInstance *)(*it_pre);
        std::vector<std::string> *ptr_predict_vec = ptr_predict->GetPrediction();
        std::vector<std::string> *ptr_golden_vec = ptr_golden->GetOutPut();
        int input_size = ptr_golden->GetInput()->size();
        int golden_size = ptr_golden_vec->size();
        int inst_size = ptr_predict_vec->size();
        for(int i = 0; i < inst_size; ++i){
            std::string predict_str = (*ptr_predict_vec)[i];
            std::string golden_str = (*ptr_golden_vec)[i];
            if(predict_str == golden_str){
                ++corr;
            }
            ++total;
        }
    }
    double accuracy = double(corr) / (double)(total);
    std::cout << "Correct and Total are: "<<corr<<","<<total<<std::endl;
    std::cout <<"Accuracy is: "<< accuracy <<std::endl;
    //release the memory
    delete ptr_param_g;
    delete ptr_fm;
    delete ptr_nc;
    delete ptr_nm;
    delete ptr_word2int_map;
    ReleaseStaticPointer();
}

