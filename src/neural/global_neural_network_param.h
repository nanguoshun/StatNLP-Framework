//
// Created by  ngs on 03/09/2018.
//

#ifndef STATNLP_GLOBAL_NN_PARAM_H
#define STATNLP_GLOBAL_NN_PARAM_H

#include "neural_network.h"
#include "src/common/common.h"

class LocalNetworkParam;
class GlobalNeuralNetworkParam{
public:
    GlobalNeuralNetworkParam(std::vector<NeuralNetwork *> *ptr_nn_vec);
    ~GlobalNeuralNetworkParam();
    void SetLearningState();
    void InitNetwork();
private:
    std::vector<NeuralNetwork *> *ptr_nn_vec_;
    LocalNetworkParam **pptr_param_l_;
};
#endif //STATNLP_GLOBAL_NN_PARAM_H
