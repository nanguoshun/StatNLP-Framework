//
// Created by  ngs on 01/08/2018.
//

#include <vector>
#include "linear_crf_nework_compiler.h"
#include "../hybridnetworks/common.h"
#include "../hybridnetworks/network_id_manager.h"


LinearCRFNetworkCompiler::LinearCRFNetworkCompiler(std::list<std::string> &labels) {
    this->labels_ = labels;
    int i = 0;
    for(std::string label: labels){
        labels_id_map_.insert(std::make_pair(label,i));
        ++i;
    }
    ptr_network_ = new LinearCRFNetwork();
    this->CompileUnlabeledGeneric();
}

LinearCRFNetworkCompiler::~LinearCRFNetworkCompiler() {
    //first release the node resource

    //the delete the network
    delete ptr_network_;
}

LinearCRFNetwork* LinearCRFNetworkCompiler::Compile(int networkId, Instance* ptr_inst, LocalNetworkParam *ptr_param) {
    LinearCRFInstance* ptr_crf_inst = (LinearCRFInstance*)ptr_inst;
    if(ptr_crf_inst->IS_Labeled()){
        return this->CompileLabeled(networkId,ptr_crf_inst,ptr_param);
    } else{
        return this->CompileUnlabeled(networkId,ptr_crf_inst,ptr_param);
    }
}

Instance* LinearCRFNetworkCompiler::Decompile(Network &network) {
    //TODO: will be done after finished the training part.
}

/**
 * Get the node ID.
 * @param pos
 * @param tag_id
 * @return
 */
long LinearCRFNetworkCompiler::ToNode(int pos, int tag_id) {
    int arr[] = {pos+1, tag_id, 0, 0, NODE_TYPES::NODE};
    std::vector<int> vec(std::begin(arr),std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

long LinearCRFNetworkCompiler::ToNodeRoot(int size) {
    int arr[] = {size,(int)this->labels_.size(),0,0,NODE_TYPES::ROOT};
    std::vector<int> vec(std::begin(arr),std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

long LinearCRFNetworkCompiler::ToNodeLeaf() {
    int arr[] = {0,0,0,0,NODE_TYPES::LEAF};
    std::vector<int> vec(std::begin(arr),std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

/**
 *  Build the generic network
 */
void LinearCRFNetworkCompiler::CompileUnlabeledGeneric() {
    long leaf = this->ToNodeLeaf();
    ptr_network_->AddNode(leaf);
    std::list<long> prev_nodes, curr_nodes;
    prev_nodes.push_back(leaf);
    for(int k=0; k< MAX_LENGTH; ++k){
        //build edges among pre nodes and current nodes.
        for(int tag_id = 0; tag_id < this->labels_.size(); ++tag_id){
            long node = this->ToNode(k,tag_id);
            curr_nodes.push_back(node);
            ptr_network_->AddNode(node);
            for(auto it = prev_nodes.begin(); it!=prev_nodes.end(); ++it){
                std::vector<long> pre_node((*it));
                ptr_network_->AddEdge(node,pre_node);
            }
        }
        //build a root for each row.
        prev_nodes = curr_nodes;
        std::list<long> root_node;
        curr_nodes = root_node;
        long root = this->ToNodeRoot(k+1);
        ptr_network_->AddNode(root);
        //build edges among nodes and root.
        for(auto it = prev_nodes.begin(); it!= prev_nodes.end(); ++it){
            std::vector<long> pre_node((*it)) ;
            ptr_network_->AddEdge(root,pre_node);
        }
    }
    ptr_network_->FinalizeNetwork();
    this->ptr_all_nodes_ = ptr_network_->GetAllNodes();
    this->ptr_all_children_ = ptr_network_->GetAllChildren();
}

LinearCRFNetwork* LinearCRFNetworkCompiler::CompileUnlabeled(int networkId, LinearCRFInstance *ptr_inst,
                                                             LocalNetworkParam *ptr_param) {
    int size = ptr_inst->Size();
    long root = this->ToNodeRoot(size);
    int pos  = 0;
    //FIXME: this finding code in an array can be further optimized by binary search.
    for(int i=0; i< size; ++i){
        if(root == ptr_all_nodes_[i]){
            pos = i;
        }
    }
    int num_nodes = pos + 1;
    //this pointer will be stored and manageed in local_network_learner_thread.
    LinearCRFNetwork *ptr_linear_crf_network = new LinearCRFNetwork(networkId,ptr_inst,this->ptr_all_nodes_, this->ptr_all_children_,ptr_param,num_nodes);
    return ptr_linear_crf_network;

}

LinearCRFNetwork* LinearCRFNetworkCompiler::CompileLabeled(int networkId, LinearCRFInstance *ptr_inst,
                                                               LocalNetworkParam *ptr_param) {
    LinearCRFNetwork *ptr_network = new LinearCRFNetwork(networkId,ptr_inst,ptr_param);

    //FIXME:
    //Label_Str_Vector *ptr_output = ptr_inst->GetOutPut();

    //Add Leaf
    long leaf = ToNodeLeaf();
    ptr_network->AddNode(leaf);
    long prevNode = leaf;
    std::vector<long> prev_nodes_vec;
    prev_nodes_vec.push_back(leaf);
    int pos = 0;
    for(auto it = labels_.begin(); it!=labels_.end(); ++it){
        std::string label_str = (*it);
        long nodeId = ToNode(pos,labels_id_map_.find(label_str)->second);
        ptr_network->AddNode(nodeId);
        ptr_network->AddEdge(nodeId,prev_nodes_vec);
        prev_nodes_vec.clear();
        prev_nodes_vec.push_back(nodeId);
    }

    //add root
    long root = ToNodeRoot(labels_.size());
    ptr_network->AddNode(root);
    ptr_network->AddEdge(root,prev_nodes_vec);

    ptr_network->FinalizeNetwork();
    return ptr_network;

}