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
    LinearCRFNetwork* Compile(int networkId, Instance &instance, LocalNetworkParam &param);
    Instance * Decompile(Network &network);
    void CompileUnlabeledGeneric();
    long ToNode(int pos, int tag_id);
    long ToNodeRoot(int size);
    long ToNodeLeaf();
    LinearCRFInstance CompileUnlabeled(int networkId, LinearCRFInstance *ptr_inst, LocalNetworkParam *ptr_param);
    LinearCRFInstance CompileLabeled(int networkId, LinearCRFInstance *ptr_inst, LocalNetworkParam *ptr_param);

private:
    std::list<std::string> labels_;
    std::map<std::string, int> labels_id_map_;
    LinearCRFNetwork *ptr_network_;
    std::vector<long> *ptr_nodes_;
    std::vector<std::vector<std::vector<int>>> *ptr_children_;
};
#endif //STATNLP_LINEAR_NEWORK_COMPILER_H
