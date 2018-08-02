//
// Created by  ngs on 02/08/2018.
//

#ifndef STATNLP_DISCRIMINATIVE_NETWORK_MODEL_H
#define STATNLP_DISCRIMINATIVE_NETWORK_MODEL_H

#include "network_model.h"
#include "feature_manager.h"
#include "network_compiler.h"

class DiscriminativeNetworkModel: public NetworkModel{
public:
    DiscriminativeNetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc);
    ~DiscriminativeNetworkModel();

    
};
#endif //STATNLP_DISCRIMINATIVE_NETWORK_MODEL_H
