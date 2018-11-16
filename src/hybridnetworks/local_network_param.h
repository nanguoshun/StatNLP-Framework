//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_LOCAL_NETWORK_PARAM_H
#define STATNLP_LOCAL_NETWORK_PARAM_H

#include "feature_manager.h"
#include <vector>
#include <unordered_map>
#include "feature_array.h"
#include <boost/any.hpp>
#include <boost/functional/hash.hpp>
#include <mutex>
#include<cmath>

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
    FeatureArray *Extract(Network *ptr_network, int parent_k, int *ptr_children_k, int children_k_index);
    void FinalizeIt();
    bool isCacheAble();
    FeatureManager *GetFeatureManager();
    void AddObj(double obj);
    void Reset();
    void AddCount(int f_local,double count);
    void SetGlobalMode();
    int ToLocalFeature(int f_global);
    int  *GetFeatures();
    int GetFeatureSize();
    double GetCount(int f_local);
    double GetObj();
    void AddNeuralHyperEdge(int netId, Network *ptr_network, int parent_k, int children_k_index, ComType::Neural_Input *ptr_edgeinput, int output);
    bool BuildNeuralCache(int netId, Network *ptr_network, int parent_k, int children_k_index,ComType::Neural_Input *ptr_edgeinput, int output);
    ComType::Neural_Input_Map_Vect *GetLocalNNInput2Id();
    NeuralIO * GetHyperEdgeIO(Network *ptr_network,int netId, int parent_k, int children_k_index);
    FeatureArray **** GetCache();
    NeuralIO ***** GetNeuralIO();
    int GetNerualNetSize();
protected:
    int thread_id_;
    FeatureManager *ptr_fm_;
    double current_obj_;
    int *ptr_fs_;
    //the gradient array for the features stored in ptr_fs_;
    double *ptr_counts_;
    std::unordered_map<int ,int> *ptr_globalFeature2LocalFeature_;
    bool isFinalized_;
    //a hyperedge, which consists of networkId, parent no, hyperedge no, note that each
    FeatureArray **** ptr_cache_;
    bool is_cache_enabled_;
    int num_networks_;
    bool is_global_mode_;
    int fs_size_;
    /*neural network ID, graph networkId, parent Id, Child IDs, (graphical network based on each instances),  */
    NeuralIO ***** ptr_neural_cache_;
    /*input map vector for all neural networks, which stores the input vector map, and the output*/
    ComType::Neural_Input_Map_Vect *ptr_localNNInput2IdMap_vect_ ;
    int neural_net_size_;
};

#endif //STATNLP_LOCAL_NETWORK_PARAM_H
