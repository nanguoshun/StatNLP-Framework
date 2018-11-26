//
// Created by  ngs on 25/11/2018.
//

#ifndef STATNLP_FEATUREMGR_H
#define STATNLP_FEATUREMGR_H

#include <src/hybridnetworks/feature_manager.h>

class TestFeatureMgr: public FeatureManager{
public:
    inline TestFeatureMgr(std::vector<Instance*> *ptr_inst, GlobalNetworkParam* ptr_param){

    }

    inline ~TestFeatureMgr(){

    }

    inline FeatureArray *ExtractHelper(Network *ptr_network, int parent, int *ptr_children, int children_k_index) override {

    }
};
#endif //STATNLP_FEATUREMGR_H
