//
// Created by  ngs on 31/08/2018.
//
#ifndef STATNLP_NEURAL_LAYER_H
#define STATNLP_NEURAL_LAYER_H
#include "../../cnn/cnn/init.h"

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
    void Initialize();
    int GetFeatureSize();
    void AddParam();
    virtual void Touch();
    virtual void Forward();
    //back propogation;
    virtual void BackWard();
    //update the model
    virtual void Update();
    void SetTraining(bool istraining);
private:
    //the neural feature size;
    int feature_size_;
    double *ptr_counts_;
    double *ptr_weights_;
    bool is_training_;
    int argc_;
    char **argv_;
};

#endif //STATNLP_NEURAL_LAYER_H
