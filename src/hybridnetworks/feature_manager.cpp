//
// Created by  ngs on 31/07/2018.
//

#include "feature_manager.h"
#include "common.h"
#include "network.h"

FeatureManager::FeatureManager() {

}

FeatureManager::FeatureManager(GlobalNetworkParam *ptr_param) {
    this->ptr_param_g_ = ptr_param;
    this->num_of_threads_ = ComParam::Num_Of_Threads;
    this->pptr_param_l_= new LocalNetworkParam*[this->num_of_threads_];
    this->cache_enabled_ = false;
    //this->word_hal_window_size_ = 1;
    this->pos_hal_window_size_ = -1;
}

FeatureManager::~FeatureManager() {
    for(int i=0; i<this->num_of_threads_; ++i){
        delete pptr_param_l_[i];
    }
    delete this->pptr_param_l_;
}

GlobalNetworkParam* FeatureManager::GetGlobalParam() {
    return ptr_param_g_;
}

bool FeatureManager::Update() {
    if (this->num_of_threads_ != 1) {
        //TODO: for multithread.
    }
    bool done = this->ptr_param_g_->Update();

    if (this->num_of_threads_ != 1) {
        for (int threadId = 0; threadId < this->num_of_threads_; ++threadId) {
            pptr_param_l_[threadId]->Reset();
        }
    } else {
        ptr_param_g_->ResetCountsAndObj();
    }
    return done;
}

void FeatureManager::EnableCache(int numNetworks) {
    num_of_networks_ = numNetworks;
    this->ptr_cache_ = new FeatureArray***[this->num_of_networks_];
    this->cache_enabled_ = true;
}

FeatureArray* FeatureManager::Extract(Network *ptr_network, int parent_k, int *ptr_children_k, int children_k_index) {
    if(this->isCacheAble()){
        int networkId = ptr_network->GetNetworkID();
        if(nullptr == this->ptr_cache_[networkId]){
            this->ptr_cache_[networkId] = new FeatureArray**[ptr_network->CountNodes()];
        }
        if(nullptr == this->ptr_cache_[networkId][parent_k]){
            //size indicates the num of hyperedges.
            int size = sizeof(ptr_network->GetChildren(parent_k));
            this->ptr_cache_[networkId][parent_k] = new FeatureArray*[size];
        }
        FeatureArray *ptr_fa = this->ptr_cache_[networkId][parent_k][children_k_index];
        //if this FeatureArray pointer is cached in the ptr_cache, then return the pointer from the cache.
        if(nullptr != ptr_fa){
            return ptr_fa;
        }
    }
    FeatureArray* ptr_fa = this->ExtractHelper(ptr_network,parent_k,ptr_children_k);
    if(this->isCacheAble()){
        this->ptr_cache_[ptr_network->GetNetworkID()][parent_k][children_k_index] = ptr_fa;
    }
    return ptr_fa;
}

bool FeatureManager::isCacheAble() {
    return cache_enabled_;
}
void FeatureManager::SetLocalNetworkParams(int threadId, LocalNetworkParam *ptr_param_l) {
    this->pptr_param_l_[threadId] = ptr_param_l;
}