//
// Created by  ngs on 02/08/2018.
//

#ifndef STATNLP_LINEAR_CRF_LINEAR_FEATURE_MANAGER_H
#define STATNLP_LINEAR_CRF_LINEAR_FEATURE_MANAGER_H

#include "../hybridnetworks/feature_manager.h"
#include "../common/types/instance.h"

enum FeatureType{
    WORD,
    WORD_BIGRAM,
    TAG,
    TAG_BIGRAM,
    TRANSITION
};

struct FeatureT{
    std::string type;
    bool isOn;
};

const int NUM_FEATURE_TYPE = 5;

class LinearCRFFeatureManager: public FeatureManager{
public:
//    LinearCRFFeatureManager();
    LinearCRFFeatureManager(GlobalNetworkParam *ptr_param, std::vector<Instance*> *ptr_inst);
    ~LinearCRFFeatureManager();
    FeatureArray* ExtractHelper(Network *ptr, int parent, int *ptr_children);
    static int word_hal_window_size_;
    static std::string feature_type_[5];
private:
    std::vector<Instance*> *ptr_inst_vector_;
    FeatureT *ptr_feature_type_;
};
#endif //STATNLP_LINEAR_CRF_LINEAR_FEATURE_MANAGER_H
