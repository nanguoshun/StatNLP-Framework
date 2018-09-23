//
// Created by  ngs on 11/09/2018.
//

#include "dynet_interface.h"

DynetFunctionHelper::DynetFunctionHelper() {

}

DynetFunctionHelper::~DynetFunctionHelper() {

}

void DynetFunctionHelper::Initialize(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    auto dyparams = dynet::extract_dynet_params(argc, argv);
    dynet::initialize(dyparams);
}

void DynetFunctionHelper::SetParameters(int &argc, char **&argv) {
    get_args(argc, argv, params_, TRAIN);
}

StatNLP::SuperNNParameter DynetFunctionHelper::GetParam() {
    return params_;
}

dynet::ComputationGraph * DynetFunctionHelper::NewGraph(dynet::ComputationGraph *ptr_cg) {
    if(nullptr != ptr_cg){
        delete ptr_cg;
    }
    return new dynet::ComputationGraph();
}
