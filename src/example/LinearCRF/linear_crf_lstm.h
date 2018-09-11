//
// Created by  ngs on 03/09/2018.
//

#ifndef STATNLP_TAG_BILSTM_H
#define STATNLP_TAG_BILSTM_H

#include <src/neural/lstm_network.h>

class LinearCRFLSTMLanuageModel: public LSTMNetwork{
public:
    LinearCRFLSTMLanuageModel();
    ~LinearCRFLSTMLanuageModel();
};

#endif //STATNLP_TAG_BILSTM_H
