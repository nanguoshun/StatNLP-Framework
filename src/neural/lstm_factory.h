//
// Created by  ngs on 04/09/2018.
//

#ifndef STATNLP_LSTM_FACTORY_H
#define STATNLP_LSTM_FACTORY_H

#include "neural_factory.h"

class LSTMFactory: NeuralFactory{
public:
    LSTMFactory();
    ~LSTMFactory();
    std::vector<NeuralNetwork*> *GetNeuralInst() override;
private:
    std::vector<NeuralNetwork*> *ptr_nn_vec_;
};

#endif //STATNLP_LSTM_FACTORY_H
