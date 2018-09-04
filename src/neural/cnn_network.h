//
// Created by  ngs on 01/09/2018.
//

#ifndef STATNLP_CNN_NETWORK_H
#define STATNLP_CNN_NETWORK_H


#include "neural_network.h"

class CNN_Network: public NeuralNetwork {

public:
    CNN_Network();
    ~CNN_Network();

    void Touch() override;

    void Forward() override;

    void BackWard() override;

    void Update() override;

private:

};


#endif //STATNLP_CNN_NETWORK_H
