//
// Created by  ngs on 03/09/2018.
//

#include "global_neural_network_param.h"
#include "../common/opt/math_calc.h"

GlobalNeuralNetworkParam::GlobalNeuralNetworkParam() {
    ptr_call_dynet_ = new DynetFunctionHelper();
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
       // (*it)->Initialize();
    }
}

double GlobalNeuralNetworkParam::GetNNScore(Network *ptr_network, int parent_k, int children_k_index) {
    double score = 0;
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        score += (*it)->GetScore(ptr_network,parent_k,children_k_index);
    }
}

/**
 *
 * @param count: the gradient for the children_k_index th hyper-edge that is rooted by parent_k
 * @param ptr_network: the graphical network.
 * @param parent_k: the rooted node.
 * @param children_k_index: the no of the hyper-edge that is rooted by parent_k.
 */
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

void GlobalNeuralNetworkParam::SetNNVect(std::vector<NeuralNetwork *> *ptr_nn_vec) {
    ptr_nn_vec_ = ptr_nn_vec;
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

void GlobalNeuralNetworkParam::InitNNParameter(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    ptr_call_dynet_->Initialize(argc,argv,random_seed,shared_parameters);
}

DynetFunctionHelper* GlobalNeuralNetworkParam::GetDynetFunctionHelper() {
    return ptr_call_dynet_;
}