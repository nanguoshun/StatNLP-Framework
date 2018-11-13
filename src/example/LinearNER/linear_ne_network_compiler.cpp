//
// Created by  ngs on 05/10/2018.
//

#include "linear_ne_network_compiler.h"
#include <boost/algorithm/string.hpp>

LinearNENetworkCompiler::LinearNENetworkCompiler(bool is_useIOBE, std::vector<Entity *> *ptr_entity) {
    std::vector<int> capvec{150,50,3};
    NetworkIDManager::SetCapacity(capvec);
    max_len_ = capvec[0];
    is_use_IOBE_ = is_useIOBE;
    ptr_entity_ = ptr_entity;
    ptr_entity_id_map_ = new std::unordered_map<Entity *, int>;
    for (int i = 0; i < ptr_entity_->size(); ++i) {
        ptr_entity_id_map_->insert(std::make_pair((*ptr_entity)[i], i));
    }
    num_of_edge_ = 0;
    CompileUnlabeledGeneric();
    std::cout << "nodes: "<<ptr_generic_network_->CountNodes()<<std::endl;
    std::cout <<"num of edge is: "<<num_of_edge_<<std::endl;
}

LinearNENetworkCompiler::~LinearNENetworkCompiler() {

}

Network *LinearNENetworkCompiler::Compile(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param) {
    LinearNEInstance *ptr_crf_inst = (LinearNEInstance *) ptr_inst;
    if (ptr_crf_inst->IS_Labeled()) {
        return this->CompileLabeled(networkId, ptr_crf_inst, ptr_param);
    } else {
        return this->CompileUnlabeled(networkId, ptr_crf_inst, ptr_param);
    }
}

Instance *LinearNENetworkCompiler::Decompile(Network *ptr_network) {
    LinearNENetwork *ptr_ne_network = (LinearNENetwork *)ptr_network;
    LinearNEInstance *ptr_inst = (LinearNEInstance*)ptr_ne_network->GetInstance();
    int sent_len = ptr_inst->GetSize();
    /*the index of node in the array*/
    int node_idx = NetworkIDManager::BinarySearch(ptr_all_nodes_,ptr_generic_network_->CountNodes(),ToNodeRoot(sent_len));
    for(int i = 0; i < ptr_generic_network_->CountNodes(); ++i){
        //std::cout << i<<"th node is: "<<ptr_all_nodes_[i]<<std::endl;
    }
    NER_OutPut_Type *ptr_pred = new NER_OutPut_Type(sent_len); /*the pointer will be released by BaseInstance class*/
    //for(auto it = ptr_entity_->begin(); it != ptr_entity_->end(); ++it){
      //  std::cout << "the entity is "<<(*it)->GetForm() <<std::endl;
    //}
    int max = EntityReader::max_len_;
    for(int i = sent_len-1; i >=0; --i){
        int child_idx = ptr_ne_network->GetMaxPath(node_idx)[0];
        long child_id = ptr_ne_network->GetNode(child_idx);
        int tag_id = NetworkIDManager::ToHybridNodeArray(child_id)[1];
        std::string entity_str = (*ptr_entity_)[tag_id]->GetForm();
        /* to align with IOB tagging scheme */
        if(boost::starts_with(entity_str,"S-")){
            entity_str = "B-"+entity_str.substr(2);
        }
        if(boost::starts_with(entity_str,"E-")){
            entity_str = "I-"+entity_str.substr(2);
        }
        if(0 == tag_id || this->ptr_entity_->size() == tag_id){
            entity_str = "O";
        }
        (*ptr_pred)[i] = entity_str;
        node_idx = child_idx;
    }
    LinearNEInstance *ptr_inst_du = new LinearNEInstance(ptr_inst->GetInstanceId(),ptr_inst->GetWeight(),ptr_inst->GetInput(),ptr_inst->GetOutPut());
    ptr_inst_du->SetPrediction(ptr_pred);
    return  ptr_inst_du;
}


