//
// Created by  ngs on 01/08/2018.
//
#include "linear_crf_network.h"

LinearCRFNetwork::LinearCRFNetwork() {
    this->num_of_nodes_ = -1;
}

LinearCRFNetwork::~LinearCRFNetwork() {

}

LinearCRFNetwork::LinearCRFNetwork(int networkid, Instance *ptr_inst, LocalNetworkParam *ptr_param)
        : TableLookupNetwork(networkid, ptr_inst, ptr_param) {
    this->num_of_nodes_ = -1;
}

LinearCRFNetwork::LinearCRFNetwork(int networkid, LinearCRFInstance *ptr_inst, TableLookupNetwork *ptr_table_network,
                                   LocalNetworkParam *ptr_param, int num_nodes) : TableLookupNetwork(networkid,
                                                                                                     ptr_inst,
                                                                                                     ptr_table_network,
                                                                                                     ptr_param,
                                                                                                     num_nodes) {

    num_of_nodes_ = num_nodes;
    node_size_ = num_nodes;
}

LinearCRFNetwork::LinearCRFNetwork(int networkid, LinearCRFInstance *ptr_inst, long *ptr_nodes, int ***ptr_children,
                                   int *ptr_childrens_size, int **ptr_children_size, LocalNetworkParam *ptr_param,
                                   int num_nodes) : TableLookupNetwork(networkid, ptr_inst, ptr_nodes, ptr_children,
                                                                       ptr_param) {

    num_of_nodes_ = num_nodes;
    node_size_ = num_nodes;
    ptr_childrens_size_ = ptr_childrens_size;
    ptr_children_size_ = ptr_children_size;
}

int LinearCRFNetwork::CountNodes() {
    if (this->num_of_nodes_ == -1) {
        return TableLookupNetwork::CountNodes();
    }
    return num_of_nodes_;
}


bool LinearCRFNetwork::IsRemovded(int k) {
    return false;
}

void LinearCRFNetwork::Remove(int k) {

}
