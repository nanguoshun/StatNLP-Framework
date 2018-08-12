//
// Created by  ngs on 01/08/2018.
//
#include "linear_crf_network.h"

LinearCRFNetwork::LinearCRFNetwork() {

}

LinearCRFNetwork::~LinearCRFNetwork() {

}

LinearCRFNetwork::LinearCRFNetwork(int networkid, Instance *ptr_inst, LocalNetworkParam *ptr_param):TableLookupNetwork(networkid,ptr_inst,ptr_param) {

}

LinearCRFNetwork::LinearCRFNetwork(int networkid, LinearCRFInstance *ptr_inst, long *ptr_nodes, int ***ptr_children,
                                   LocalNetworkParam *ptr_param, int num_nodes):TableLookupNetwork(networkid,ptr_inst,ptr_nodes,ptr_children,ptr_param) {

    this->num_of_nodes_ = num_nodes;

}

int LinearCRFNetwork::CountNodes() {
    return 0;
}

long LinearCRFNetwork::GetNode(int k) {
    return 0;
}

std::vector<int> *LinearCRFNetwork::GetNodeArray(int k) {
    return nullptr;
}

int **LinearCRFNetwork::GetChildren(int k) {
    return nullptr;
}

bool LinearCRFNetwork::IsRemovded(int k) {
    return false;
}

void LinearCRFNetwork::Remove(int k) {

}

bool LinearCRFNetwork::IsRoot(int k) {
    return false;
}

bool LinearCRFNetwork::IsLeaf(int k) {
    return false;
}

bool LinearCRFNetwork::IsContain(long node) {
    return false;
}
