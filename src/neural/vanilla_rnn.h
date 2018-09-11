//
// Created by  ngs on 03/09/2018.
//

#ifndef STATNLP_VANILLA_RNN_H
#define STATNLP_VANILLA_RNN_H

#include "neural_network.h"

class VanillaRNN: public NeuralNetwork{
public:
    VanillaRNN();
    ~VanillaRNN();
    void Touch() override;
    void Forward();
    void BackWard() override;
    void Update(double count, Network *ptr_network, int parent_k, int children_k_index) override;
};

#endif //STATNLP_VANILLA_RNN_H
