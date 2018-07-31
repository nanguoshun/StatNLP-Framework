//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_NETWORK_MODEL_H
#define STATNLP_NETWORK_MODEL_H

#include "../common/types/instance.h"

class NetworkModel{
public:
NetworkModel();
~NetworkModel();
void Train(Instance *ptr_train_instances, int max_num_interations);
Instance* Decode(Instance *ptr_test_instences);
};

#endif //STATNLP_NETWORK_MODEL_H
