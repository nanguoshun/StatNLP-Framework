//
// Created by  ngs on 01/08/2018.
//

#include <vector>
#include "linear_crf_nework_compiler.h"
#include "../hybridnetworks/common.h"
#include "../hybridnetworks/network_id_manager.h"


LinearCRFNetworkCompiler::LinearCRFNetworkCompiler(std::vector<std::string> &labels) {
    this->labels_ = labels;
    int i = 0;
    for(std::string label: labels){
        labels_id_map_.insert(std::make_pair(label,i));
        ++i;
    }
    this->CompileUnlabeledGeneric();
}

LinearCRFNetworkCompiler::~LinearCRFNetworkCompiler() {
    //first release the node resource

    //the delete the network
    delete ptr_generic_network_;
}

LinearCRFNetwork* LinearCRFNetworkCompiler::Compile(int networkId, Instance* ptr_inst, LocalNetworkParam *ptr_param) {
    LinearCRFInstance* ptr_crf_inst = (LinearCRFInstance*)ptr_inst;
    if(ptr_crf_inst->IS_Labeled()){
        return this->CompileLabeled(networkId,ptr_crf_inst,ptr_param);
    } else{
        return this->CompileUnlabeled(networkId,ptr_crf_inst,ptr_param);
    }
}

/**
 * Get the node ID.
 * @param pos
 * @param tag_id
 * @return
 */
