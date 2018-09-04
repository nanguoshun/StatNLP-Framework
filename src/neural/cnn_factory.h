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

protected:

};
#endif //STATNLP_CNN_FACTORY_H
