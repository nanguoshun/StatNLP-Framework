//
// Created by  ngs on 04/09/2018.
//

#include "lstm_factory.h"

LSTMFactory::LSTMFactory() {
    //create neural network instances here.
    ptr_nn_vec_ = new std::vector<NeuralNetwork*>;
}

LSTMFactory::~LSTMFactory() {

}

std::vector<NeuralNetwork *>* LSTMFactory::GetNeuralInst() {
    return ptr_nn_vec_;
}
