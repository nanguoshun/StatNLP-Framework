//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_CRF_COMPILER_H
#define STATNLP_TREE_CRF_COMPILER_H

#include "src/hybridnetworks/network_compiler.h"
#include "src/common/types/binarytree.h"
#include "tree_crf_network.h"
#include "tree_crf_instance.h"
#include "label.h"

enum NodeType{
    SINK,
    NODE,
    ROOT,
    SUPER_ROOT
};

class TreeCRFNetworkCompiler: public NetworkCompiler{
public:
    TreeCRFNetworkCompiler(std::vector<Label *> *ptr_label);
    ~TreeCRFNetworkCompiler();
    Network * Compile(int networkId, Instance* ptr_inst, LocalNetworkParam *ptr_param);
    Instance *Decompile(Network *ptr_network);
    void CompileUnlabeledGeneric();
    void CompileUnlabeledGenericNode(int pos,std::vector<long> &children_vec, std::vector<long>& cur_node_vec);
    void CompileUnlabeledGenericRoot(int pos,std::vector<long>& cur_node_vec);
    long ToNode(int height, int index, int label_id);
    long ToNode(int height, int index, int label_id, NodeType type);
    long ToNodeRoot(int height);
    long ToNodeSuperRoot(int height);
    long ToNodeSink();
    TreeCRFNetwork* CompileUnlabeled(int networkId, TreeCRFInstance *ptr_inst, LocalNetworkParam *ptr_param);
    TreeCRFNetwork* CompileLabeled(int networkId, TreeCRFInstance *ptr_inst, LocalNetworkParam *ptr_param);
    long CompileLabeledHelper(TreeCRFNetwork *ptr_network, BinaryTree *ptr_tree, int start);
private:
    int max_len_; /*max lenght unlabeled netwwork*/
    std::vector<Label *> *ptr_label_;
    TreeCRFNetwork *ptr_generic_network_;
    //contains node Id
    long *ptr_all_nodes_;
    //parent index, HyperEdge no(children), Nodes index
    int *** ptr_all_children_;

};
#endif //STATNLP_TREE_CRF_COMPILER_H
