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
static std::vector<std::string> all_labels; /*all unique labels*/
static int max_len = 0;
/**
 *
 * @param file_name
 * @param ptr_inst_vec_all : the vector of all instances.
 * @param withLabels
 * @param isLabeled
 *
 */
void ReadData(std::string file_name, std::vector<Instance*> *ptr_inst_vec_all, std::vector<Instance*> *ptr_inst_vec_all_duplicate, bool istest, bool withLabels, bool isLabeled, std::unordered_map<std::string,int> *ptr_word2int,dynet::Dict *ptr_dict) {
    std::ifstream ifs(file_name);
    std::string str;
    std::vector<std::string> *ptr_labels = nullptr;
    std::vector<std::string> *ptr_labels_du = nullptr;
    int instance_id = 0;
    bool is_allocate_vector = true;
    ComType::Input_Str_Matrix *ptr_vec_matrix;
    ComType::Input_Str_Matrix *ptr_list_vect_du;
    while (std::getline(ifs, str)) {
        //allocate the space for each instance at the beginning.
        if (is_allocate_vector) {
            ptr_vec_matrix = new ComType::Input_Str_Matrix;
            ptr_labels = new std::vector<std::string>;
            //ptr_list_vect_du = new ComType::Input_Str_Matrix;
            ptr_labels_du = new std::vector<std::string>;
            is_allocate_vector = false;
        }
        if (0 == str.length()) {
            if (ptr_vec_matrix->size() > max_len) {
                max_len = ptr_vec_matrix->size();
            }
            LinearCRFInstance *ptr_crf_inst = new LinearCRFInstance(instance_id, 1.0, ptr_vec_matrix, ptr_labels);
            ptr_crf_inst->ExtractStrVect();
            LinearCRFInstance *ptr_crf_inst_du = nullptr;
            if (!istest) {
//                ptr_crf_inst_du = new LinearCRFInstance(-instance_id, -1.0, ptr_list_vect_du, ptr_labels_du);
                ptr_crf_inst_du = new LinearCRFInstance(-instance_id, -1.0, ptr_vec_matrix, ptr_labels_du);
                ptr_crf_inst_du->SetStrVect(ptr_crf_inst->GetStrVect());
            }
            if (isLabeled) {
                ptr_crf_inst->SetLabeled();
            } else {
                ptr_crf_inst->SetUnlabeled();
            }
            instance_id++;
            ptr_inst_vec_all->push_back(ptr_crf_inst);
            if (!istest) {
                ptr_inst_vec_all_duplicate->push_back(ptr_crf_inst_du);
            }
            is_allocate_vector = true;
#ifdef DEBUG
            if(!istest){
                std::cout <<"The end of "<<instance_id<<" th training instance" <<std::endl;
            } else{
                std::cout <<"The end of "<<instance_id<<" th test instance" <<std::endl;
            }
            std::cout << std::endl;
#endif
        } else {
            std::stringstream ss(str);
            std::string feature1, feature2;
            std::vector<std::string> words_vec;
            std::vector<std::string> words_vec_du;
            ss >> feature1;
            ss >> feature2;
            words_vec.push_back(feature1);
            words_vec.push_back(feature2);
            ptr_dict->convert(feature1);
            if (!istest) {
                //words_vec_du.push_back(feature1);
                //words_vec_du.push_back(feature2);
            }
            ptr_vec_matrix->push_back(words_vec);
            if (!istest) {
                //ptr_list_vect_du->push_back(words_vec_du);
            }
            //std::cout << feature1 <<" "<< feature2<<" ";
            if (withLabels) {
                std::string label;
                ss >> label;
                auto it = std::find(all_labels.begin(), all_labels.end(), label);
                if (it == all_labels.end()) {
                    all_labels.push_back(label);
                    ptr_dict->convert(label);
                }
                ptr_labels->push_back(label);
                if (!istest) {
                    ptr_labels_du = nullptr;
                }
                //std::cout<<label<<std::endl;
            }
            if (!istest) {
                auto it = ptr_word2int->find(feature1);
                if (it == ptr_word2int->end()) {
                    ptr_word2int->insert(std::make_pair(feature1, ptr_word2int->size()));
                }
            }
        }
    }
    //duplicate the data
    if (!istest) {
        auto it = ptr_word2int->find(ComParam::UNK);
        if(it == ptr_word2int->end()){
            ptr_word2int->insert(std::make_pair(ComParam::UNK, ptr_word2int->size()));
        }
    }
}
void Release(std::vector<Instance*> *ptr_vec_all){
    for(auto it = ptr_vec_all->begin(); it!=ptr_vec_all->end(); ++it){
        delete (*it);
    }
    delete ptr_vec_all;
}

