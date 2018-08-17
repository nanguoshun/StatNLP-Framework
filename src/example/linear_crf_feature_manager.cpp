//
// Created by  ngs on 02/08/2018.
//

#include "linear_crf_feature_manager.h"
#include "linear_crf_network.h"
#include "../hybridnetworks/network_id_manager.h"

int LinearCRFFeatureManager::word_hal_window_size_ = 1;
std::string LinearCRFFeatureManager::feature_type_[5] = {"WORD","WORD_BIGRAM","TAG","TAG_BIGRAM","TRANSITION"};

LinearCRFFeatureManager::LinearCRFFeatureManager(GlobalNetworkParam *ptr_param, std::vector<Instance*> *ptr_inst):FeatureManager(ptr_param){
    ptr_inst_vector_ = ptr_inst;
    ptr_feature_type_ = new FeatureT[5];
    //init the feature type and its ID.
    for(int i = 0; i < NUM_FEATURE_TYPE; ++i){
        if(i == 0 || i == 4){
            ptr_feature_type_[i].isOn = true;
        } else{
            ptr_feature_type_[i].isOn = false;
        }
        ptr_feature_type_[i].type = feature_type_[i];
    }
}

LinearCRFFeatureManager::~LinearCRFFeatureManager() {
    delete ptr_feature_type_;
}

// this function is called LocalNetworkLearnerThread during touch phase.
FeatureArray* LinearCRFFeatureManager::ExtractHelper(Network *ptr_network, int parent_k, int *ptr_children) {
    LinearCRFNetwork *ptr_crf_network = (LinearCRFNetwork*) ptr_network;
    //FIXME: should get the instance from (LinearCRFInstance*)ptr_crf_network->GetInstance(), but encounters errors. Alternatively, we use ptr_inst_vector_ to get instance directly.
    LinearCRFInstance *ptr_instance = (LinearCRFInstance*)ptr_crf_network->GetInstance();

    //Instance *ptr_instance = (*ptr_inst_vector_)[ptr_crf_network->GetNetworkID()];
    //LinearCRFInstance *ptr_crf_instance = (LinearCRFInstance*)ptr_crf_instance;
    int size = ptr_instance->GetSize();
    //FIXME:
    Input_Str_Matrix *ptr_input = ptr_instance->GetInput();

    int input_size = ptr_input->size();

    long curr_node = ptr_crf_network->GetNode(parent_k);
    std::vector<int> array = NetworkIDManager::ToHybridNodeArray(curr_node);
    int pos = array[0] - 1;
    int tag_id = array[1];
    int node_type = array[4];
    if(node_type == NODE_TYPES::LEAF){
        return FeatureArray::PTR_EMPTY;
    }
    //only one child for linear CRF.
    int child_tag_id = (ptr_network->GetNodeArray(ptr_children[0]))[1];

    FeatureArray *ptr_features = new FeatureArray((int*) nullptr,0);

    if(true == ptr_feature_type_[0].isOn){
        int word_window_size = word_hal_window_size_*2 + 1;
        if(word_window_size < 0){
            word_window_size = 0;
        }
        int* ptr_word_window_features = new int[word_window_size];
        for(int i=0; i<word_window_size; ++i){
            std::string word = "***";
            int relIdx = i - word_hal_window_size_;
            int idx = pos + relIdx;
            if(idx >=0 && idx < size){
                word = (*ptr_input)[idx][0];
            }
            if(idx > pos){
                continue;
            }
            std::string type = ptr_feature_type_[0].type + ":" + std::to_string(relIdx);
            std::string output = std::to_string(tag_id);
            ptr_word_window_features[i] =  this->ptr_param_g_->ToFeature(type,output,word);
        }
        FeatureArray *ptr_word_features = new FeatureArray(ptr_word_window_features, word_window_size,ptr_features);
        ptr_features = ptr_word_features;
    }
    if(true == ptr_feature_type_[1].isOn){

    }
    if(true == ptr_feature_type_[2].isOn){

    }
    if(true == ptr_feature_type_[3].isOn){

    }
    if(true == ptr_feature_type_[4].isOn){
        std::string type = ptr_feature_type_[4].type;
        std::string output = std::to_string(tag_id);
        std::string input = std::to_string(child_tag_id) + " " + std::to_string(tag_id);
        int transition_feature = this->ptr_param_g_->ToFeature(type,output,input);
        ptr_features = new FeatureArray(new int[1]{transition_feature},1,ptr_features);
    }
    return ptr_features;
}