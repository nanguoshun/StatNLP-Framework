//
// Created by  ngs on 04/09/2018.
//

#include "lstm_factory.h"

LSTMFactory::LSTMFactory() {
    //create neural network instances here.
}

LSTMFactory::~LSTMFactory() {

}

void LSTMFactory::InitNNParameter(int &argc, char **&argv, int vocab_size, int label_size, unsigned random_seed, bool shared_parameters) {
    ptr_call_dynet_->Initialize(argc,argv,random_seed,shared_parameters);
    ptr_call_dynet_->SetParameters(argc,argv);
    StatNLP::SuperNNParameter param = ptr_call_dynet_->GetParam();
    param_.layers_ = param.LAYERS;
    param_.hidden_dim_ = param.HIDDEN_DIM;
    param_.input_dim_ = param.INPUT_DIM;
    param_.dropout_rate_ = param.dropout_rate;
    param_.decay_rate_ = param.eta_decay_rate;
    param_.vocab_size_ = vocab_size;
    param_.label_size_ = label_size;
}

std::vector<NeuralNetwork *>* LSTMFactory::GetNeuralInst() {
    return ptr_nn_vec_;
}

NeuralNetwork *LSTMFactory::CreateNN() {
    ptr_nn_vec_ = new std::vector<NeuralNetwork*>;
    LSTMNetwork *ptr_lstm = new LSTMNetwork(param_);
    ptr_nn_vec_->push_back(ptr_lstm);
}
