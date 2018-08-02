//
// Created by  ngs on 01/08/2018.
//
#include "linear_crf_network.h"

LinearCRFNetwork::LinearCRFNetwork() {

}

LinearCRFNetwork::~LinearCRFNetwork() {

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

std::vector<std::vector<int>> *LinearCRFNetwork::GetChildren(int k) {
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
