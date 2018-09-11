//
// Created by  ngs on 11/09/2018.
//

#include "dynet_interface.h"

CallDynetFunction::CallDynetFunction() {
}

CallDynetFunction::~CallDynetFunction() {

}

void CallDynetFunction::Initialize(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    auto dyparams = dynet::extract_dynet_params(argc, argv);
    dynet::initialize(dyparams);
}

void CallDynetFunction::SetParameters(int &argc, char **&argv) {
    get_args(argc, argv, params_, TRAIN);
}

SuperNNParameter CallDynetFunction::GetParam() {
    return params_;
}

