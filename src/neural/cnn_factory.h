//
// Created by  ngs on 04/09/2018.
//

#ifndef STATNLP_CNN_FACTORY_H
#define STATNLP_CNN_FACTORY_H

#include "neural_factory.h"
class CNNFactory: public NeuralFactory{
public:
    CNNFactory();
    ~CNNFactory();
    std::vector<NeuralNetwork *> *GetNeuralInst() override;
    NeuralNetwork *CreateNN() override;

    void InitNNParameter(int &argc, char **&argv, int vocab_size, int label_size, unsigned int random_seed,
                         bool shared_parameters) override;

protected:

};
#endif //STATNLP_CNN_FACTORY_H
