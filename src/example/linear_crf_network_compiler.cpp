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

LinearCRFNetwork* LinearCRFNetworkCompiler::Compile(int networkId, Instance &instance, LocalNetworkParam &param) {
    Instance *ptr_inst = &instance;
    LinearCRFInstance* ptr_crf_inst = (LinearCRFInstance*)ptr_inst;
    if(ptr_crf_inst->IS_Labeled()){
        //to be done
    } else{
        //to be done
    }
}

Instance* LinearCRFNetworkCompiler::Decompile(Network &network) {

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

LinearCRFInstance LinearCRFNetworkCompiler::CompileUnlabeled(int networkId, LinearCRFInstance *ptr_inst,
                                                             LocalNetworkParam *ptr_param) {
    int size = ptr_inst->Size();
    long root = this->ToNodeRoot(size);
    //to be done
}

LinearCRFInstance LinearCRFNetworkCompiler::CompileLabeled(int networkId, LinearCRFInstance *ptr_inst,

                                                               LocalNetworkParam *ptr_param) {
    //to be done

}