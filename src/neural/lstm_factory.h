//
// Created by  ngs on 04/09/2018.
//

#ifndef STATNLP_LSTM_FACTORY_H
#define STATNLP_LSTM_FACTORY_H
#include "neural_factory.h"
#include "dynet_interface.h"

class LSTMFactory: NeuralFactory{
public:
    LSTMFactory();
    ~LSTMFactory();
    std::vector<NeuralNetwork*> *GetNeuralInst() override;
    NeuralNetwork *CreateNN() override;
    void InitNNParameter(int &argc, char **&argv, int vocab_size, unsigned random_seed = 0, bool shared_parameters = false) override;
private:
    std::vector<NeuralNetwork*> *ptr_nn_vec_;
    LSTMSuperParam param_;
    StatNLP::ParamCollection *ptr_model_;
};

#endif //STATNLP_LSTM_FACTORY_H
