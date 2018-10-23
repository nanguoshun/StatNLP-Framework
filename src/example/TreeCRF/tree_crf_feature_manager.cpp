//
// Created by  ngs on 18/10/2018.
//

#include "tree_crf_feature_manager.h"

std::string TreeCRFFeatureManager::feature_type_[8] = {"LEFT_RIGHT", "FIRST_WORD", "SPLIT_WORD", "LAST_WORD",
                                                       "LAST_WORD_ENDING_1", "LAST_WORD_ENDING_2", "LAST_WORD_ENDING_3",
                                                       "FIRST_CAPITAL"};

TreeCRFFeatureManager::TreeCRFFeatureManager() {

}

TreeCRFFeatureManager::TreeCRFFeatureManager(std::vector<Instance *> *ptr_inst, GlobalNetworkParam *ptr_param)
        : FeatureManager(ptr_param) {
    ptr_inst_vector_ = ptr_inst;
    root_count_ = 0;
}

TreeCRFFeatureManager::~TreeCRFFeatureManager() {

}

FeatureArray *
TreeCRFFeatureManager::ExtractHelper(Network *ptr_network, int parent, int *ptr_children, int children_k_index) {
    TreeCRFNetwork *ptr_tree_crf_network = (TreeCRFNetwork *) ptr_network;
    TreeCRFInstance *ptr_tree_crf_inst = (TreeCRFInstance *) ptr_network->GetInstance();
    std::vector<std::string> *ptr_words_vec = ptr_tree_crf_inst->GetInput();
    long parent_id = ptr_tree_crf_network->GetNode(parent);
    std::vector<int> parent_vec = NetworkIDManager::ToHybridNodeArray(parent_id);
    int height = parent_vec[1]; /* height equals to span size */
    int index = parent_vec[0] - height;
    int label_id = parent_vec[2];
    int node_type = parent_vec[4];
    int start = index;
    int end = index + height; /* the value end - start equals to span size of a tree */
    int children_len = (ptr_network->GetChildren_Size(parent))[children_k_index];
    //test only
/*    std::cout << "the length is: "<<children_len<<std::endl;
    if( 0 == children_len){
        std::cout << "the length is zero"<<std::endl;
    }*/
    if (0 == children_len || node_type == NodeType::ROOT) {
        root_count_++;
        std::cout << root_count_<<"th root"<<std::endl;
        return FeatureArray::PTR_EMPTY;
    }
    std::string start_word = (*ptr_words_vec)[start];
    std::string end_word = (*ptr_words_vec)[end];
    int first_word_feature = ptr_param_g_->ToFeature(feature_type_[1], std::to_string(label_id), start_word);
    int last_word_feature = ptr_param_g_->ToFeature(feature_type_[3], std::to_string(label_id), end_word);
    int end_word_len = end_word.size();
    std::string last1 = end_word_len >= 1 ? end_word.substr(end_word_len - 1) : end_word;
    std::string last2 = end_word_len >= 2 ? end_word.substr(end_word_len - 2) : end_word;
    std::string last3 = end_word_len >= 3 ? end_word.substr(end_word_len - 3) : end_word;
    int last1_feature = ptr_param_g_->ToFeature(feature_type_[4], std::to_string(label_id), last1);
    int last2_feature = ptr_param_g_->ToFeature(feature_type_[5], std::to_string(label_id), last2);
    int last3_feature = ptr_param_g_->ToFeature(feature_type_[6], std::to_string(label_id), last3);
    bool is_first_capital = std::isupper(start_word[0]);
    int first_capital_feature = ptr_param_g_->ToFeature(feature_type_[7], std::to_string(label_id),
                                                        std::to_string(is_first_capital));

    /* if the children length equals to 1, then we use 6 features */
    if (1 == children_len) {
        int *ptr_f = new int[6]{first_word_feature, last_word_feature, last1_feature, last2_feature, last3_feature,
                                first_capital_feature};
        FeatureArray *ptr_fa = new FeatureArray(ptr_f, 6);
        return ptr_fa;
    }

    /* if the children length is larger than 1, then we use 8 features */
    long left_node = ptr_network->GetNode(ptr_children[0]);
    std::vector<int> left_array = NetworkIDManager::ToHybridNodeArray(left_node);
    int left_label_id = left_array[2];
    long right_node = ptr_network->GetNode(ptr_children[1]);
    std::vector<int> right_array = NetworkIDManager::ToHybridNodeArray(right_node);
    int right_height = right_array[1];
    int right_index = right_array[0] - right_height;
    int right_label_id = right_array[2];
    int right_start = right_index;
    std::string split_word = (*ptr_words_vec)[right_start];
    int split_word_feature = ptr_param_g_->ToFeature(feature_type_[2], std::to_string(label_id), split_word);
    std::string left_right_input = left_label_id + " " + right_label_id;
    int left_right_feature = ptr_param_g_->ToFeature(feature_type_[0], std::to_string(label_id), left_right_input);
    int *ptr_f = new int[8]{left_right_feature, first_word_feature, split_word_feature, last_word_feature,
                            last1_feature, last2_feature, last3_feature, first_capital_feature};
    FeatureArray *ptr_fa = new FeatureArray(ptr_f, 8);
    return ptr_fa;
}