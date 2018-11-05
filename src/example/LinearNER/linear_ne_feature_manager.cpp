//
// Created by  ngs on 05/10/2018.
//

#include "linear_ne_feature_manager.h"

std::string LinearNEFeatureManager::feature_type_[5] = {"WORD","WORD_BIGRAM","TAG","TAG_BIGRAM","TRANSITION"};

LinearNEFeatureManager::LinearNEFeatureManager() {

}

LinearNEFeatureManager::~LinearNEFeatureManager() {

}

LinearNEFeatureManager::LinearNEFeatureManager(std::vector<Instance*> *ptr_inst, GlobalNetworkParam *ptr_param):FeatureManager(ptr_param){
    ptr_inst_vector_ = ptr_inst;
    ptr_feature_type_ = new ComType::FeatureT[NUM_FEATURE_TYPE];
    //init the feature type and its ID.
    for(int i = 0; i < NUM_FEATURE_TYPE; ++i){
        if(i == 0 || i == 4){
            ptr_feature_type_[i].isOn = true;
        } else{
            ptr_feature_type_[i].isOn = false;
        }
        ptr_feature_type_[i].type = feature_type_[i];
    }
    tmp_count_t_ = 0;
    tmp_count_e_ = 0;
}

FeatureArray* LinearNEFeatureManager::ExtractHelper(Network *ptr_network, int parent, int *ptr_children,
                                                    int children_k_index) {
    LinearNENetwork *ptr_ne_network = (LinearNENetwork *)ptr_network;
    LinearNEInstance *ptr_inst = (LinearNEInstance*)ptr_ne_network->GetInstance();
    Sentence *ptr_sent = ptr_inst->GetInput();
    int inst_size = ptr_inst->GetSize();
    long node_id = ptr_ne_network->GetNode(parent);
    std::vector<int> node_vec = NetworkIDManager::ToHybridNodeArray(node_id);
    int pos = node_vec[0];
    int tag_Id = node_vec[1];
    int node_type =node_vec[2];
    if(ComType::NODE_TYPES::LEAF == node_type) return FeatureArray::PTR_EMPTY;
    if(ComType::NODE_TYPES::ROOT == node_type && pos != inst_size - 1) return FeatureArray::PTR_EMPTY;
    FeatureArray *ptr_features = new FeatureArray((int*) nullptr,0);
    std::vector<int> child_vev = NetworkIDManager::ToHybridNodeArray(ptr_ne_network->GetNode(ptr_children[0]));
    /* transition */
    std::string entity_str = std::to_string(tag_Id);
    /* index 1 is tag */
    std::string prev_entity_str = std::to_string(child_vev[1]);// + " "+std::to_string(tag_Id);
    std::string type = ptr_feature_type_[4].type + ":" + std::to_string(0);
    int featureID =  this->ptr_param_g_->ToFeature(type,entity_str,prev_entity_str);
    FeatureArray *ptr_word_features = new FeatureArray(new int[1]{featureID}, 1, ptr_features);
    tmp_count_t_++;
    ptr_features = ptr_word_features;
    /* the root node has no emission feature */
    if(node_type == ComType::NODE_TYPES::ROOT){
        return ptr_features;
    }

    /* neural or emission */
    if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){ /* neural */
        if(ComType::NeuralType::LSTM == NetworkConfig::Neural_Type){
            /* where the ptr_nn_input_pair released?? */
            ComType::Neural_Input *ptr_nn_input_pair = new ComType::Neural_Input;
            ComType::Input_Str_Vector *ptr_input_vec = ptr_sent->GetSentence();
            ptr_nn_input_pair->first = ptr_input_vec;
            ptr_nn_input_pair->second = pos;
            /* noted that the output is tag_id - 1, why tag_Id - 1 ????? */
            AddNeural(ptr_network,0,parent,children_k_index,ptr_nn_input_pair,tag_Id - 1);
        }
    } else if(ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type){ /* emission */
        std::string word = ptr_inst->GetInput()->Get(pos)->GetForm();
        std::string type = ptr_feature_type_[0].type + ":" + std::to_string(0);
        int featureID =  this->ptr_param_g_->ToFeature(type,entity_str,word);
        ptr_features = new FeatureArray(new int[1]{featureID}, 1, ptr_features);
        tmp_count_e_ ++;
    } else if(ComParam::USE_PURE_NEURAL_FEATURES){

    }
    return ptr_features;
}