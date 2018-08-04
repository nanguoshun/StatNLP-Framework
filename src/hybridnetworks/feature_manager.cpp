//
// Created by  ngs on 31/07/2018.
//

#include "feature_manager.h"
#include "network_config.h"
#include "common.h"

FeatureManager::FeatureManager() {

}

FeatureManager::FeatureManager(GlobalNetworkParam *ptr_param) {
    this->ptr_param_g_ = ptr_param;
    this->num_of_threads_ = NetworkConfig::NUM_OF_THREADS;
    this->ptr_param_l_= new std::vector<LocalNetworkParam*>(this->num_of_threads_);
}

FeatureManager::~FeatureManager() {
    delete this->ptr_param_l_;
}

GlobalNetworkParam* FeatureManager::GetGlobalParam() {
    return ptr_param_g_;
}

bool FeatureManager::Update() {
    if(Num_Of_Threads != 1){

    }
}