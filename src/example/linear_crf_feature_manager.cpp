//
// Created by  ngs on 02/08/2018.
//

#include "linear_crf_feature_manager.h"
#include "linear_crf_network.h"
#include "../hybridnetworks/network_id_manager.h"

int LinearCRFFeatureManager::word_hal_window_size_ = 1;

LinearCRFFeatureManager::LinearCRFFeatureManager(GlobalNetworkParam *ptr_param, std::vector<Instance*> *ptr_inst):FeatureManager(ptr_param){
    ptr_inst_vector_ = ptr_inst;
}

LinearCRFFeatureManager::~LinearCRFFeatureManager() {

}

// this function is called LocalNetworkLearnerThread during touch phase.
FeatureArray* LinearCRFFeatureManager::ExtractHelper(Network *ptr_network, int parent_k, int *ptr_children) {
    LinearCRFNetwork *ptr_crf_network = (LinearCRFNetwork*) ptr_network;
    //FIXME: should get the instance from (LinearCRFInstance*)ptr_crf_network->GetInstance(), but encounters errors. Alternatively, we use ptr_inst_vector_ to get instance directly.
    //LinearCRFInstance *ptr_instance = (LinearCRFInstance*)ptr_crf_network->GetInstance();
    Instance *ptr_instance = (*ptr_inst_vector_)[ptr_crf_network->GetNetworkID()];
    LinearCRFInstance *ptr_crf_instance = (LinearCRFInstance*)ptr_crf_instance;
    int size = ptr_crf_instance->Size();
    //FIXME:
    Input_Str_List_Vector *ptr_input;//ptr_instance->GetInput();

    int curr_node = ptr_crf_network->GetNode(parent_k);
    std::vector<int> array = NetworkIDManager::ToHybridNodeArray(curr_node);
    int pos = array[0] - 1;
    int tag_id = array[1];
    int node_type = array[4];
    if(node_type == NODE_TYPES::LEAF){
        return FeatureArray::PTR_EMPTY;
    }
    int child_tag_id = (*ptr_network->GetNodeArray(ptr_children[0]))[1];

    FeatureArray *ptr_features = new FeatureArray((int*)NULL,0);

    if(true == FeatureType::WORD){
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
                //TODO:
//                word = ptr_crf_instance
            }
            if(idx > pos){
                continue;
            }
            ptr_word_window_features[i] =  this->ptr_param_g_->ToFeature(FeatureType::WORD+":"+relIdx,tag_id+"",word);
        }
        FeatureArray *ptr_word_features = new FeatureArray(ptr_word_window_features, word_window_size,ptr_features);
    }
    if(true == FeatureType::TAG){

    }
    if(true == FeatureType::WORD_BIGRAM){

    }
    if(true == FeatureType::TAG_BIGRAM){

    }
    if(true == FeatureType::TRANSITION){
        int transition_feature = this->ptr_param_g_->ToFeature(FeatureType::TRANSITION+"", tag_id+" ",child_tag_id+""+tag_id);
        ptr_features = new FeatureArray(new int[1]{transition_feature},1,ptr_features);
    }
    return ptr_features;
}