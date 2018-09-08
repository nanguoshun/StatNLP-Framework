//
// Created by  ngs on 31/07/2018.
//

#include "feature_manager.h"
#include "src/common/common.h"
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
    delete [] pptr_param_l_;
}

GlobalNetworkParam* FeatureManager::GetGlobalParam() {
    return ptr_param_g_;
}

bool FeatureManager::Update(bool just_update_obj_gradient) {
#ifdef GLOBAL
    std::lock_guard<std::mutex> mtx_locker(mtx);
#endif
    //FIXME: why the updating process is different for single thread and mutlithread?? the former will be updated in the current iteration
    //FIXME: while the latter will updated in the next iteration ( at the beginning of the function).
    /**
     * For the multithread mode, update the gradient and then calc the objective.
     */
    if (this->num_of_threads_ != 1) {
        this->ptr_param_g_->ResetCountsAndObj();
        for(int i=0; i<this->num_of_threads_; ++i){
            int *ptr_fs = pptr_param_l_[i]->GetFeatures();
            int fs_size = pptr_param_l_[i]->GetFeatureSize();
            for(int j = 0; j < fs_size; ++j){
                int fs_global = ptr_fs[j];
                double count = pptr_param_l_[i]->GetCount(j);
                this->ptr_param_g_->AddCount(fs_global,count);
            }
            this->ptr_param_g_->AddObj(pptr_param_l_[i]->GetObj());
        }
    }
    /**
     * we always get the gradient and then the objective based on the weights calculated in the previous
     * iteration, hence we will have one more iteration compared with the maximum iteration value to get the
     * last gradient and objective
     */
    if(just_update_obj_gradient){
        double obj = ptr_param_g_->GetCurrentObj();
        ptr_param_g_->SetOldObj(obj);
        return false;
    }
    //calc the gradient of the NN
    ptr_param_g_->GetNNParam()->Backward();
    //update the gradient.
    bool done = this->ptr_param_g_->Update();
    if (this->num_of_threads_ != 1) {
        for (int threadId = 0; threadId < this->num_of_threads_; ++threadId) {
            pptr_param_l_[threadId]->Reset();
        }
    } else {
        //update the objective and reset all gradients to 0 in the vector
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