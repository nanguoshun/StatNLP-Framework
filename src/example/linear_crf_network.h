//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_LINEAR_CRF_NETWORK_H
#define STATNLP_LINEAR_CRF_NETWORK_H

#include "../hybridnetworks/table_lookup_network.h"
#include "linear_crf_instance.h"

class LinearCRFNetwork: public TableLookupNetwork{
public:
    LinearCRFNetwork();
    ~LinearCRFNetwork();
    LinearCRFNetwork(int NetworkId, Instance *ptr_inst, LocalNetworkParam *ptr_param);
    LinearCRFNetwork(int networkId, LinearCRFInstance *ptr_inst, long *ptr_nodes, int ***ptr_children, LocalNetworkParam *ptr_param, int num_nodes);
    int CountNodes() override;

    bool IsRemovded(int k) override;

    void Remove(int k) override;



private:

    int num_of_nodes_;
};
#endif //STATNLP_LINEAR_CRF_NETWORK_H
