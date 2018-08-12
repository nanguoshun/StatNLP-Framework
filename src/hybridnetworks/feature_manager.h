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
    FeatureArray * Extract(Network *ptr_network, int parent_k, int * ptr_children_k, int children_k_index);
    bool isCacheAble();
    void EnableCache(int numNetworks);
    virtual FeatureArray* ExtractHelper(Network *ptr_network, int parent, int *ptr_children) = 0;
protected:
    GlobalNetworkParam *ptr_param_g_;
    std::vector<LocalNetworkParam *> *ptr_param_l_;
    int num_of_threads_;
    bool cache_enabled_;
    FeatureArray ****ptr_cache_;
    int word_hal_window_size_;
    int pos_hal_window_size_;
    int num_of_networks_;
};

#endif //STATNLP_FEATUREMANAGER_H
