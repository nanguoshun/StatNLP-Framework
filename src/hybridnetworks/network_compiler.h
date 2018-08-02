//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_NETWORK_COMPILER_H
#define STATNLP_NETWORK_COMPILER_H

#include "network.h"
#include "../common/types/instance.h"
#include "local_network_param.h"
#include "../example/linear_crf_network.h"

class NetworkCompiler{
public:
    NetworkCompiler();
    ~NetworkCompiler();
    virtual LinearCRFNetwork * Compile(int networkId, Instance &inst, LocalNetworkParam &param) = 0;
    virtual Instance *Decompile(Network &network) = 0;
};

#endif //STATNLP_NETWORK_COMPILER_H
