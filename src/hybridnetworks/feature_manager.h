//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_FEATUREMANAGER_H
#define STATNLP_FEATUREMANAGER_H

#include "global_network_param.h"
#include "feature_array.h"
#include <vector>
//#include "local_network_param.h"

class LocalNetworkParam;
class Network;
class FeatureManager{
public:
    FeatureManager();
    FeatureManager(GlobalNetworkParam* ptr_param);
    ~FeatureManager();
    GlobalNetworkParam * GetGlobalParam();
    bool Update();
    virtual FeatureArray* ExtractHelper(Network *ptr, int parent, std::vector<int> *ptr_children) = 0;
protected:
    GlobalNetworkParam *ptr_param_g_;
    std::vector<LocalNetworkParam *> *ptr_param_l_;
    int num_of_threads_;
};

#endif //STATNLP_FEATUREMANAGER_H
