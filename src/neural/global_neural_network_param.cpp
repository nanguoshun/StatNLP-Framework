//
// Created by  ngs on 03/09/2018.
//

#include "global_neural_network_param.h"
#include "../common/opt/math_calc.h"

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

double GlobalNeuralNetworkParam::GetNNScore(Network *ptr_network, int parent_k, int children_k_index) {
    double score = 0;
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        score += (*it)->GetScore(ptr_network,parent_k,children_k_index);
    }
}

void GlobalNeuralNetworkParam::SetNNGradientOutput(double count, Network *ptr_network, int parent_k,
                                                   int children_k_index) {
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        //set the output of NN
        (*it)->Update(count,ptr_network,parent_k,children_k_index);
    }
}

void GlobalNeuralNetworkParam::ResetAllNNGradient() {
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        (*it)->ResetGrad();
    }
}

void GlobalNeuralNetworkParam::Forward() {
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        (*it)->Forward();
    }
}

void GlobalNeuralNetworkParam::Backward() {
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        (*it)->BackWard();
    }
}
