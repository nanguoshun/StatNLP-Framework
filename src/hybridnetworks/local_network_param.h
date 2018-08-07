//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_LOCAL_NETWORK_PARAM_H
#define STATNLP_LOCAL_NETWORK_PARAM_H

#include "feature_manager.h"
#include <vector>
#include <unordered_map>
#include "feature_array.h"

class Network;

class LocalNetworkParam{
public:
    LocalNetworkParam();
    LocalNetworkParam(int threadId, FeatureManager *ptr_fm, int numNetworks);
    ~LocalNetworkParam();
    void DisableCache();
    int GetThreadId();
    double GetWeight(int featureId);
    bool IsGlobalMode();
    FeatureArray *Extract(Network *ptr_network, int parent_k, std::vector<int> *ptr_children_k, int children_k_index);
    void FinalizeIt();
    bool isCacheAble();
protected:
    int thread_id_;
    FeatureManager *ptr_fm_;
    double obj_;
    std::vector<int> *ptr_fs_;
    std::vector<int> *ptr_counts_;
    std::unordered_map<int ,int> *ptr_globalFeature2LocalFeature_;
    bool isFinalized_;
    //networkId, parent Id, Child ID
    std::vector<std::vector<std::vector<FeatureArray *>>> *ptr_cache_;
    bool is_cache_enabled_;
    int num_networks_;
    bool is_gobal_mode_;
};

#endif //STATNLP_LOCAL_NETWORK_PARAM_H
