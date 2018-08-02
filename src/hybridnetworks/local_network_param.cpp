//
// Created by  ngs on 31/07/2018.
//

#include "local_network_param.h"
#include "common.h"

LocalNetworkParam::LocalNetworkParam() {

}

LocalNetworkParam::LocalNetworkParam(int threadId, FeatureManager *ptr_fm, int numNetworks) {
    this->thread_id_ = threadId;
    this->num_networks_ = numNetworks;
    this->ptr_fm_ = ptr_fm;
    //this->fs_ = NULL;
    this->ptr_globalFeature2LocalFeature_ = new std::unordered_map<int, int>();
    this->isFinalized_ = false;
    this->is_gobal_mode_ = false;
    // to be done for multithread
    if(!_CACHE_FEATURES_DURING_TRAINING){
        this->DisableCache();
    }
    if(Num_Of_Threads ==1){
        this->is_gobal_mode_ = true;
    }
}

LocalNetworkParam::~LocalNetworkParam() {

}

void LocalNetworkParam::DisableCache() {
    this->ptr_cache_ = NULL;
    this->cache_enabled_ = false;
}

int LocalNetworkParam::GetThreadId() {
    return thread_id_;
}

double LocalNetworkParam::GetWeight(int featureId) {
    if(this->IsGlobalMode()){
        //return this->ptr_fm_.
    } else{
        // to be done;
    }
}

bool LocalNetworkParam::IsGlobalMode() {
    return is_gobal_mode_;
}

void LocalNetworkParam::FinalizeIt() {
    if(this->IsGlobalMode()){
        this->isFinalized_ = true;
        return;
    }
    this->ptr_fs_ = new std::vector<int>(this->ptr_globalFeature2LocalFeature_->size());

}

FeatureArray* LocalNetworkParam::Extract(Network *ptr_network, int parent_k, std::vector<int> *ptr_children_k,
                                         int children_k_index) {
    //to be done...
    FeatureArray * ptr;
    return ptr;
}