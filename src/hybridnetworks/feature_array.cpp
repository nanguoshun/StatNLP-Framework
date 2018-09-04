//
// Created by  ngs on 02/08/2018.
//

#include "feature_array.h"
#include "src/common/common.h"
#include "local_network_param.h"

FeatureArray * FeatureArray::PTR_EMPTY = new FeatureArray((int*) nullptr,0);

FeatureArray::FeatureArray() {
    score_ = 0;
    ptr_fs_ = nullptr;
    fs_size_ = 0;
    ptr_next_= nullptr;
    islocal_ = false;

}

FeatureArray::~FeatureArray() {
    delete ptr_fs_;
}

FeatureArray::FeatureArray(int *ptr_fs, int fs_size) {
    score_ = 0;
    ptr_fs_ = ptr_fs;
    fs_size_ = fs_size;
    ptr_next_ = nullptr;
    islocal_ = false;
}

FeatureArray::FeatureArray(int *ptr_fs, int fs_size, FeatureArray *ptr_next) {
    score_ = 0;
    ptr_fs_ = ptr_fs;
    fs_size_ = fs_size;
    ptr_next_ = ptr_next;
    islocal_ = false;
}

FeatureArray::FeatureArray(double score) {
    score_ = score;
    fs_size_ = 0;
    ptr_fs_ = nullptr;
    ptr_next_ = nullptr;
}

/**
 *
 * Compute the score summation for all features for all FeatureArray instances of the hyperedge.
 * @param ptr_local_param: local parameter pointer.
 * @return: score value
 */
double FeatureArray::GetScore(LocalNetworkParam *ptr_local_param) {
    if(this->score_ == ComParam::DOUBLE_NEGATIVE_INFINITY){
        return this->score_;
    }
    //compute the scores of features in the feature array ptr_fs_;
    this->score_ = ComputeScore(ptr_local_param, ptr_fs_);
    //compute the scores in next FeatureArray instance.
    if(this->ptr_next_ != nullptr){
        this->score_ += this->ptr_next_->GetScore(ptr_local_param);
    }
    return this->score_;
}

/**
 *
 * Compute the score summation for all features stored in the array ptr_fs.
 *
 * @param ptr_local_param:
 * @param ptr_fs: the array that stores the feature ids
 * @return: score value
 */
double FeatureArray::ComputeScore(LocalNetworkParam *ptr_local_param, int* ptr_fs) {
    double score = 0.0;
    //ptr_fs stores the feature ID.
    for(int i=0; i<fs_size_; ++i){
        int fs = ptr_fs[i];
        if(fs != -1){
            //fs indicates the feature ID.
            score += ptr_local_param->GetWeight(fs);
        }
    }
    return score;
}

int* FeatureArray::GetCurrent() {
    return  ptr_fs_;
}

void FeatureArray::Update(LocalNetworkParam *ptr_param, double count) {
    int *ptr_fs = this->GetCurrent();
    for(int i=0; i<fs_size_; ++i){
        ptr_param->AddCount(ptr_fs[i],count);
    }
    if(nullptr != this->ptr_next_){
        this->ptr_next_->Update(ptr_param,count);
    }
}

FeatureArray* FeatureArray::ToLocal(LocalNetworkParam *ptr_param) {
    if(islocal_){
        return this;
    }
    int* ptr_fs_local = new int[fs_size_];
    for (int i = 0; i < fs_size_; ++i){
        ptr_fs_local[i] = ptr_param->ToLocalFeature(ptr_fs_[i]);
        if(-1 == ptr_fs_local[i]){
            std::cerr << "The local feature got an id of -1 for "<<ptr_fs_[i]<<std::endl;
        }
    }
    FeatureArray *ptr_fa;
    if(this->ptr_next_){
        ptr_fa = new FeatureArray(ptr_fs_local,fs_size_,ptr_next_->ToLocal(ptr_param));
    } else{
        ptr_fa = new FeatureArray(ptr_fs_local,fs_size_);
    }
    islocal_ = true;
    return ptr_fa;
}