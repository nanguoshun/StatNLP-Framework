//
// Created by  ngs on 01/09/2018.
//

#include "lstm_network.h"

LSTMNetwork::LSTMNetwork() {

}

LSTMNetwork::~LSTMNetwork() {

}

void LSTMNetwork::Touch() {
    NeuralNetwork::Touch();
}

void LSTMNetwork::Forward() {
    //call LSTM forward from dynet and get the output.
    ptr_output_ = nullptr; // get the output from LSTM output.
    output_size_ = 0; // should be a value.
    if(NeuralNetwork::is_training_){
        if(nullptr == ptr_output_counts_){
            ptr_output_counts_ = new double[output_size_];
            //todo:
        }
    }
}

void LSTMNetwork::BackWard() {
    NeuralNetwork::BackWard();
}

void LSTMNetwork::Update(double count, Network *ptr_network, int parent_k, int children_k_index) {
}
