//
// Created by  ngs on 31/08/2018.
//
#ifndef STATNLP_NEURAL_LAYER_H
#define STATNLP_NEURAL_LAYER_H
#include "../../dynet/dynet/init.h"
class Network;
namespace neural_layer{
    enum NeuralType{
        LSTM,CNN
    };
}

class NeuralNetwork{
public:
    NeuralNetwork();
    ~NeuralNetwork();
    void Initialize(int &argc, char **&argv, unsigned random_seed = 0, bool shared_parameters = false);
    void Initialize(dynet::DynetParams& params);
    int GetFeatureSize();
    void AddParam();
    virtual void Touch();
    virtual void Forward() = 0;
    //back propogation;
    virtual void BackWard();
    //update the model
    virtual void Update(double count, Network *ptr_network, int parent_k, int children_k_index);
    void SetTraining(bool istraining);
    double GetL2Param();
    void ResetGrad();
    double GetScore(Network *ptr_network, int parent_k, int childeren_k_index);

protected:
    //double *ptr_counts_;
    //double *ptr_weights_;
    bool is_training_;
    int argc_;
    char **argv_;
    double *ptr_params_, *ptr_gradparams_;
    int param_size_;
    double *ptr_output_;
    double *ptr_output_counts_;
    //the output size;
    int output_size_;
    //the dynet
};

#endif //STATNLP_NEURAL_LAYER_H
