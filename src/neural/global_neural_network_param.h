//
// Created by  ngs on 03/09/2018.
//

#ifndef STATNLP_GLOBAL_NN_PARAM_H
#define STATNLP_GLOBAL_NN_PARAM_H

#include "neural_network.h"
#include "src/common/common.h"

class Network;
class LocalNetworkParam;
class GlobalNeuralNetworkParam{
public:
    GlobalNeuralNetworkParam(std::vector<NeuralNetwork *> *ptr_nn_vec);
    ~GlobalNeuralNetworkParam();
    void SetLearningState();
    void InitNetwork();
    std::vector<NeuralNetwork *> *GetNNVect();
    void Forward();
    void Backward();
    double GetNNScore(Network *ptr_network, int parent_k, int children_k_index);
    void SetNNGradientOutput(double count, Network *ptr_network, int parent_k, int children_k_index);
    void ResetAllNNGradient();
private:
    std::vector<NeuralNetwork *> *ptr_nn_vec_;
    LocalNetworkParam **pptr_param_l_;
    //the nerual net's internal weight and gradient;
};
#endif //STATNLP_GLOBAL_NN_PARAM_H
