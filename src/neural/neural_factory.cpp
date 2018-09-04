//
// Created by  ngs on 04/09/2018.
//

#include "neural_factory.h"
#include "lstm_factory.h"

NeuralFactory::NeuralFactory() {
    //NeuralFactory::ptr_nn_vec_ = new std::vector<NeuralNetwork*>;
}

NeuralFactory::~NeuralFactory() {

}

LSTMFactory* NeuralFactory::GetLSTMFactory() {
    return new LSTMFactory();
}