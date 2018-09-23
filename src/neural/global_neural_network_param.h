//
// Created by  ngs on 03/09/2018.
//

#ifndef STATNLP_GLOBAL_NN_PARAM_H
#define STATNLP_GLOBAL_NN_PARAM_H

#include "neural_network.h"
#include "src/common/common.h"
#include "dynet_interface.h"
#include "neural_factory.h"
#include "src/common/types/instance.h"

class Network;
class LocalNetworkParam;
class GlobalNeuralNetworkParam{
public:
    GlobalNeuralNetworkParam();
    ~GlobalNeuralNetworkParam();
    void SetLearningState();
    void InitNetwork();
    std::vector<NeuralNetwork *> *GetNNVect();
    void Forward();
    void Backward();
    double GetNNScore(Network *ptr_network, int parent_k, int children_k_index);
    void SetNNGradientOutput(double count, Network *ptr_network, int parent_k, int children_k_index);
    void ResetAllNNGradient();
    void Initialization(std::vector<NeuralNetwork *> *ptr_nn_vec,int max_len,std::unordered_map<std::string,int> *ptr_word2int_map);
    void InitNNParameter(int &argc, char **&argv, unsigned random_seed = 0, bool shared_parameters = false);
    DynetFunctionHelper *GetDynetFunctionHelper();
    void SetLocalNetworkParams( LocalNetworkParam **pptr_param_l);
    void PrepareInputId();
    bool IsLearningState();
    int GetNeuralFeatureSize();
    void SetMaxSentenceLength(int max_len);
    int GetMaxSentenceLength(int netId);
    void SetInstance(std::vector<Instance *> *ptr_inst);
private:
    std::vector<NeuralNetwork *> *ptr_nn_vec_;
    LocalNetworkParam **pptr_param_l_;
    int param_size;
    //the neural net's internal weight and gradient;
    StatNLP::NeuralParameter *ptr_nn_param_;
    DynetFunctionHelper *ptr_call_dynet_;
    NeuralFactory *ptr_nf_;
    int neural_size_;
    int n_feature_size_;
    //std::vector<>
    ComType::Neural_Input_Map_Vect *ptr_all_NNInput2Id_ ;
};
#endif //STATNLP_GLOBAL_NN_PARAM_H
