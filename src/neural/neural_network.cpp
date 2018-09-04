//
// Created by  ngs on 31/08/2018.
//

#include "neural_network.h"

NeuralNetwork::NeuralNetwork() {
    is_training_ = false;
}

NeuralNetwork::~NeuralNetwork() {

}

void NeuralNetwork::Initialize(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    cnn::Initialize(argc,argv);
}

void NeuralNetwork::Initialize() {
    Initialize(argc_, argv_);
}

void NeuralNetwork::AddParam() {

}

int NeuralNetwork::GetFeatureSize() {
    return feature_size_;
}

void NeuralNetwork::Touch() {

}

void NeuralNetwork::Forward() {

}

void NeuralNetwork::BackWard() {

}

void NeuralNetwork::Update() {

}

void NeuralNetwork::SetTraining(bool istraining) {
    is_training_ = istraining;
}