//
// Created by  ngs on 18/10/2018.
//

#include "tree_crf_network_compiler.h"

TreeCRFNetworkCompiler::TreeCRFNetworkCompiler(std::vector<Label *> *ptr_label) {
    ptr_label_ = ptr_label;
    std::vector<int> capvec{150, 50, 3};//????
    NetworkIDManager::SetCapacity(capvec);
    max_len_ = 100;
}

TreeCRFNetworkCompiler::~TreeCRFNetworkCompiler() {

}

long TreeCRFNetworkCompiler::ToNodeRoot(int height) {
    return ToNode(height, 0, ptr_label_->size(), NodeType::ROOT);
}

long TreeCRFNetworkCompiler::ToNode(int height, int index, int label_id) {
    return ToNode(height, index, label_id, NodeType::NODE);
}

long TreeCRFNetworkCompiler::ToNode(int height, int index, int label_id, NodeType type) {
    std::vector<int> vec{height + index, height, label_id, 0, type};
    return NetworkIDManager::ToHybridNodeID(vec);
}

long TreeCRFNetworkCompiler::ToNodeSuperRoot(int height) {
    return ToNode(height, 0, ptr_label_->size(), NodeType::SUPER_ROOT);
}

long TreeCRFNetworkCompiler::ToNodeSink() {
    return ToNode(0, 0, 0, NodeType::SINK);
}

TreeCRFNetwork *TreeCRFNetworkCompiler::CompileLabeled(int networkId, TreeCRFInstance *ptr_inst,
                                                       LocalNetworkParam *ptr_param) {
    TreeCRFNetwork *ptr_network = new TreeCRFNetwork(networkId, ptr_inst, ptr_param);

    BinaryTree *ptr_tree = ptr_inst->GetOutPut();
    int size = ptr_inst->GetSize();
    long node = CompileLabeledHelper(ptr_network,ptr_tree,0);

    /*add root node and edge*/
    std::vector<long> prev_nodes_vec;
    prev_nodes_vec.push_back(node);
    long root = ToNodeRoot(size - 1);
    ptr_network->AddNode(root);
    ptr_network->AddEdge(root,prev_nodes_vec);

    ptr_network->FinalizeNetwork();
    return ptr_network;
}
/**
 *
 * Build the labeled network recursively.
 *
 * @param ptr_network
 * @param ptr_tree
 * @param start
 * @return
 */
long TreeCRFNetworkCompiler::CompileLabeledHelper(TreeCRFNetwork *ptr_network, BinaryTree *ptr_tree, int start) {
    int height = ptr_tree->GetXVector()->size() - 1;
    int index = start;
    long node_id = ToNode(height,index, ptr_tree->GetRootNode()->GetId()); //todo: id should be set when initailize data.
    ptr_network->AddNode(node_id);
    if(nullptr != ptr_tree->LeftNode()){
        std::vector<long> vec;
        int left_size = ptr_tree->LeftTree()->GetXVector()->size();
        long left_node_id = CompileLabeledHelper(ptr_network, ptr_tree->LeftTree(),start);
        long right_node_id = CompileLabeledHelper(ptr_network, ptr_tree->RightTree(),start+left_size);
        vec.push_back(left_node_id);
        vec.push_back(right_node_id);
        ptr_network->AddEdge(node_id,vec);
    } else{
        std::vector<long> vec;
        long sink = ToNodeSink();
        ptr_network->AddNode(sink);
        vec.push_back(sink);
        ptr_network->AddEdge(node_id,vec);
    }
    return node_id;
}

/**
 * Compile unlabeled network
 *
 * @param networkId
 * @param ptr_inst
 * @param ptr_param
 * @return
 */
TreeCRFNetwork* TreeCRFNetworkCompiler::CompileUnlabeled(int networkId, TreeCRFInstance *ptr_inst,
                                                         LocalNetworkParam *ptr_param) {
    int size = ptr_inst->GetSize();
    long root = ToNodeRoot(size-1);
    int all_node_size = ptr_generic_network_->CountNodes();
    //get the index of root node in generetic unlabeled network.
    int pos = NetworkIDManager::BinarySearch(ptr_all_nodes_,all_node_size,root);
    //total number of nodes in the network.
    int num_nodes = pos + 1;
    int *childrens_size = ptr_generic_network_->GetChildrens_Size();
    int **children_size = ptr_generic_network_->GetChildren_Size();
    TreeCRFNetwork *ptr_tree_crf_network = new TreeCRFNetwork(networkId, ptr_inst, this->ptr_all_nodes_,
                                                                  this->ptr_all_children_, childrens_size,
                                                                  children_size, ptr_param, num_nodes);
    return ptr_tree_crf_network;


}

void TreeCRFNetworkCompiler::CompileUnlabeledGeneric() {
    long sink = ToNodeSink();
    TreeCRFNetwork *ptr_network = new TreeCRFNetwork();
    for (int col = 0; col < max_len_; ++col) {
        for(int row = col; row >=0; --row){
            int height = col - row;
            int index = row;
            if(0 == height){ // terminator
                for(auto it = ptr_label_->begin(); it != ptr_label_->end(); ++it){
                    if(LabelType::TERMINAL == (*it)->GetType()) continue;
                    int label_id = (*it)->GetId();

                }
            } else{ //non-terminator
                for(auto it = ptr_label_->begin(); it != ptr_label_->end(); ++it){

                }
            }
        }
    }
}

void TreeCRFNetworkCompiler::CompileUnlabeledGenericRoot(int pos, std::vector<long> &cur_node_vec) {

}

void TreeCRFNetworkCompiler::CompileUnlabeledGenericNode(int pos, std::vector<long> &children_vec,
                                                         std::vector<long> &cur_node_vec) {

}

Instance *TreeCRFNetworkCompiler::Decompile(Network *ptr_network) {

}

Network *TreeCRFNetworkCompiler::Compile(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param) {

}

