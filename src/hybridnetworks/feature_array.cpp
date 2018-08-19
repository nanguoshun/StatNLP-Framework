//
// Created by  ngs on 02/08/2018.
//

#include "feature_array.h"
#include "common.h"
#include "local_network_param.h"

FeatureArray * FeatureArray::PTR_EMPTY = new FeatureArray((int*)NULL,0);

FeatureArray::FeatureArray() {

}

FeatureArray::~FeatureArray() {
    delete ptr_fs_;
}

FeatureArray::FeatureArray(int *ptr_fs, int fs_size) {
    ptr_fs_ = ptr_fs;
    fs_size_ = fs_size;
}

FeatureArray::FeatureArray(int *ptr_fs, int fs_size, FeatureArray *ptr_next) {
    ptr_fs_ = ptr_fs;
    fs_size_ = fs_size;
    ptr_next_ = ptr_next;
}

FeatureArray::FeatureArray(double score) {
    score_ = score;
}

double FeatureArray::GetScore(LocalNetworkParam *ptr_local_param) {
    if(this->score_ == DOUBLE_NEGATIVE_INFINITY){
        return this->score_;
    }
    this->score_ = ComputeScore(ptr_local_param, ptr_fs_);
}

double FeatureArray::ComputeScore(LocalNetworkParam *ptr_local_param, int* ptr_fs) {
    double score = 0.0;
    //ptr_fs stores the feature ID.
    for(int i=0; i<fs_size_; ++i){
        int fs = ptr_fs[i] ;
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

}