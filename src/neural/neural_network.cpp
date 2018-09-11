//
// Created by  ngs on 31/08/2018.
//

#include <cstring>
#include "src/common/opt/math_calc.h"
#include "neural_network.h"

NeuralNetwork::NeuralNetwork() {
    is_training_ = false;
    ptr_params_ = nullptr;
    ptr_gradparams_ = nullptr;
    ptr_output_ = nullptr;
    ptr_output_counts_ = nullptr;

}

NeuralNetwork::~NeuralNetwork() {

}

void NeuralNetwork::Initialize(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    //dynet::initialize(argc,argv);
}

void NeuralNetwork::Initialize(dynet::DynetParams& params) {
    Initialize(params);
}

void NeuralNetwork::AddParam() {

}

int NeuralNetwork::GetFeatureSize() {
    return output_size_;
}

void NeuralNetwork::Touch() {

}

/*
void NeuralNetwork::Forward() {
    //call back the forward function
}*/

void NeuralNetwork::BackWard() {
    //call the back propagation of NN.
}

/**
 * Update the gradient of the output.
 *
 * @param count
 * @param ptr_network
 * @param parent_k
 * @param children_k_index
 */
void NeuralNetwork::Update(double count, Network *ptr_network, int parent_k, int children_k_index) {
    int idex = 0;
    ptr_output_counts_[idex] -= count;
}

void NeuralNetwork::SetTraining(bool istraining) {
    is_training_ = istraining;
}
/**
 * Get the L2 param
 * @return
 */
double NeuralNetwork::GetL2Param() {
    if(nullptr != ptr_params_){
        return MathCalc::SquareVector(ptr_params_,param_size_);
    } else{
        return 0.0;
    }
}

/**
 * Reset all gradient to zero.
 */
void NeuralNetwork::ResetGrad() {
    //reset the gradient of features that used in CRF
    if(nullptr != ptr_output_counts_){
        memset(ptr_output_counts_,0, sizeof(double) * output_size_);
    }
    //reset the neural internal gradient.
    if(nullptr != ptr_gradparams_){
        memset(ptr_gradparams_,0, sizeof(double) * param_size_);
    }
}

/**
 *
 * Get the neural score of a hyper-edge.
 *
 * @param ptr_network
 * @param parent_k: the parent node of the hyperedge
 * @param childeren_k_index: the  childeren_k_index th hyperedge.
 * @return: the score
 */
double NeuralNetwork::GetScore(Network *ptr_network, int parent_k, int childeren_k_index) {
    double score = 0;
}