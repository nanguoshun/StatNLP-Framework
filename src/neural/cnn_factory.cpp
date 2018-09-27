//
// Created by  ngs on 04/09/2018.
//

#include "cnn_factory.h"

CNNFactory::CNNFactory() {

}

CNNFactory::~CNNFactory() {

}

NeuralNetwork *CNNFactory::CreateNN() {
    return nullptr;
}

void CNNFactory::InitNNParameter(int &argc, char **&argv, int vocab_size, int label_size, unsigned int random_seed,
                                 bool shared_parameters) {

}

std::vector<NeuralNetwork *> *CNNFactory::GetNeuralInst() {
    //todo:
    return nullptr;
}
