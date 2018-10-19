//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_CRF_FEATUURE_MANAGER_H
#define STATNLP_TREE_CRF_FEATUURE_MANAGER_H

#include "src/hybridnetworks/feature_manager.h"
#include "src/hybridnetworks/global_network_param.h"
#include "tree_crf_network.h"

enum FeatureType{
    LEFT_RIGHT,
    FIRST_WORD,
    SPLIT_WORD,
    LAST_WORD,
    LAST_WORD_ENDING_1,
    LAST_WORD_ENDING_2,
    LAST_WORD_ENDING_3,
    FIRST_CAPITAL
};

class TreeCRFFeatureManager: public FeatureManager{
public:
    TreeCRFFeatureManager();
    ~TreeCRFFeatureManager();
    TreeCRFFeatureManager(std::vector<Instance*> *ptr_inst, GlobalNetworkParam* ptr_param);
    FeatureArray* ExtractHelper(Network *ptr_network, int parent, int *ptr_children, int children_k_index);

private:
    std::vector<Instance*> *ptr_inst_vector_;
};

#endif //STATNLP_TREE_CRF_FEATUURE_MANAGER_H
