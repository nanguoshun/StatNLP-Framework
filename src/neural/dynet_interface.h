//
// Created by  ngs on 11/09/2018.
//

#ifndef STATNLP_DYNET_INTERFACE_H
#define STATNLP_DYNET_INTERFACE_H

#include "dynet/dynet/model.h"
#include "dynet/dynet/expr.h"
#include "dynet/dynet/dynet.h"
#include "dynet/dynet/dict.h"
#include "dynet/examples/cpp-utils/cl-args.h"

using namespace dynet;
typedef Parameter NeuralParameter;
typedef Params SuperNNParameter;

class CallDynetFunction{
 public:
    CallDynetFunction();
    ~CallDynetFunction();
    void Initialize(int &argc, char **&argv, unsigned int random_seed,bool shared_parameters);
    void SetParameters(int &argc, char **&argv);
    SuperNNParameter GetParam();
private:
    SuperNNParameter params_;
};



#endif //STATNLP_DYNET_INTERFACE_H
