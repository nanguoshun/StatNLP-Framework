//
// Created by  ngs on 02/08/2018.
//

#include "linear_crf_feature_manager.h"

LinearCRFFeatureManager::LinearCRFFeatureManager(GlobalNetworkParam *ptr_param):FeatureManager(ptr_param){

}

LinearCRFFeatureManager::~LinearCRFFeatureManager() {

}

// this function is called LocalNetworkLearnerThread during touch phase.
FeatureArray* LinearCRFFeatureManager::ExtractHelper(Network *ptr, int parent, std::vector<int> *ptr_children) {
    //to be done..
    //
}