//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_LINEAR_NEWORK_COMPILER_H
#define STATNLP_LINEAR_NEWORK_COMPILER_H

#include <list>
#include <map>
#include <string>
#include "../hybridnetworks/network_compiler.h"
#include "linear_crf_instance.h"
#include "linear_crf_network.h"

class LinearCRFNetworkCompiler: public NetworkCompiler {
public:
    LinearCRFNetworkCompiler(std::list<std::string> &labels);
    ~LinearCRFNetworkCompiler();
    LinearCRFNetwork* Compile(int networkId, Instance inst, LocalNetworkParam param);
    Instance * Decompile(Network network);
    void CompileUnlabeledGeneric();
    long ToNode(int pos, int tag_id);
    long ToNodeRoot(int size);
    long ToNodeLeaf();
private:
    std::list<std::string> labels_;
    std::map<std::string, int> labels_id_map_;
    LinearCRFNetwork *ptr_network_;
};
#endif //STATNLP_LINEAR_NEWORK_COMPILER_H
