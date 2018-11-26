//
// Created by  ngs on 25/11/2018.
//

#ifndef STATNLP_TEST_NETWORK_H
#define STATNLP_TEST_NETWORK_H

#include "src/hybridnetworks/table_lookup_network.h"
#include "src/hybridnetworks/local_network_param.h"
class TestNetwork: public TableLookupNetwork{
public:
    TestNetwork();
    ~TestNetwork();
    TestNetwork(int networkid, Instance *ptr_inst, LocalNetworkParam *ptr_param);
    TestNetwork(int networkId, TestNetwork *ptr_inst, long *ptr_nodes, int ***ptr_children,int *ptr_childrens_size, int **ptr_children_size, LocalNetworkParam *ptr_param, int num_nodes);
    int CountNodes();
    int num_of_nodes_;

};
#endif //STATNLP_TEST_NETWORK_H
