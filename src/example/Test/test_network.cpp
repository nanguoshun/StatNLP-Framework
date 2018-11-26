//
// Created by  ngs on 25/11/2018.
//
#include "test_network.h"

TestNetwork::TestNetwork(){
    num_of_nodes_ = -1;
}

TestNetwork::TestNetwork(int networkid, Instance *ptr_inst, LocalNetworkParam *ptr_param):TableLookupNetwork(networkid,ptr_inst,ptr_param) {
    num_of_nodes_ = -1;
}

TestNetwork::TestNetwork(int networkId, TestNetwork *ptr_inst, long *ptr_nodes, int ***ptr_children, int *ptr_childrens_size, int **ptr_children_size, LocalNetworkParam *ptr_param, int num_nodes):TableLookupNetwork(networkId,ptr_inst,ptr_nodes,ptr_children,ptr_param){
    num_of_nodes_ = num_nodes;
    node_size_ = num_nodes;
    ptr_childrens_size_ = ptr_childrens_size;
    ptr_children_size_ = ptr_children_size;

}

int TestNetwork::CountNodes() {
    if(this->num_of_nodes_ == -1){
        return TableLookupNetwork::CountNodes();
    }
    return num_of_nodes_;
}


TestNetwork::~TestNetwork() {

}