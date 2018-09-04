//
// Created by  ngs on 03/09/2018.
//

#ifndef STATNLP_TAG_BILSTM_H
#define STATNLP_TAG_BILSTM_H
#include <src/neural/neural_network.h>
class LinearCRFTagBiLSTM: public NeuralNetwork{
public:
    LinearCRFTagBiLSTM();
    ~LinearCRFTagBiLSTM();
};

#endif //STATNLP_TAG_BILSTM_H
