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
    bool UpdateDiscriminative();
    bool UpdateGenerative();
    bool Update();
    bool IsDiscriminative();
    double GetWeight(int featureId);
    double GetOldObj();
private:
    bool is_locked_;
    //the num of feature
    int size_;
    //the final num of features.
    int fixed_featureSize_;
    bool is_discriminative_;
    //previous objective value;
    double obj_prev_;
    //current objective value;
    double obj_current_;
    std::vector<double> *ptr_weights_;
    CRFPP::LBFGS *ptr_opt;
    //L2 reguliztion weight;
    double kappa_;
    FeatureIntMap *ptr_featureIntMap_;
    Type2InputMap *ptr_type2InputMap_;
};

#endif //STATNLP_GLOBAL_NETWORK_PARAM_H