void ReleaseStaticPointer(){
    delete FeatureArray::PTR_EMPTY;
//    delete Network::ptr_inside_shared_array_;
//    delete Network::ptr_outside_shared_array_;
}

int main(int argc, char **argv){
    dynet::Dict *ptr_dict = new dynet::Dict();
    //std::string train_file_name = "data/conll2000/sample_train_2.txt";
    std::string train_file_name = "data/conll2000/sample_train.txt";
    //std::string test_file_name =  "/Users/ngs/Documents/cplusproject/statNLP/data/conll2000/sample_part_test.txt";
    //std::string train_file_name = "/Users/ngs/Documents/cplusproject/statNLP/data/conll2000/sample_train.txt";
    //std::string train_file_name = "/Users/ngs/Documents/cplusproject/statNLP/data/conll2000/train.txt";
    std::string test_file_name =  "data/conll2000/sample_test.txt";
    std::vector<Instance*> *ptr_inst_vec_all = new std::vector<Instance *>;
    std::vector<Instance*> *ptr_inst_vec_all_duplicate_ = new std::vector<Instance *>;
    std::vector<Instance*> *ptr_inst_vec_all_test = new std::vector<Instance*>;
    std::unordered_map<std::string,int> *ptr_word2int_map = new std::unordered_map<std::string,int>;

    ReadData(train_file_name,ptr_inst_vec_all,ptr_inst_vec_all_duplicate_,false,true,true,ptr_word2int_map,ptr_dict);
    int size = ptr_inst_vec_all->size();
    ReadData(test_file_name,ptr_inst_vec_all_test, nullptr, true, true, false,ptr_word2int_map,ptr_dict);
    int num_iterations = 200;
#ifdef DEBUG
    int size_train = ptr_inst_vec_all->size();
    int size_train_du = ptr_inst_vec_all_duplicate_->size();
    int size_test = ptr_inst_vec_all_test->size();
#endif
    Feature_TEST = 2;
    std::cout << "feature test is "<<Feature_TEST <<std::endl;
    //NetworkConfig::Feature_Type = ComParam::USE_HYBRID_NEURAL_FEATURES;
    //std::cout << "feature type is "<<NetworkConfig::Feature_Type<<std::endl;
//    GlobalNetworkParam *ptr_param_g = new GlobalNetworkParam(argc,argv,ptr_inst_vec_all->size(),max_len);
    GlobalNetworkParam *ptr_param_g = nullptr;
    if(ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type){
        ptr_param_g = new GlobalNetworkParam(argc,argv,max_len,ptr_inst_vec_all->size(),&all_labels);

    } else if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){
        ptr_param_g = new GlobalNetworkParam(argc,argv,max_len,ptr_inst_vec_all->size(),&all_labels, (NeuralFactory*)NeuralFactory::GetLSTMFactory(),ptr_word2int_map,ptr_dict);
    }
    //std::cout << "feature type is "<<NetworkConfig::Feature_Type<<std::endl;

    //Below is the only hand-crafted features, and there are no parameters in the constructor of GlobalNetworkParam
    // GlobalNetworkParam *ptr_param_g = new GlobalNetworkParam();
    LinearCRFFeatureManager *ptr_fm = new LinearCRFFeatureManager(ptr_param_g, ptr_inst_vec_all);
    LinearCRFNetworkCompiler *ptr_nc = new LinearCRFNetworkCompiler(all_labels);
    NetworkModel *ptr_nm = new DiscriminativeNetworkModel(ptr_fm,ptr_nc);
    ptr_nm->Train(ptr_inst_vec_all, ptr_inst_vec_all_duplicate_,num_iterations);
    std::vector<Instance *> *ptr_predictions = ptr_nm->Decode(ptr_inst_vec_all_test,false);
#ifdef DEBUG
    std::cout << "size of predict instances is: "<<ptr_predictions->size()<<std::endl;
    std::cout << "size of godlen instances is: "<<ptr_inst_vec_all_test->size()<<std::endl;
#endif
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
    //BaseInstance<int, int, int> ins(a,b,c,d,e);//*ptr = new LinearCRFInstance(a,b,c);
    Release(ptr_inst_vec_all);
    Release(ptr_inst_vec_all_duplicate_);
    Release(ptr_inst_vec_all_test);
    ReleaseStaticPointer();
}

