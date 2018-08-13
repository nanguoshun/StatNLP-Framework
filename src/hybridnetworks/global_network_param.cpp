//
// Created by  ngs on 31/07/2018.
//

#include "global_network_param.h"

GlobalNetworkParam::GlobalNetworkParam() {
    is_locked_ = false;
    size_ = 0;
    fixed_featureSize_ = 0;
    obj_prev_= DOUBLE_NEGATIVE_INFINITY;
    obj_current_ = DOUBLE_NEGATIVE_INFINITY;
    is_discriminative_ = ! TRAIN_MODE_IS_GENERATIVE;
    if(IsDiscriminative()){
        ptr_opt = new CRFPP::LBFGS();
        kappa_ =L2_REGULARIZATION_CONSTANT;
    }
    ptr_featureIntMap_ = new FeatureIntMap;
    ptr_type2InputMap_ = new Type2InputMap;
}

GlobalNetworkParam::~GlobalNetworkParam() {
    delete ptr_opt;
    delete ptr_featureIntMap_;
    delete ptr_type2InputMap_;
}

void GlobalNetworkParam::LockIt() {
    // to be done....

}

bool GlobalNetworkParam::Update() {
    bool done;
    if(this->IsDiscriminative()){
        done = this->UpdateDiscriminative();
    } else{
        done = this->UpdateGenerative();
    }
    this->obj_prev_ = this->obj_current_;
    return done;
}

bool GlobalNetworkParam::UpdateDiscriminative() {

}

bool GlobalNetworkParam::UpdateGenerative() {

}

bool GlobalNetworkParam::IsDiscriminative() {
    return is_discriminative_;
}

double GlobalNetworkParam::GetWeight(int featureId) {
    return (*ptr_weights_)[featureId];
}

double GlobalNetworkParam::GetOldObj() {
    return obj_prev_;
}

int GlobalNetworkParam::ToFeature(std::string type, std::string output, std::string input) {
    //TODO:

}