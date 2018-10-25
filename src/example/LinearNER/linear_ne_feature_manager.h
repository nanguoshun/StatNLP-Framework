//
// Created by  ngs on 05/10/2018.
//

#ifndef STATNLP_LINEAR_NE_FEATURE_MANAGER_H
#define STATNLP_LINEAR_NE_FEATURE_MANAGER_H

#include "src/common/types/common.h"
#include "src/hybridnetworks/feature_manager.h"
#include "linear_ne_instance.h"
#include "linear_ne_network.h"
const int NUM_FEATURE_TYPE = 5;

class LinearNEFeatureManager: public FeatureManager{
public:
    LinearNEFeatureManager();
    LinearNEFeatureManager(std::vector<Instance*> *ptr_inst, GlobalNetworkParam* ptr_param);
    ~LinearNEFeatureManager();
    FeatureArray* ExtractHelper(Network *ptr_network, int parent, int *ptr_children, int children_k_index);
    static int word_hal_window_size_;
    static std::string feature_type_[5];
private:
    std::vector<Instance*> *ptr_inst_vector_;
    ComType::FeatureT *ptr_feature_type_;
};

#endif //STATNLP_LINEAR_NE_FEATURE_MANAGER_H
