//
// Created by  ngs on 18/10/2018.
//

#include "tree_crf_network_compiler.h"

TreeCRFNetworkCompiler::TreeCRFNetworkCompiler(std::vector<Label *> *ptr_label) {
    num_of_edge_ = 0;
    ptr_label_ = ptr_label;
    max_len_ = 100;
    CompileUnlabeledGeneric();
    std::cout << "num of edges is "<<num_of_edge_<<std::endl;
    std::cout << "num of nodes is "<<ptr_generic_network_->CountNodes()<<std::endl;
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
    /**/
    long node = CompileLabeledHelper(ptr_network, ptr_tree->GetRootNode(), 0);

    /*add root node and edge*/
    std::vector<long> prev_nodes_vec;
    prev_nodes_vec.push_back(node);
    long root = ToNodeRoot(size - 1);
    ptr_network->AddNode(root);
    ptr_network->AddEdge(root, prev_nodes_vec);

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
 *
 */
long TreeCRFNetworkCompiler::CompileLabeledHelper(TreeCRFNetwork *ptr_network, Node *ptr_node, int start) {
    int height = ptr_node->GetHeight();
    int index = start;
    std::string label_str = ptr_node->GetData();
    int label_id =  Label::Get(label_str,false,false)->GetId();
    long node_id = ToNode(height, index,label_id);
    ptr_network->AddNode(node_id);
    Node *ptr_left_node = ptr_node->GetLeftNode();
    Node *ptr_right_node = ptr_node->GetRightNode();
    if (nullptr != ptr_left_node){
        std::vector<long> vec;
        int left_size = ptr_node->GetLeftNode()->GetHeight() + 1;
        long left_node_id = CompileLabeledHelper(ptr_network, ptr_left_node, start);
        vec.push_back(left_node_id);
        if(nullptr != ptr_right_node){ /* if right node exists */
            long right_node_id = CompileLabeledHelper(ptr_network, ptr_right_node, start + left_size );
            vec.push_back(right_node_id);
        }
        ptr_network->AddEdge(node_id, vec);
    } else {
        std::vector<long> vec;
        long sink = ToNodeSink();
        ptr_network->AddNode(sink);
        vec.push_back(sink);
        ptr_network->AddEdge(node_id, vec);
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
TreeCRFNetwork *TreeCRFNetworkCompiler::CompileUnlabeled(int networkId, TreeCRFInstance *ptr_inst,
                                                         LocalNetworkParam *ptr_param) {
    int size = ptr_inst->GetSize();
    long root = ToNodeRoot(size - 1);
    int all_node_size = ptr_generic_network_->CountNodes();
    //get the index of root node in generic unlabeled network.
    int pos = NetworkIDManager::BinarySearch(ptr_all_nodes_, all_node_size, root);
    //total number of nodes in the network.
    int num_nodes = pos + 1;
    int *childrens_size = ptr_generic_network_->GetChildrens_Size();
    int **children_size = ptr_generic_network_->GetChildren_Size();
    TreeCRFNetwork *ptr_tree_crf_network = new TreeCRFNetwork(networkId, ptr_inst, this->ptr_all_nodes_,
                                                              this->ptr_all_children_, childrens_size,
                                                              children_size, ptr_param, num_nodes);
    return ptr_tree_crf_network;
}

/**
 *
 * build a generic network from bottom to top.
 *
 */
void TreeCRFNetworkCompiler::CompileUnlabeledGeneric() {
    ptr_generic_network_ = new TreeCRFNetwork();
    long sink = ToNodeSink(); /*only one sink node for a graph*/
    ptr_generic_network_->AddNode(sink);
    for (int col = 0; col < max_len_; ++col) {
        for (int row = col; row >= 0; --row) {
            int height = col - row; // the width of the span, and it equals to the height. The height increaes from 0 to col.
            int index = row; // index represents the position.
            if (0 == height) {  // terminator label.
                CompileUnlabeledGenericTerminator(ptr_generic_network_,height,index,sink);
            } else {  //non-terminator label
                CompileUnlabeledGenericNonTerminator(ptr_generic_network_, height,index);
                if(0 == index){
                    CompileUnlabeledGenericRoot(ptr_generic_network_,height);
                }
            }
        }
    }
    long super_root = ToNodeSuperRoot(max_len_);
    ptr_generic_network_->AddNode(super_root);
    for(int height = 1; height < max_len_; ++height){
        long root = ToNodeRoot(height);
        std::vector<long> vec{root};
        ptr_generic_network_->AddEdge(super_root,vec);
        num_of_edge_++;
    }
    ptr_generic_network_->FinalizeNetwork();

}


void TreeCRFNetworkCompiler::CompileUnlabeledGenericNonTerminator(TreeCRFNetwork *ptr_network, int height, int index) {
    for (auto it = ptr_label_->begin(); it != ptr_label_->end(); ++it) {
/*        if(0 == (*it)->GetForm().compare("PRN")){
            std::cout << "PRN"<<std::endl;
        }*/
        if (LabelType::NON_TERMINAL != (*it)->GetType()) continue;
        std::string label_str = (*it)->GetForm();
        std::set<CFG_Rule> *ptr_rule_set = CFGRule::ptr_rule_label_map_->find((*it))->second;
        if(nullptr == ptr_rule_set) {
            std::cerr << "Error: the rule is not existed in the ptr_rule_label_map"<<std::endl;
            continue;
        }
        int label_id = (*it)->GetId();
        long node_id = ToNode(height,index,label_id);
        ptr_network->AddNode(node_id);
        /* */
        for(int childIdx = 0; childIdx < height; ++childIdx){
            int right1_height = childIdx;
            int right1_index = index;
            int right2_height = height-1-childIdx;
            int right2_index = index+1+childIdx;
            for(auto itt = ptr_rule_set->begin(); itt != ptr_rule_set->end(); ++itt){
//                int right1_label_id = (*itt)->GetFirstRight()->GetId();
//                int right2_label_id = (*itt)->GetSecondRight()->GetId();
                int right1_label_id = Label::Get((*itt).second.first,false)->GetId();
                int right2_label_id = Label::Get((*itt).second.second,false)->GetId();
                long right1_node_id = ToNode(right1_height,right1_index,right1_label_id);
                if(false == ptr_network->IsContain(right1_node_id)) continue;
                long right2_node_id = ToNode(right2_height,right2_index,right2_label_id);
                if(false == ptr_network->IsContain(right2_node_id)) continue;
                std::vector<long> vec{right1_node_id,right2_node_id};
                ptr_network->AddEdge(node_id,vec);
                num_of_edge_ ++;
            }
        }
    }
}

void TreeCRFNetworkCompiler::CompileUnlabeledGenericTerminator(TreeCRFNetwork *ptr_network,int height,int index,long sink) {
    std::vector<long> vec;
    vec.push_back(sink);
    int size = ptr_label_->size();
    for (auto it = ptr_label_->begin(); it != ptr_label_->end(); ++it) {
        //for test only
        std::string label_str = (*it)->GetForm();
        int label_id_ = (*it)->GetId();
        int type = (*it)->GetType();
        //above code is for test only
        if (LabelType::TERMINAL != (*it)->GetType()) continue;
        int label_id = (*it)->GetId();
        long leaf_node_id = ToNode(height,index,label_id);
        ptr_network->AddNode(leaf_node_id);
        ptr_network->AddEdge(leaf_node_id,vec); /*the edge between terminator and sink*/
        num_of_edge_ ++;
    }
    std::cout << "udpate: num of edge is " <<num_of_edge_<<std::endl;
}

void TreeCRFNetworkCompiler::CompileUnlabeledGenericRoot(TreeCRFNetwork *ptr_network, int height) {
    long root = ToNodeRoot(height);
    ptr_network->AddNode(root);
    for(auto it = ptr_label_->begin(); it != ptr_label_->end(); ++it){
        std::vector<long> vec;
        if(LabelType::NON_TERMINAL != (*it)->GetType()) continue;
        long node_id = ToNode(height, 0, (*it)->GetId());
        vec.push_back(node_id);
        ptr_network->AddEdge(root,vec);
        num_of_edge_ ++;
    }
}

Network *TreeCRFNetworkCompiler::Compile(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param) {
    TreeCRFInstance *ptr_tree_crf_inst = (TreeCRFInstance*) ptr_inst;
    if(ptr_tree_crf_inst->IS_Labeled()){
        return CompileLabeled(networkId,ptr_tree_crf_inst,ptr_param);
    } else{
        return CompileUnlabeled(networkId,ptr_tree_crf_inst,ptr_param);
    }
}

Instance *TreeCRFNetworkCompiler::Decompile(Network *ptr_network) {
    TreeCRFNetwork *ptr_tree_crf_network = (TreeCRFNetwork *) ptr_network;
    TreeCRFInstance *ptr_inst = (TreeCRFInstance *) ptr_network->GetInstance();
    int root_k = ptr_tree_crf_network->CountNodes() - 1;
    BinaryTree *ptr_predict = DecompileHelper(ptr_tree_crf_network,root_k);
    ptr_inst->SetPrediction(ptr_predict);
    return ptr_inst;
}

BinaryTree* TreeCRFNetworkCompiler::DecompileHelper(TreeCRFNetwork *ptr_network, int parent_k) {
    //ptr_network->GetMaxPath();
}