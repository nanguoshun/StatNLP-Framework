//
// Created by  ngs on 01/09/2018.
//

#ifndef STATNLP_LSTM_H
#define STATNLP_LSTM_H

#include "neural_network.h"
#include "vanilla_rnn.h"

class LSTMNetwork: public VanillaRNN{
public:
    LSTMNetwork();
    ~LSTMNetwork();
    void Touch() override;
    void Forward() override;
    void BackWard() override;
    void Update() override;
};


#endif //STATNLP_LSTM_H