long LinearCRFNetworkCompiler::ToNode(int pos, int tag_id) {
    int arr[] = {pos+1, tag_id, 0, 0, ComType::NODE_TYPES::NODE};
    std::vector<int> vec(std::begin(arr),std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

long LinearCRFNetworkCompiler::ToNodeRoot(int size) {
    int arr[] = {size,(int)this->labels_.size(),0,0,ComType::NODE_TYPES::ROOT};
    std::vector<int> vec(std::begin(arr),std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

long LinearCRFNetworkCompiler::ToNodeLeaf() {
    int arr[] = {0,0,0,0,ComType::NODE_TYPES::LEAF};
    std::vector<int> vec(std::begin(arr),std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

/**
 *  Build the generic network
 */

void LinearCRFNetworkCompiler::CompileUnlabeledGeneric() {
    ptr_generic_network_ = new LinearCRFNetwork();
    long leaf = this->ToNodeLeaf();
    ptr_generic_network_->AddNode(leaf);
    std::list<long> prev_nodes_list, curr_nodes_list;
    prev_nodes_list.push_back(leaf);
    for(int k=0; k< ComParam::MAX_LENGTH; ++k){
        //build edges among pre nodes and current nodes.
        for(int tag_id = 0; tag_id < this->labels_.size(); ++tag_id){
            long node_id = this->ToNode(k,tag_id);
            curr_nodes_list.push_back(node_id);
            ptr_generic_network_->AddNode(node_id);
            //build edges between a current node and each prev node,
            for(auto it = prev_nodes_list.begin(); it!=prev_nodes_list.end(); ++it){
                //pre_node_vec indicates all child nodes of a hyperedge.
                std::vector<long> pre_node_vec;
                pre_node_vec.push_back((*it));
                //add a hyperedge, for tree-based CRF, the vector pre_node contains multiple child nodes.
                ptr_generic_network_->AddEdge(node_id,pre_node_vec);
            }
        }
        //build a root for each row.
        prev_nodes_list = curr_nodes_list;
        std::list<long> root_node_list;
        curr_nodes_list = root_node_list;
        long root_id = this->ToNodeRoot(k+1);
        ptr_generic_network_->AddNode(root_id);
        //build edges among nodes and root.
        for(auto it = prev_nodes_list.begin(); it!= prev_nodes_list.end(); ++it){
            std::vector<long> pre_node_vec; //FIXME: this line has running error.
            pre_node_vec.push_back((*it));
            ptr_generic_network_->AddEdge(root_id,pre_node_vec);
        }
    }
    ptr_generic_network_->FinalizeNetwork();
    this->ptr_all_nodes_ = ptr_generic_network_->GetAllNodes();
    this->ptr_all_children_ = ptr_generic_network_->GetAllChildren();
}

LinearCRFNetwork* LinearCRFNetworkCompiler::CompileUnlabeled(int networkId, LinearCRFInstance *ptr_inst,
                                                             LocalNetworkParam *ptr_param) {
    int size = ptr_inst->GetSize();
    long root = this->ToNodeRoot(size);
    int pos  = 0;
    //FIXME: this finding code in an array can be further optimized by binary search.
    int all_node_size = ptr_generic_network_->CountNodes();
    for(int i=0; i< all_node_size; ++i){
        //std::cout <<"node value is:"<<std::endl;
        if(root == ptr_all_nodes_[i]){
            pos = i;
            break;
        }
    }
    int num_nodes = pos + 1;
    //this pointer will be stored and manageed in local_network_learner_thread.
    int *childrens_size = ptr_generic_network_->GetChildrens_Size();
    int **children_size = ptr_generic_network_->GetChildren_Size();

    LinearCRFNetwork *ptr_linear_crf_network = new LinearCRFNetwork(networkId,ptr_inst, this->ptr_all_nodes_, this->ptr_all_children_, childrens_size, children_size,ptr_param,num_nodes);

    return ptr_linear_crf_network;
}

LinearCRFNetwork* LinearCRFNetworkCompiler::CompileLabeled(int networkId, LinearCRFInstance *ptr_inst,
                                                               LocalNetworkParam *ptr_param) {
    LinearCRFNetwork *ptr_network = new LinearCRFNetwork(networkId,ptr_inst,ptr_param);
    //FIXME: link error when call the GetOuput Function, need further analyze the cause.
    ComType::Label_Str_Vector *ptr_output = ptr_inst->GetOutPut();
    int size = ptr_output->size();
    //Add Leaf
    long leaf = ToNodeLeaf();
    ptr_network->AddNode(leaf);
    std::vector<long> prev_nodes_vec;
    prev_nodes_vec.push_back(leaf);
    auto it = ptr_output->begin();
    //the golden path from beginning to the end.
    for(int i=0; i < ptr_output->size(); ++i){
        std::string label_str = (*it);
        long nodeId = ToNode(i,labels_id_map_.find(label_str)->second);
        ptr_network->AddNode(nodeId);
        ptr_network->AddEdge(nodeId,prev_nodes_vec);
        prev_nodes_vec.clear();
        prev_nodes_vec.push_back(nodeId);
        ++it;
    }
    //add root
    long root = ToNodeRoot(ptr_output->size());
    ptr_network->AddNode(root);
    ptr_network->AddEdge(root,prev_nodes_vec);
    ptr_network->FinalizeNetwork();
    return ptr_network;
}

/**
 * Backtracking to get the predictions.
 * @param ptr_network
 * @return
 */
LinearCRFInstance* LinearCRFNetworkCompiler::Decompile(Network *ptr_network) {
    int num_node = ptr_network->CountNodes();
    LinearCRFInstance *ptr_inst = (LinearCRFInstance *) ptr_network->GetInstance();
    std::vector<std::string> *ptr_prediction = new std::vector<std::string>(num_node);
    for(int nodeid = num_node - 1; nodeid >=0; --nodeid){
        int* ptr_nodes = ptr_network->GetPath(nodeid);
        int child_k = ptr_nodes[0];
        long node = ptr_network->GetNode(child_k);
        std::vector<int> array = NetworkIDManager::ToHybridNodeArray(node);
        int pos = array[0] - 1;
        int tag_id = array[1];
        (*ptr_prediction)[num_node-1] = labels_[tag_id];

        if(pos != nodeid){
            std::cerr << "Error:the position encoded in the node not the same as the interpretation! @LinearCRFNetworkCompiler::Decompile"<<std::endl;
        }
    }

    LinearCRFInstance *ptr_inst_du = new LinearCRFInstance(ptr_inst->GetInstanceId(),ptr_inst->GetWeight(),ptr_inst->GetInput(),ptr_inst->GetOutPut());
    ptr_inst_du->SetPrediction(ptr_prediction);
    return  ptr_inst_du;
}