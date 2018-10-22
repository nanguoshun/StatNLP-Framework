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
        num_of_nodes_ = -1;
    }

    inline TreeCRFNetwork(int networkid, Instance *ptr_inst, LocalNetworkParam *ptr_param):TableLookupNetwork(networkid,ptr_inst,ptr_param) {
        num_of_nodes_ = -1;
    }

    inline TreeCRFNetwork(int networkId, TreeCRFInstance *ptr_inst, long *ptr_nodes, int ***ptr_children,
                          int *ptr_childrens_size, int **ptr_children_size, LocalNetworkParam *ptr_param,
                          int num_nodes):TableLookupNetwork(networkId,ptr_inst,ptr_nodes,ptr_children,ptr_param) {
        num_of_nodes_ = num_nodes;
        node_size_ = num_nodes;
        ptr_childrens_size_ = ptr_childrens_size;
        ptr_children_size_ = ptr_children_size;

    }

    inline ~TreeCRFNetwork() {

    }

    inline int CountNodes(){
        if(this->num_of_nodes_ == -1){
            return TableLookupNetwork::CountNodes();
        }
        return num_of_nodes_;
    }

private:
    int num_of_nodes_;
};

#endif //STATNLP_TREE_CRF_NETWORK_H

