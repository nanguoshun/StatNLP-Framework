//
// Created by  ngs on 02/08/2018.
//

#include "feature_array.h"
#include "common.h"
#include "local_network_param.h"

FeatureArray::FeatureArray() {

}

FeatureArray::~FeatureArray() {

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

double FeatureArray::ComputeScore(LocalNetworkParam *ptr_local_param, std::vector<int>* ptr_fs) {
    double score = 0.0;
    //ptr_fs stores the feature ID.
    for(auto it = ptr_fs->begin(); it != ptr_fs->end(); ++it){
        int fs = (*it);
        if(fs != -1){
            //fs indicates the feature ID.
            score += ptr_local_param->GetWeight(fs);
        }
    }
    return score;
}

std::vector<int>* FeatureArray::GetCurrent() {
    return  ptr_fs_;
}