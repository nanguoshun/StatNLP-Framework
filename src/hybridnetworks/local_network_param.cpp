//
// Created by  ngs on 31/07/2018.
//

#include "local_network_param.h"
#include "common.h"
#include "network.h"

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
    if(!ComParam::_CACHE_FEATURES_DURING_TRAINING){
        this->DisableCache();
    }
    if(ComParam::Num_Of_Threads ==1){
        this->is_gobal_mode_ = true;
    }
    is_cache_enabled_ = true;
    ptr_cache_ = nullptr;
}

LocalNetworkParam::~LocalNetworkParam() {

}

void LocalNetworkParam::DisableCache() {
    this->ptr_cache_ = nullptr;
    this->is_cache_enabled_ = false;
}

int LocalNetworkParam::GetThreadId() {
    return thread_id_;
}

double LocalNetworkParam::GetWeight(int featureId) {
    if(this->IsGlobalMode()){
        return this->ptr_fm_->GetGlobalParam()->GetWeight(featureId);
    } else{
        return this->ptr_fm_->GetGlobalParam()->GetWeight(this->ptr_fs_[featureId]);
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
    fs_size_ = this->ptr_globalFeature2LocalFeature_->size();
    this->ptr_fs_ = new int[fs_size_];
    // to be confirmed for this part: global to local feature.
    for(auto it = this->ptr_globalFeature2LocalFeature_->begin(); it != ptr_globalFeature2LocalFeature_->end(); ++it){
        int f_global = (*it).first;
        int f_local = this->ptr_globalFeature2LocalFeature_->find(f_global)->second;
        ptr_fs_[f_local] = f_global;
    }
    this->isFinalized_= true;
    this->ptr_counts_ = new double[fs_size_];
}

bool LocalNetworkParam::isCacheAble() {
    return is_cache_enabled_;
}

/**
 * Extract features from a hyperedge, which is denoted by the parent node parent_k (its index), the hyperedge index
 (as there are multiple hyper edges that is rooted by parent node parent_k), and the children_nodes of the

 * @param ptr_network : the network (graph)
 * @param parent_k : the index of parent node
 * @param ptr_children_k : the pointer of a hyperedge.
 * @param children_k_index : the index of the hyperedge, which is rooted by parent node.
 * @return
 */
FeatureArray* LocalNetworkParam::Extract(Network *ptr_network, int parent_k, int *ptr_children_k,
                                         int children_k_index) {
    if(this->isCacheAble()){
        if(!this->ptr_cache_){
            this->ptr_cache_ = new FeatureArray***[this->num_networks_];
            for(int networkid = 0; networkid < this->num_networks_; ++networkid){
                ptr_cache_[networkid] = nullptr;
            }
        }
        int networkId = ptr_network->GetNetworkID();
        if(!this->ptr_cache_[networkId]){
            this->ptr_cache_[networkId] = new FeatureArray**[ptr_network->CountNodes()];
            for(int nodeId = 0; nodeId < ptr_network->CountNodes(); ++nodeId){
                this->ptr_cache_[networkId][nodeId] = nullptr;
            }
        }
        if(!this->ptr_cache_[networkId][parent_k]){
            //size indicates the num of hyperedges rooted by node parent_k;
            int size = ptr_network->GetChildrens_Size(parent_k);
            this->ptr_cache_[networkId][parent_k] = new FeatureArray*[size];
            for(int hyperedge_no = 0; hyperedge_no < size; ++hyperedge_no){
                this->ptr_cache_[networkId][parent_k][hyperedge_no] = nullptr;
            }
        }
        if(this->ptr_cache_[networkId][parent_k][children_k_index]){
            return ptr_cache_[networkId][parent_k][children_k_index];
        }
    }
    //if the feature Array is not cached in the ptr_cache, then extract it via feature manager
    FeatureArray *ptr_fa = this->ptr_fm_->Extract(ptr_network,parent_k,ptr_children_k,children_k_index);
    if(this->isCacheAble()){
        //store the FeatureArray pointer to the cache.
        this->ptr_cache_[ptr_network->GetNetworkID()][parent_k][children_k_index] = ptr_fa;
    }
    return ptr_fa;
}

FeatureManager* LocalNetworkParam::GetFeatureManager() {
    return ptr_fm_;
}

void LocalNetworkParam::AddObj(double obj) {
    if(this->is_gobal_mode_){
        this->ptr_fm_->GetGlobalParam()->AddObj(obj);
        return;
    }
    this->current_obj_ += obj;
}

void LocalNetworkParam::Reset() {
    if(this->is_gobal_mode_){
        return;
    }
    this->current_obj_ = 0;
    for(int i=0; i<fs_size_; ++i){
        ptr_counts_[i] = 0.0;
    }
}

/**
 *
 * Add the gradient to the feature indexed by f_local.
 *
 * @param f_local
 * @param count
 */

void LocalNetworkParam::AddCount(int f_local, double count) {
    if(f_local == -1){
        std::cerr<<"Error: the feature id is -1. @LocalNetworkParam::AddCount "<<std::endl;
        return;
    }
    if(isnan(count)){
        std::cerr<<"Error: the count is NAN. @LocalNetworkParam::AddCount"<<std::endl;
    }
    if(this->is_gobal_mode_){
        this->ptr_fm_->GetGlobalParam()->AddCount(f_local,count);
        return;
    }
    this->ptr_counts_[f_local] += count;
}

void LocalNetworkParam::SetGlobalMode() {
    this->is_gobal_mode_ = true;
    this->ptr_globalFeature2LocalFeature_ = nullptr;
}