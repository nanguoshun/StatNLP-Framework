//
// Created by  ngs on 18/10/2018.
//

#include "tree_crf_network_compiler.h"

TreeCRFNetworkCompiler::TreeCRFNetworkCompiler() {

}

TreeCRFNetworkCompiler::~TreeCRFNetworkCompiler() {

}

long TreeCRFNetworkCompiler::ToNodeRoot(int size) {

}

long TreeCRFNetworkCompiler::ToNode(int height, int index, int label_id) {

}

long TreeCRFNetworkCompiler::ToNodeSuperRoot(int height) {

}

long TreeCRFNetworkCompiler::ToNodeSink() {

}

TreeCRFNetwork* TreeCRFNetworkCompiler::CompileLabeled(int networkId, TreeCRFNetwork *ptr_inst,
                                                       LocalNetworkParam *ptr_param) {

}

void TreeCRFNetworkCompiler::CompileUnlabeledGenericRoot(int pos, std::vector<long> &cur_node_vec) {

}

Instance* TreeCRFNetworkCompiler::Decompile(Network *ptr_network) {

}