//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_LINEAR_NEWORK_COMPILER_H
#define STATNLP_LINEAR_NEWORK_COMPILER_H

#include "../hybridnetworks/network_compiler.h"
#include "linear_crf_instance.h"

class LinearCRFNetworkCompiler: public NetworkCompiler {
public:
    LinearCRFNetworkCompiler();
    ~LinearCRFNetworkCompiler();
    LinearCRFNetworkCompiler Compile(int networkId, Instance inst, LocalNetworkParam param);
    LinearCRFInstance Decomple(Network network);
private:

};
#endif //STATNLP_LINEAR_NEWORK_COMPILER_H
