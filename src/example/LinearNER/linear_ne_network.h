//
// Created by  ngs on 05/10/2018.
//

#ifndef STATNLP_LINEAR_NE_NETWORK_H
#define STATNLP_LINEAR_NE_NETWORK_H

#include "src/hybridnetworks/table_lookup_network.h"
#include "linear_ne_instance.h"
class LinearNENetwork: public TableLookupNetwork{
public:
    LinearNENetwork();
    LinearNENetwork(int networkid, Instance *ptr_inst, LocalNetworkParam *ptr_param);
    LinearNENetwork(int networkId, LinearNEInstance *ptr_inst, long *ptr_nodes, int ***ptr_children,int *ptr_childrens_size, int **ptr_children_size, LocalNetworkParam *ptr_param, int num_nodes);
    ~LinearNENetwork();
    int CountNodes();
private:
    int num_of_nodes_;
};

#endif //STATNLP_LINEAR_NE_NETWORK_H
