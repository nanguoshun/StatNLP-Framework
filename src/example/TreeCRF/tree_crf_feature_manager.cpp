//
// Created by  ngs on 18/10/2018.
//

#include "tree_crf_feature_manager.h"

TreeCRFFeatureManager::TreeCRFFeatureManager() {

}

TreeCRFFeatureManager::TreeCRFFeatureManager(std::vector<Instance *> *ptr_inst, GlobalNetworkParam *ptr_param):FeatureManager(ptr_param) {
    ptr_inst_vector_ = ptr_inst;
}

TreeCRFFeatureManager::~TreeCRFFeatureManager() {

}

FeatureArray* TreeCRFFeatureManager::ExtractHelper(Network *ptr_network, int parent, int *ptr_children, int children_k_index) {
    TreeCRFNetwork *ptr_tree_crf_network = (TreeCRFNetwork*) ptr_network;
}