//
// Created by  ngs on 31/07/2018.
//

#include "feature_manager.h"
#include "network_config.h"

FeatureManager::FeatureManager(GlobalNetworkParam param) {
    this->param_g_ = param;
    this->num_of_threads_ = NetworkConfig::NUM_OF_THREADS;
    this->ptr_param_l_= new LocalNetworkParam[this->num_of_threads_];

}

FeatureManager::~FeatureManager() {
    delete [] this->ptr_param_l_;
}
