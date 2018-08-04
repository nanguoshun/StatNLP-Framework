//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_GLOBAL_NETWORK_PARAM_H
#define STATNLP_GLOBAL_NETWORK_PARAM_H

class GlobalNetworkParam{
public:
    GlobalNetworkParam();
    ~GlobalNetworkParam();
    void LockIt();
    bool UpdateDiscriminative();
    bool UpdateGenerative();
    bool Update();
    bool IsDiscriminative();
private:
    bool is_discriminative_;
    double obj_prev_;
    double obj_current_;
};

#endif //STATNLP_GLOBAL_NETWORK_PARAM_H
