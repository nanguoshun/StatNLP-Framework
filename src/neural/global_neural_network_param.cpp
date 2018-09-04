//
// Created by  ngs on 03/09/2018.
//

#include "global_neural_network_param.h"

GlobalNeuralNetworkParam::GlobalNeuralNetworkParam(std::vector<NeuralNetwork *> *ptr_nn_vec) {
    ptr_nn_vec_ = ptr_nn_vec;

}

GlobalNeuralNetworkParam::~GlobalNeuralNetworkParam() {

}

//set the learning state
void GlobalNeuralNetworkParam::SetLearningState() {
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        (*it)->SetTraining(true);
    }
}

void GlobalNeuralNetworkParam::InitNetwork() {
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        (*it)->Initialize();
    }
}
