//
// Created by  ngs on 01/08/2018.
//

#include "network_id_manager.h"

std::vector<int> *NetworkIDManager::ptr_capacity_vec_ = new std::vector<int>{ComParam::FOREST_MAX_HEIGHT + 1,
                                                                             ComParam::FOREST_MAX_WIDTH + 1,
                                                                             ComParam::FOREST_MAX_HEIGHT + 1,
                                                                             ComParam::FOREST_MAX_HEIGHT + 1,
                                                                             ComParam::NETWORK_MAX_DEPTH};

NetworkIDManager::NetworkIDManager() {
}

NetworkIDManager::~NetworkIDManager() {
    delete ptr_capacity_vec_;
}

/**
 * Convert a node array an node ID, which is unique in the graph.
 * @param vec
 * @return
 */
long NetworkIDManager::ToHybridNodeID(std::vector<int> &vec) {
    long value = vec[0];
    for(int i = 1; i < vec.size(); ++i){
        if(vec[i] >= (*ptr_capacity_vec_)[i]){
            std::cout << "Invalid: capacity"<<std::endl;
            return -1;
        }
        value = value * (*ptr_capacity_vec_)[i] + vec[i];
    }
    return value;
}

std::vector<int> NetworkIDManager::ToHybridNodeArray(long nodeId) {
    int size = ptr_capacity_vec_->size();
    std::vector<int> result(size,0);
    for(int k=size-1; k>=1; --k){
        long v = nodeId / (*ptr_capacity_vec_)[k];
        result[k] = (int) (nodeId % (*ptr_capacity_vec_)[k]);
        nodeId = v;
    }
    result[0] = (int) nodeId;
    return result;
}

