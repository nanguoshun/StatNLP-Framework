//
// Created by  ngs on 04/09/2018.
//

#ifndef STATNLP_NEURAL_FACTOR_H
#define STATNLP_NEURAL_FACTOR_H

#include "neural_network.h"
#include "lstm_network.h"
#include <iostream>
#include <vector>
#include "dynet_interface.h"
class LSTMFactory;
class CNNFactory;
class NeuralFactory{
public:
    NeuralFactory();
    ~NeuralFactory();
    static LSTMFactory *GetLSTMFactory();
    static CNNFactory *GetCNNFactory();
    virtual std::vector<NeuralNetwork *> *GetNeuralInst() = 0;
    virtual NeuralNetwork* CreateNN() = 0;
    void SetDynetCallFunctionHelper(DynetFunctionHelper *ptr_helper);
    virtual void InitNNParameter(int &argc, char **&argv, int vocab_size, unsigned random_seed = 0, bool shared_parameters = false) = 0;
protected:
    DynetFunctionHelper *ptr_call_dynet_;
};

#endif //STATNLP_NEURAL_FACTOR_H
