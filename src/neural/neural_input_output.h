//
// Created by  ngs on 19/09/2018.
//

#ifndef STATNLP_NUERAL_INPUT_OUTPUT_H
#define STATNLP_NUERAL_INPUT_OUTPUT_H

#include <iostream>
#include "../common/common.h"
#include <boost/any.hpp>
class NeuralIO{
public:
    NeuralIO();
    NeuralIO(ComType::Neural_Input *ptr_input, int output);
    ComType::Neural_Input* GetInput();
    int GetOutput();
    void SetInput(ComType::Neural_Input *ptr_input);
    void SetOutput(int output);
private:
    ComType::Neural_Input* ptr_input_;
    int output_;
};

#endif //STATNLP_NUERAL_INPUT_OUTPUT_H