//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_GLOBAL_NETWORK_PARAM_H
#define STATNLP_GLOBAL_NETWORK_PARAM_H

#include "../common/opt/lbfgs.h"
#include "common.h"

class GlobalNetworkParam{
public:
    GlobalNetworkParam();
    ~GlobalNetworkParam();
    void LockIt();
    bool IsLocked();
    bool UpdateDiscriminative();
    bool UpdateGenerative();
    bool Update();
    bool IsDiscriminative();
    double GetWeight(int featureId);
    double GetOldObj();
    int ToFeature(std::string type, std::string output, std::string input);
    void ExpandFeaturesForGenerativeModelDuringTesting();
    double DoubleRandom(double min, double max);
    //caution: this function should be sychronized in multithread
    void ResetCountsAndObj();
    double SquareVector(double* vec, int size);
    int tmp_count_;
private:
    bool is_locked_;
    //the num of feature (feature size)
    int size_;
    //the final num of features.
    int fixed_featureSize_;
    bool is_discriminative_;
    //previous objective value;
    double obj_prev_;
    //current objective value;
    double obj_current_;
    double *ptr_weights_;
    //optimizer
    CRFPP::LBFGS *ptr_opt_;
    //L2 reguliztion weight;
    double kappa_;
    //feature map;
    FeatureIntMap *ptr_featureIntMap_;
    Type2InputMap *ptr_type2InputMap_;
    //gradient;
    double *ptr_counts_;
    //the final number of features
    int fixed_feature_size_;
    std::string ** ptr_feature2rep;

    int version_;

};

#endif //STATNLP_GLOBAL_NETWORK_PARAM_H
