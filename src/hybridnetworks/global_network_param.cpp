//
// Created by  ngs on 31/07/2018.
//

#include "global_network_param.h"

GlobalNetworkParam::GlobalNetworkParam() {

}

GlobalNetworkParam::~GlobalNetworkParam() {

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