LinearNENetwork *LinearNENetworkCompiler::CompileLabeled(int networkId, LinearNEInstance *ptr_inst,
                                                         LocalNetworkParam *ptr_param) {
    LinearNENetwork *ptr_network = new LinearNENetwork(networkId, ptr_inst, ptr_param);
    int inst_size = ptr_inst->GetSize();
    long leaf = ToNodeLeaf();
    std::vector<long> prev_nodes_vec;
    prev_nodes_vec.push_back(leaf);
    ptr_network->AddNode(leaf);
    //the golden path from beginning to the end.
    for (int i = 0; i < inst_size; ++i) {
        std::string entity_str = (*(ptr_inst->GetOutPut()))[i];
        Entity *ptr_et = Entity::Get(entity_str);
        int tag_id = ptr_entity_id_map_->find(ptr_et)->second;
        long nodeId = ToNode(i, tag_id);
        ptr_network->AddNode(nodeId);
        ptr_network->AddEdge(nodeId, prev_nodes_vec);
        prev_nodes_vec.clear();
        prev_nodes_vec.push_back(nodeId);
    }
    //add root
    long root = ToNodeRoot(inst_size);
    ptr_network->AddNode(root);
    ptr_network->AddEdge(root, prev_nodes_vec);
    ptr_network->FinalizeNetwork();
    return ptr_network;
}

LinearNENetwork *LinearNENetworkCompiler::CompileUnlabeled(int networkId, LinearNEInstance *ptr_inst,
                                                           LocalNetworkParam *ptr_param) {
    int size = ptr_inst->GetSize();
    long root = this->ToNodeRoot(size);
    int pos = 0;
    //FIXME: this finding code in an array can be further optimized by binary search.
    int all_node_size = ptr_generic_network_->CountNodes();
    for (int i = 0; i < all_node_size; ++i) {
        //std::cout <<"node value is:"<<std::endl;
        if (root == ptr_all_nodes_[i]) {
            pos = i;
            break;
        }
    }
    int num_nodes = pos + 1;
    //this pointer will be stored and manageed in local_network_learner_thread.
    int *childrens_size = ptr_generic_network_->GetChildrens_Size();
    int **children_size = ptr_generic_network_->GetChildren_Size();
    LinearNENetwork *ptr_linear_crf_network = new LinearNENetwork(networkId, ptr_inst, this->ptr_all_nodes_,
                                                                  this->ptr_all_children_, childrens_size,
                                                                  children_size, ptr_param, num_nodes);
    return ptr_linear_crf_network;

}

