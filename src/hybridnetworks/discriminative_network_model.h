//
// Created by  ngs on 02/08/2018.
//

#ifndef STATNLP_DISCRIMINATIVE_NETWORK_MODEL_H
#define STATNLP_DISCRIMINATIVE_NETWORK_MODEL_H

#include "network_model.h"
#include "feature_manager.h"
#include "network_compiler.h"
#include <vector>

class DiscriminativeNetworkModel: public NetworkModel{
public:
    DiscriminativeNetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc);
    ~DiscriminativeNetworkModel();
    std::vector<std::vector<Instance*>*> *SplitInstanceForTrain() override;

private:
    //to improve the efficiency, we use the pointer there instead of STL vector.
    //thread id, instance array, each instance pointer.
    std::vector<std::vector<Instance*>*> *pptr_insts_;
    //thread id, instances.
    std::vector<std::vector<Instance*>*> *ptr_inst_matrix_;
};
#endif //STATNLP_DISCRIMINATIVE_NETWORK_MODEL_H
