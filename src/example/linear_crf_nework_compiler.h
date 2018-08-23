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
    LinearCRFNetworkCompiler(std::vector<std::string> &labels);
    ~LinearCRFNetworkCompiler();
    LinearCRFNetwork* Compile(int networkId, Instance *ptr_instance, LocalNetworkParam *ptr_param);
    LinearCRFInstance * Decompile(Network *ptr_network);
    void CompileUnlabeledGeneric();
    long ToNode(int pos, int tag_id);
    long ToNodeRoot(int size);
    long ToNodeLeaf();
    LinearCRFNetwork* CompileUnlabeled(int networkId, LinearCRFInstance *ptr_inst, LocalNetworkParam *ptr_param);
    LinearCRFNetwork* CompileLabeled(int networkId, LinearCRFInstance *ptr_inst, LocalNetworkParam *ptr_param);

private:
    std::vector<std::string> labels_;
    std::map<std::string, int> labels_id_map_;
    LinearCRFNetwork *ptr_generic_network_;
    //contains node Id
    long *ptr_all_nodes_;
    //parent index, HyperEdge no(children), Nodes index
    int *** ptr_all_children_;
};
#endif //STATNLP_LINEAR_NEWORK_COMPILER_H
