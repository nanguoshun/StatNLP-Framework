//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_CRF_COMPILER_H
#define STATNLP_TREE_CRF_COMPILER_H

#include "src/hybridnetworks/network_compiler.h"
#include "binarytree.h"
#include "tree_crf_network.h"
#include "tree_crf_instance.h"
#include "label.h"
#include "rule.h"
#include <unordered_map>

#define TREE_DEBUG_;

class TreeCRFNetworkCompiler: public NetworkCompiler{
public:
    TreeCRFNetworkCompiler(std::vector<Label *> *ptr_label);
    ~TreeCRFNetworkCompiler();
    Network * Compile(int networkId, Instance* ptr_inst, LocalNetworkParam *ptr_param);
    Instance *Decompile(Network *ptr_network);
    Node *DecompileHelper(TreeCRFNetwork *ptr_network,int parent_k);
    void CompileUnlabeledGeneric();
    void CompileUnlabeledGenericTerminator(TreeCRFNetwork *ptr_network,int height, int index, long sink);
    void CompileUnlabeledGenericNonTerminator(TreeCRFNetwork *ptr_network, int height, int index);
    void CompileUnlabeledGenericRoot(TreeCRFNetwork *ptr_network, int height);
    long ToNode(int height, int index, int label_id);
    long ToNode(int height, int index, int label_id, NodeType type);
    long ToNodeRoot(int height);
    long ToNodeSuperRoot(int height);
    long ToNodeSink();
    TreeCRFNetwork* CompileUnlabeled(int networkId, TreeCRFInstance *ptr_inst, LocalNetworkParam *ptr_param);
    TreeCRFNetwork* CompileLabeled(int networkId, TreeCRFInstance *ptr_inst, LocalNetworkParam *ptr_param);
    long CompileLabeledHelper(TreeCRFNetwork *ptr_network, Node *ptr_tree, int start);

private:
    int max_len_; /*max length unlabeled network, actually is also indicates the height and the span of a graph*/
    std::vector<Label *> *ptr_label_;
    TreeCRFNetwork *ptr_generic_network_;
    //contains node Id
    long *ptr_all_nodes_;
    //parent index, HyperEdge no(children), Nodes index
    int *** ptr_all_children_;
    //for test only
    int num_of_edge_;
};
#endif //STATNLP_TREE_CRF_COMPILER_H
