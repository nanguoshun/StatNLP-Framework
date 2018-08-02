//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_FEATUREMANAGER_H
#define STATNLP_FEATUREMANAGER_H

#include "global_network_param.h"
#include <vector>
//#include "local_network_param.h"

class LocalNetworkParam;

class FeatureManager{
public:
    FeatureManager();
    FeatureManager(GlobalNetworkParam* ptr_param);
    ~FeatureManager();

protected:
    GlobalNetworkParam *ptr_param_g_;
    std::vector<LocalNetworkParam *> *ptr_param_l_;
    int num_of_threads_;
};

#endif //STATNLP_FEATUREMANAGER_H
