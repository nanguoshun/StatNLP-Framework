//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_FEATUREMANAGER_H
#define STATNLP_FEATUREMANAGER_H

#include "global_network_param.h"
#include "local_network_param.h"

class FeatureManager{
public:
    FeatureManager(GlobalNetworkParam param);
    ~FeatureManager();

protected:
    GlobalNetworkParam param_g_;
    LocalNetworkParam *ptr_param_l_;
    int num_of_threads_;
};

#endif //STATNLP_FEATUREMANAGER_H
