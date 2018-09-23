//
// Created by  ngs on 19/09/2018.
//

#include "neural_input_output.h"

NeuralIO::NeuralIO() {

}

NeuralIO::NeuralIO(ComType::Neural_Input *ptr_input, int output) {
    ptr_input_ = ptr_input;
    output_ = output;
}

ComType::Neural_Input* NeuralIO::GetInput() {
    return ptr_input_;
}

int NeuralIO::GetOutput() {
    return output_;
}

void NeuralIO::SetInput(ComType::Neural_Input *ptr_input) {
    ptr_input_ = ptr_input;
}

void NeuralIO::SetOutput(int output) {
    output_ = output;
}

/*
template <class T>
NeuralIO<T>::NeuralIO(T &input, int output) {
    ptr_input_ = input;
    output_ = output;
}

template <class T>
int NeuralIO<T>::GetOutput() {
    return output_;
}

template <class T>
T NeuralIO<T>::GetInput() {
    return ptr_input_;
}

template <class T>
void NeuralIO<T>::SetInput(T &input) {
    ptr_input_ = input;
}

template <class T>
void NeuralIO<T>::SetOutput(int output) {
    output_ = output;
}*/