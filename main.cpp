#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "src/hybridnetworks/network_config.h"
#include "src/hybridnetworks/data_manager.h"
#include "src/example/linear_crf_instance.h"
#include "src/example/linear_crf_feature_manager.h"
#include "src/hybridnetworks/network_model.h"
#include "src/hybridnetworks/discriminative_network_model.h"
#include "src/example/linear_crf_nework_compiler.h"

double NetworkConfig::L2_REGULARIZATION_CONSTANT = 0.01;
int NetworkConfig::NUM_OF_THREADS = 1;
bool NetworkConfig::TRAIN_MODE_IS_GENERATIVE = false;
static std::list<std::string> all_labels;

/**
 *
 * @param file_name
 * @param ptr_inst_vec_all : the vector of all instances.
 * @param withLabels
 * @param isLabeled
 */
void ReadData(std::string file_name, std::vector<Instance*> *ptr_inst_vec_all, bool withLabels, bool isLabeled){
    std::ifstream ifs(file_name);
    std::string str;

    std::vector<std::string> *ptr_words = NULL;
    std::vector<std::string> *ptr_labels = NULL;
    int instance_id = 0;
    bool is_allocate_vector = true;
    Input_Str_Matrix *ptr_list_vect;
    while (std::getline(ifs,str)){
        //allocate the space for each instance at the beginning.
        if(is_allocate_vector){
            ptr_list_vect = new Input_Str_Matrix;
            ptr_words = new std::vector<std::string>;
            ptr_labels = new std::vector<std::string>;
            is_allocate_vector = false;
        }
        if(str.length() == 0){
            LinearCRFInstance *ptr_crf_inst = new LinearCRFInstance(instance_id,1,ptr_list_vect,ptr_labels);
            if(isLabeled){
                ptr_crf_inst->SetLabeled();
            } else{
                ptr_crf_inst->SetUnlabeled();
            }
            instance_id++;
            ptr_inst_vec_all->push_back(ptr_crf_inst);
            is_allocate_vector = true;
            std::cout <<"The end of "<<instance_id<<" th instance" <<std::endl;
            std::cout << std::endl;
        } else{
            std::stringstream ss(str);
            std::string feature1, feature2;
            ss >> feature1;
            ss >> feature2;
            ptr_words->push_back(feature1);
            ptr_words->push_back(feature2);
            ptr_list_vect->push_back(*ptr_words);
            std::cout << feature1 <<" "<< feature2<<" ";
            if(withLabels){
                std::string label;
                ss >> label;
                auto it = std::find(all_labels.begin(),all_labels.end(),label);
                if(it == all_labels.end()){
                    all_labels.push_back(label);
                } else{
                    label = (*it);
                }
                ptr_labels->push_back(label);
                std::cout<<label<<std::endl;
            }
        }
    }
}

void Release(std::vector<Instance*> *ptr_vec_all){
    for(auto it = ptr_vec_all->begin(); it!=ptr_vec_all->end(); ++it){
        delete (*it);
    }
    delete ptr_vec_all;
    delete FeatureArray::PTR_EMPTY;
}

int main(){
    std::string train_file_name = "/Users/ngs/Documents/cplusproject/statNLP/data/conll2000/sample_part_train.txt";
    //std::string test_file_name =  "/Users/ngs/Documents/cplusproject/statNLP/data/conll2000/sample_test.txt";
    std::vector<Instance*> *ptr_inst_vec_all = new std::vector<Instance *>;
    ReadData(train_file_name,ptr_inst_vec_all,true,true);
    int num_iterations = 100;
    int size = ptr_inst_vec_all->size();
    GlobalNetworkParam *ptr_param_g = new GlobalNetworkParam();
    LinearCRFFeatureManager *ptr_fm = new LinearCRFFeatureManager(ptr_param_g, ptr_inst_vec_all);
    LinearCRFNetworkCompiler *ptr_nc = new LinearCRFNetworkCompiler(all_labels);
    NetworkModel *ptr_nm = new DiscriminativeNetworkModel(ptr_fm,ptr_nc);
    ptr_nm->Train(ptr_inst_vec_all,num_iterations);
    std::cout << "the size is:"<<size<<std::endl;
    delete ptr_param_g;
    delete ptr_fm;
    delete ptr_nc;
    delete ptr_nm;
    //BaseInstance<int, int, int> ins(a,b,c,d,e);//*ptr = new LinearCRFInstance(a,b,c);
    Release(ptr_inst_vec_all);
}

