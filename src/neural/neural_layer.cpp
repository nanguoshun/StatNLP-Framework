//
// Created by  ngs on 31/08/2018.
//

#include "neural_layer.h"

NeuralLayer::NeuralLayer() {

}

NeuralLayer::~NeuralLayer() {

}

void NeuralLayer::Init(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    cnn::Initialize(argc,argv);
}