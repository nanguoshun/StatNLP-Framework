//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_CRF_FEATUURE_MANAGER_H
#define STATNLP_TREE_CRF_FEATUURE_MANAGER_H

#include "src/hybridnetworks/feature_manager.h"
#include "src/hybridnetworks/global_network_param.h"
#include "tree_crf_network.h"
#include "rule.h"

class TreeCRFFeatureManager: public FeatureManager{
public:
    TreeCRFFeatureManager();
    ~TreeCRFFeatureManager();
    TreeCRFFeatureManager(std::vector<Instance*> *ptr_inst, GlobalNetworkParam* ptr_param);
    FeatureArray* ExtractHelper(Network *ptr_network, int parent, int *ptr_children, int children_k_index);
    static std::string feature_type_[8];
private:
    std::vector<Instance*> *ptr_inst_vector_;
    //for test only
    int root_count_;
    int zero_count_;
    //above parameters are for test only
};

#endif //STATNLP_TREE_CRF_FEATUURE_MANAGER_H
