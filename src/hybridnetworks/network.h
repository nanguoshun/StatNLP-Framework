//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_NETWORK_H
#define STATNLP_NETWORK_H

#include "hypergraph.h"
#include "../common/types/instance.h"
#include "local_network_param.h"

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
    void UpdateInsideOutside(int nodeId);

protected:
    int network_id_;
    int thread_id_;
    Instance *ptr_inst_;
    double weight_;
    LocalNetworkParam *ptr_param_;

};

#endif //STATNLP_NETWORK_H
