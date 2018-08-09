//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_GLOBAL_NETWORK_PARAM_H
#define STATNLP_GLOBAL_NETWORK_PARAM_H

#include <vector>
#include "../common/opt/lbfgs.h"
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
    bool is_discriminative_;
    //previous objective value;
    double obj_prev_;
    //current objective value;
    double obj_current_;
    std::vector<double> *ptr_weights_;
    CRFPP::LBFGS *ptr_opt;

};

#endif //STATNLP_GLOBAL_NETWORK_PARAM_H
