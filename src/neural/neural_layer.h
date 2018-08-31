//
// Created by  ngs on 31/08/2018.
//

#ifndef STATNLP_NEURAL_LAYER_H
#define STATNLP_NEURAL_LAYER_H
#include "../../cnn/cnn/init.h"

namespace neural_layer{
    enum NeuralType{
        LSTM,CNN
    };
}

class NeuralLayer{
public:
    NeuralLayer();
    ~NeuralLayer();
    void Init(int& argc, char**& argv, unsigned random_seed = 0, bool shared_parameters = false);

};

#endif //STATNLP_NEURAL_LAYER_H
