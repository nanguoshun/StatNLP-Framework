//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_CRF_NETWORK_H
#define STATNLP_TREE_CRF_NETWORK_H

#include "src/hybridnetworks/table_lookup_network.h"
#include "tree_crf_instance.h"

class TreeCRFNetwork : public TableLookupNetwork {
public:
    inline TreeCRFNetwork() {

    }

    inline TreeCRFNetwork(int networkid, Instance *ptr_inst, LocalNetworkParam *ptr_param) {

    }

    inline TreeCRFNetwork(int networkId, TreeCRFInstance *ptr_inst, long *ptr_nodes, int ***ptr_children,
                          int *ptr_childrens_size, int **ptr_children_size, LocalNetworkParam *ptr_param,
                          int num_nodes) {

    }

    inline ~TreeCRFNetwork() {

    }
};

#endif //STATNLP_TREE_CRF_NETWORK_H

