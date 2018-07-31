//
// Created by  ngs on 31/07/2018.
//

#include "network_config.h"

NetworkConfig::NetworkConfig() {

}

NetworkConfig::~NetworkConfig() {

}

double NetworkConfig::GetL2Param() {
    return L2_para_;
}

void NetworkConfig::SetL2Param(double L2) {
    L2_para_ = L2;
}