long LinearNENetworkCompiler::ToNode(int pos, int tag_id) {
    int arr[] = {pos, tag_id, ComType::NODE_TYPES::NODE};
    std::vector<int> vec(std::begin(arr), std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

long LinearNENetworkCompiler::ToNodeLeaf() {
    int arr[] = {0, 0, ComType::NODE_TYPES::LEAF};
    std::vector<int> vec(std::begin(arr), std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

long LinearNENetworkCompiler::ToNodeRoot(int size) {
    int arr[] = {size-1, (int) this->ptr_entity_->size()-1, ComType::NODE_TYPES::ROOT};
    std::vector<int> vec(std::begin(arr), std::end(arr));
    return NetworkIDManager::ToHybridNodeID(vec);
}

/***
 *
 * Build generic graph.
 *
 */
void LinearNENetworkCompiler::CompileUnlabeledGeneric() {
    ptr_generic_network_ = new LinearNENetwork();
    long leaf = ToNodeLeaf();
    ptr_generic_network_->AddNode(leaf);
    std::vector<long> children_vec;
    children_vec.push_back(leaf);
    std::vector<long> cur_node_vec;
    for (int pos = 0; pos < max_len_; ++pos) {
        /* for the leaf node */
        /* for each label */
        CompileUnlabeledGenericNode(pos, children_vec, cur_node_vec);
        /* root */
        CompileUnlabeledGenericRoot(pos, cur_node_vec);
        children_vec = cur_node_vec;
        cur_node_vec.clear();
    }
    ptr_generic_network_->FinalizeNetwork();
    this->ptr_all_nodes_ = ptr_generic_network_->GetAllNodes();
    this->ptr_all_children_ = ptr_generic_network_->GetAllChildren();
}

/***
 *
 * build a graph for all nodes, except leaf and root nodes.
 *
 * @param pos : pos in graph, same as the position in a sentence.
 * @param children_vec: vector that stores the children node id
 * @param cur_node_vec: vector that stores parent node id.
 *
 */
void LinearNENetworkCompiler::CompileUnlabeledGenericNode(int pos, std::vector<long> &children_vec,
                                                          std::vector<long> &cur_node_vec) {
    int labels_length = ptr_entity_->size();
    for (int i = 0; i < labels_length; ++i) {
        long parent_node_id = ToNode(pos, i);
        if(16 == parent_node_id){
            std::cout << "the node id is 16"<<std::endl;
        }
        std::string entity_cur_str = (*ptr_entity_)[i]->GetForm();
        /* for each child */
        for (auto it = children_vec.begin(); it != children_vec.end(); ++it) {
            long child_id = (*it);
            if (-1 == child_id) continue;
            std::vector<int> child_id_array = NetworkIDManager::ToHybridNodeArray(child_id);
            std::string entity_child_str = 0 == pos? "O":(*ptr_entity_)[child_id_array[1]]->GetForm();
            if (0 == i || labels_length -1 == i) { continue; } /*ignore the START_TAG and END_TAG*/
            /*child is B- and I-, parent can be I- and E-*/
            if ((boost::starts_with(entity_child_str, "B-") || boost::starts_with(entity_child_str, "I-")) &&
                (boost::starts_with(entity_cur_str, "I-") || boost::starts_with(entity_cur_str, "E-")) &&
                (0 == entity_child_str.substr(2).compare(entity_cur_str.substr(2)))) {
                if (ptr_generic_network_->IsContain(child_id)) {
                    std::vector<long> vec;
                    vec.push_back(child_id);
                    ptr_generic_network_->AddNode(parent_node_id);
                    ptr_generic_network_->AddEdge(parent_node_id, vec);/* build a hyper-edge */
                    num_of_edge_ ++;
                }
                /*child is S-, E- and O, parent can be B-, S- and O*/
            } else if ((boost::starts_with(entity_child_str, "S-") || boost::starts_with(entity_child_str, "E-") ||
                        0 == entity_child_str.compare("O")) &&
                       (boost::starts_with(entity_cur_str, "B-") || boost::starts_with(entity_cur_str, "S-") ||
                        0 == entity_cur_str.compare("O"))) {
                if (ptr_generic_network_->IsContain(child_id)) {
                    std::vector<long> vec;
                    vec.push_back(child_id);
                    ptr_generic_network_->AddNode(parent_node_id);
                    ptr_generic_network_->AddEdge(parent_node_id, vec);
                    num_of_edge_++;
                }
            }
        }
        /*if the parent node is selected, then */
        if (ptr_generic_network_->IsContain(parent_node_id)) {
            cur_node_vec.push_back(parent_node_id);
            //std::cout << "node id is: "<<parent_node_id<<std::endl;
        } else {
            cur_node_vec.push_back(-1);
            //std::cout << "node id is: "<<-1<<std::endl;
        }
    }
}

/***
 *
 * Build the graph for root node. We have a root node for each position.
 *
 * @param pos : pos in graph, same as the position in a sentence.
 * @param cur_node_vec: vector that stores parent node id.
 *
 */
void LinearNENetworkCompiler::CompileUnlabeledGenericRoot(int pos, std::vector<long> &cur_node_vec) {
//    std::cout << "---------"<<std::endl;
    int labels_length = ptr_entity_->size();
    long root_id = ToNodeRoot(pos + 1);
    ptr_generic_network_->AddNode(root_id);
    for (auto it = cur_node_vec.begin(); it != cur_node_vec.end(); ++it) {
        long child_id = (*it);
        if(-1 == child_id) continue;
        std::vector<int> vec = NetworkIDManager::ToHybridNodeArray(child_id);
        std::string entity_child_str = (*ptr_entity_)[vec[1]]->GetForm();
        //FIXME: why vec[1], should be deleted ???
        if (false == boost::starts_with(entity_child_str, "B-") && false == boost::starts_with(entity_child_str, "I-")
            && 0 != vec[1] && labels_length - 1 != vec[1]) { /* the last two conditions are START_TAG and END_TAG*/
            std::vector<long> child_vec;
            child_vec.push_back(child_id);
            ptr_generic_network_->AddEdge(root_id, child_vec);
            num_of_edge_++;
        }
    }
}

