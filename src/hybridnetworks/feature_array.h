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
    FeatureArray(int *ptr_fs, int fs_size);
    FeatureArray(int *ptr_fs, int fs_size, FeatureArray *ptr_next);
    FeatureArray(double score);
    ~FeatureArray();
    double GetScore(LocalNetworkParam *ptr_local_param);
    double ComputeScore(LocalNetworkParam *ptr_local_param, int *ptr_fs);
    int * GetCurrent();
    void Update(LocalNetworkParam *ptr_param, double count);
    static FeatureArray *PTR_EMPTY;
private:
    double score_;
    int *ptr_fs_;
    int fs_size_;
    FeatureArray *ptr_next_;
};
#endif //STATNLP_FEATURE_ARRAY_H
