//
// Created by  ngs on 01/08/2018.
//

#include <iostream>
#include "table_lookup_network.h"

TableLookupNetwork::TableLookupNetwork() {

    ptr_children_tmp_ = new std::unordered_map<long, std::list<std::vector<long>>>;
    //ptr_children_tmp_ = new std::unordered_map<long,std::list<long>>;
   // ptr_children_ = new std::vector<std::vector<std::vector<int>>>;
    //ptr_nodes_ = new std::vector<long>;
    //ptr_nodes_ = new int;
}

TableLookupNetwork::~TableLookupNetwork() {

    //delete ptr_children_temp.
    for(auto it = ptr_children_tmp_->begin(); it!=ptr_children_tmp_->end(); ++it){
        std::list<std::vector<long>> childrens_list = (*it).second;
        for(auto itt = childrens_list.begin(); itt != childrens_list.end(); ++itt){
            delete &(*itt);
        }
        delete &childrens_list;
    }

    for(int i =0; i < node_size_; ++i){
        int childrens_size = sizeof(ptr_children_[i]);
        for(int j=0; j < childrens_size; ++j){
                delete ptr_children_[i][j];
        }
        delete ptr_children_[i];
    }

    delete ptr_children_tmp_;
    delete ptr_children_;
    delete ptr_nodes_;
//    delete ptr_childrens_size_;
}

TableLookupNetwork::TableLookupNetwork(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param):Network(networkId,ptr_inst,ptr_param) {

}

TableLookupNetwork::TableLookupNetwork(int networkId, Instance *ptr_inst, long *ptr_nodes, int ***ptr_children,
                                       LocalNetworkParam *ptr_param):Network(networkId,ptr_inst,ptr_param) {
    this->ptr_nodes_ = ptr_nodes_;
    this->ptr_children_ = ptr_children;
}

bool TableLookupNetwork::AddNode(long nodeId) {
    if(ptr_children_tmp_->find(nodeId) != ptr_children_tmp_->end()){
        return false;
    }
    //note that null_list is meaningless.
    //FIXME:
    std::list<std::vector<long>> null_list;
    ptr_children_tmp_->insert(std::make_pair(nodeId,null_list));
    return true;
}

/**
 * Add a hyper-edge.
 *
 * @param parent
 * @param children
 */
void TableLookupNetwork::AddEdge(long parent, std::vector<long> &children) {
    this->CheckLinkValidity(parent,children);
    auto it = ptr_children_tmp_->find(parent);
    //the nodes in a hyperedge.
    std::vector<long> *ptr_children = new std::vector<long>;
    (*ptr_children) = children;
    //if the hyperedge rooted by parent is not found, create hash_map;
    if(it == ptr_children_tmp_->end()){
        std::list<std::vector<long>> *ptr_children_list = new std::list<std::vector<long>>;
        ptr_children_list->push_back((*ptr_children));
        ptr_children_tmp_->insert(std::make_pair(parent,(*ptr_children_list)));
    } else{
        //add this hyperedge to the list, which is root by parent.
        (*it).second.push_back((*ptr_children));
    }
}

void TableLookupNetwork::CheckLinkValidity(long parent, std::vector<long>& children) {
    for(int i=0, size = children.size(); i<size; ++i){
        if(children[i] >= parent){
            std::cerr << "This link seems to be invalid:"<<std::endl;
        }
    }
    CheckNodeValidity(parent);
    for(int i=0; i<children.size(); ++i){
        CheckNodeValidity(children[i]);
    }
}

void TableLookupNetwork::CheckNodeValidity(long node) {
    if(ptr_children_tmp_->find(node) == ptr_children_tmp_->end()){
        std::cerr << "This node seems to be invalid"<<std::endl;
    }
}

long * TableLookupNetwork::GetAllNodes() {
    return this->ptr_nodes_;
}

int *** TableLookupNetwork::GetAllChildren() {
    return this->ptr_children_;
}

std::unordered_map<long, int>* TableLookupNetwork::FinalizeNodes() {
    node_size_ = ptr_children_tmp_->size();
    ptr_nodes_ = new long[node_size_];
    //ptr_childrens_size_ = new int[node_size_];
    ptr_children_ = new int **[node_size_];
    std::vector<long> *ptr_node_vector_temp = new std::vector<long>;
    auto end = ptr_children_tmp_->end();
    for(auto it = ptr_children_tmp_->begin(); it!=end; ++it){
        ptr_node_vector_temp->push_back((*it).first);
    }
    //sort the Node ID in the vector.
    std::sort(ptr_node_vector_temp->begin(),ptr_node_vector_temp->end());
    //ptr_nodeId2Index_map_tmp: the fist element is the nodeID,and the second element is node index;
    std::unordered_map<long, int> *ptr_nodeId2Index_map_tmp = new std::unordered_map<long, int>;
    //save the node ID to ptr_nodes_ according to the node index.
    for(int i = 0; i < node_size_; ++i){
        long nodeId = (*ptr_node_vector_temp)[i];
        ptr_nodes_[i] = nodeId;
        ptr_nodeId2Index_map_tmp->insert(std::make_pair(nodeId,i));
    }
    //release the memory;
    delete ptr_node_vector_temp;
    return ptr_nodeId2Index_map_tmp;
}

void TableLookupNetwork::FinalizeNetwork() {
    std::unordered_map<long, int> * ptr_nodeId2Index_map_tmp = FinalizeNodes();
    int size = ptr_nodeId2Index_map_tmp->size();
    for(auto it = this->ptr_children_tmp_->begin(); it!=this->ptr_children_tmp_->end(); ++it){
        long parent = (*it).first;
        //get the parent index;
        int parent_index = ptr_nodeId2Index_map_tmp->find(parent)->second;
        //get all hypereges for a parent;
        std::list<std::vector<long>> childrens = (*it).second;
        if(childrens.empty()){
            this->ptr_children_[parent_index] = NULL;
        }else{
            //k is the index of the hyperedge rooted by node parent.
            this->ptr_children_[parent_index] = new int*[childrens.size()];
            int hyper_edge_no = 0;
            //childrens is a list, which stores the hyperege rooted by a parent note.
            //itt is a vector, which stores the nodes in a hpyeredge.
            for(auto itt = childrens.begin(); itt!=childrens.end(); ++itt){
                int *ptr_node_vector = new int[(*itt).size()];
                //ittt is a node ID.
                int node_no = 0;
                for(auto ittt = (*itt).begin(); ittt != (*itt).end(); ++ittt){
                    //find the node index
                    ptr_node_vector[node_no] = ptr_nodeId2Index_map_tmp->find((*ittt))->second;
                    node_no++;
                }
                this->ptr_children_[parent_index][hyper_edge_no] = ptr_node_vector;
                hyper_edge_no++;
            }
        }
    }
    //release the memory;
    delete ptr_nodeId2Index_map_tmp;
}

int TableLookupNetwork::CountNodes() {
    return 0;
}

long TableLookupNetwork::GetNode(int k) {
    return 0;
}

std::vector<int> *TableLookupNetwork::GetNodeArray(int k) {
    return nullptr;
}

int **TableLookupNetwork::GetChildren(int k) {
    return this->ptr_children_[k];
}

bool TableLookupNetwork::IsRemovded(int k) {
    return false;
}

void TableLookupNetwork::Remove(int k) {

}

bool TableLookupNetwork::IsRoot(int k) {
    return false;
}

bool TableLookupNetwork::IsLeaf(int k) {
    return false;
}

bool TableLookupNetwork::IsContain(long node) {
    return false;
}




