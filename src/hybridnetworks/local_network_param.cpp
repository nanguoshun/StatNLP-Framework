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
    this->is_cache_enabled_ = false;
}

int LocalNetworkParam::GetThreadId() {
    return thread_id_;
}

double LocalNetworkParam::GetWeight(int featureId) {
    if(this->IsGlobalMode()){
        return this->ptr_fm_->GetGlobalParam()->GetWeight(featureId);
    } else{
        return this->ptr_fm_->GetGlobalParam()->GetWeight((*this->ptr_fs_)[featureId]);
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
    // to be confirmed for this part: global to local feature.
    for(auto it = this->ptr_globalFeature2LocalFeature_->begin(); it != ptr_globalFeature2LocalFeature_->end(); ++it){
        int f_global = (*it).first;
        int f_local = this->ptr_globalFeature2LocalFeature_->find(f_global)->second;
        (*ptr_fs_)[f_local] = f_global;
    }
    this->isFinalized_= true;
    this->ptr_counts_ = new std::vector<int>(this->ptr_fs_->size());
}

bool LocalNetworkParam::isCacheAble() {
    return is_cache_enabled_;
}

FeatureArray* LocalNetworkParam::Extract(Network *ptr_network, int parent_k, std::vector<int> *ptr_children_k,
                                         int children_k_index) {
    if(this->isCacheAble()){

    }
    //to be done...
    FeatureArray * ptr;
    return ptr;
}