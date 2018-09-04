//
// Created by  ngs on 04/09/2018.
//

#ifndef STATNLP_NEURAL_FACTOR_H
#define STATNLP_NEURAL_FACTOR_H

#include "neural_network.h"
#include "lstm_network.h"
#include <iostream>
#include <vector>
class LSTMFactory;
class CNNFactory;
class NeuralFactory{
public:
    NeuralFactory();
    ~NeuralFactory();
    static LSTMFactory *GetLSTMFactory();
    static CNNFactory *GetCNNFactory();
    virtual std::vector<NeuralNetwork *> *GetNeuralInst() = 0;
protected:
    //std::vector<NeuralNetwork *> *ptr_nn_vec_;
};

#endif //STATNLP_NEURAL_FACTOR_H
