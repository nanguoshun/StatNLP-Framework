//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_CRF_FEATUURE_MANAGER_H
#define STATNLP_TREE_CRF_FEATUURE_MANAGER_H

#include "src/hybridnetworks/feature_manager.h"

class TreeCRFFeatureManager: public FeatureManager{
public:
    TreeCRFFeatureManager();
    ~TreeCRFFeatureManager();
    TreeCRFFeatureManager(std::vector<Instance*> *ptr_inst, GlobalNetworkParam* ptr_param);
    FeatureArray* ExtractHelper(Network *ptr_network, int parent, int *ptr_children, int children_k_index);

};

#endif //STATNLP_TREE_CRF_FEATUURE_MANAGER_H
