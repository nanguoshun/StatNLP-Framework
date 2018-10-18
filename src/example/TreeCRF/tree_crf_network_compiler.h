//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_CRF_COMPILER_H
#define STATNLP_TREE_CRF_COMPILER_H

#include "src/hybridnetworks/network_compiler.h"
#include "tree_crf_network.h"
class TreeCRFNetworkCompiler: public NetworkCompiler{
public:
    TreeCRFNetworkCompiler();
    ~TreeCRFNetworkCompiler();
    Network * Compile(int networkId, Instance* ptr_inst, LocalNetworkParam *ptr_param);
    Instance *Decompile(Network *ptr_network);
    void CompileUnlabeledGeneric();
    void CompileUnlabeledGenericNode(int pos,std::vector<long> &children_vec, std::vector<long>& cur_node_vec);
    void CompileUnlabeledGenericRoot(int pos,std::vector<long>& cur_node_vec);
    long ToNode(int height, int index, int label_id);
    long ToNodeRoot(int height);
    long ToNodeSuperRoot(int height);
    long ToNodeSink();
    TreeCRFNetwork* CompileUnlabeled(int networkId, TreeCRFNetwork *ptr_inst, LocalNetworkParam *ptr_param);
    TreeCRFNetwork* CompileLabeled(int networkId, TreeCRFNetwork *ptr_inst, LocalNetworkParam *ptr_param);
};
#endif //STATNLP_TREE_CRF_COMPILER_H
