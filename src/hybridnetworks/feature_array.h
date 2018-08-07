//
// Created by  ngs on 02/08/2018.
//

#ifndef STATNLP_FEATURE_ARRAY_H
#define STATNLP_FEATURE_ARRAY_H

#include <vector>

class LocalNetworkParam;

class FeatureArray{
public:
    FeatureArray();
    FeatureArray(double score);
    ~FeatureArray();
    double GetScore(LocalNetworkParam *ptr_local_param);
    double ComputeScore(LocalNetworkParam *ptr_local_param, std::vector<int> *ptr_fs);
    std::vector<int> * GetCurrent();
private:
    double score_;
    std::vector<int> *ptr_fs_;

};
#endif //STATNLP_FEATURE_ARRAY_H
