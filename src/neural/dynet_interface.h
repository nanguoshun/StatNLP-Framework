//
// Created by  ngs on 11/09/2018.
//

#ifndef STATNLP_DYNET_INTERFACE_H
#define STATNLP_DYNET_INTERFACE_H

#include "dynet/dynet/model.h"
#include "dynet/dynet/expr.h"
#include "dynet/dynet/dynet.h"
#include "dynet/dynet/dict.h"
#include "dynet/dynet/tensor.h"
#include "dynet/examples/cpp-utils/cl-args.h"

struct LSTMSuperParam{
    unsigned vocab_size_;
    unsigned hidden_dim_;
    unsigned layers_;
    unsigned input_dim_;
    float decay_rate_;
    float dropout_rate_;
};
//typedef int Params;

namespace StatNLP{
    typedef dynet::Parameter NeuralParameter;
    typedef Params SuperNNParameter;
    typedef dynet::ParameterCollection ParamCollection;
}

class DynetFunctionHelper{
 public:
    DynetFunctionHelper();
    ~DynetFunctionHelper();
    void Initialize(int &argc, char **&argv, unsigned int random_seed,bool shared_parameters);
    void SetParameters(int &argc, char **&argv);
    StatNLP::SuperNNParameter GetParam();
private:
    StatNLP::SuperNNParameter params_;
};



#endif //STATNLP_DYNET_INTERFACE_H
