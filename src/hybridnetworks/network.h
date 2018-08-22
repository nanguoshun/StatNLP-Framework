//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_NETWORK_H
#define STATNLP_NETWORK_H

#include "hypergraph.h"
#include "../common/types/instance.h"
#include "local_network_param.h"
#include "network_id_manager.h"
#include <vector>

/**
 *  inside-outside algorithm.
 */
class Network: public HyperGraph{
public:
    Network();
    Network(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param);
    ~Network();
    void Touch();
    void Touch(int k);
    void Train();
    void Inside();
    void Outside();
    void UpdateInsideOutside();
    void Inside(int nodeId);
    void Outside(int nodeId);
    void UpdateGradient(int nodeId);
    double* GetInsideSharedArray();
    double* GetOutsideSharedArray();
    //Network * GetNetwork(int networkId);
    int GetNetworkID();
    Instance *GetInstance();
    double GetInside(int nodeId);
    std::vector<int> GetNodeArray(int nodeIndex);
    int tmp_count_;
    static double **ptr_inside_shared_array_;
    static double **ptr_outside_shared_array_;
    //the array size for each thread
    static bool is_initialized_shared_array_;
    static void InitShareArray();
protected:
    int network_id_;
    int thread_id_;
    Instance *ptr_inst_;
    //the weight of an instance, usually we set it as 1 for instance un-biased training.
    double weight_;
    LocalNetworkParam *ptr_param_;
    double *ptr_inside_;
    double *ptr_outside_;
    int inside_shared_array_size_;
    int outside_shared_array_size_;
};

#endif //STATNLP_NETWORK_H
