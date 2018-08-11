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

    for(int i =0; i<node_size_; ++i){
        for(int j=0; j<ptr_childrens_size_[i]; ++j){
            for(int k=0; i<)
            ptr_children_[i][j]
        }
        delete ptr_children_[i];
    }

    delete ptr_children_tmp_;
    delete ptr_children_;
    delete ptr_nodes_;
    delete ptr_childrens_size_;
}

bool TableLookupNetwork::AddNode(long nodeId) {
    if(ptr_children_tmp_->find(nodeId) != ptr_children_tmp_->end()){
        return false;
    }
    //note that null_list is meaningless.
    //FIXME:
    std::list<std::vector<long>> null_list;
    std::vector<long> null;
    null_list.push_back(null);
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

long *** TableLookupNetwork::GetAllChildren() {
    return this->ptr_children_;
}

std::unordered_map<long, int>* TableLookupNetwork::FinalizeNodes() {
    node_size_ = ptr_children_tmp_->size();
    ptr_nodes_ = new long[node_size_];
    ptr_childrens_size_ = new int[node_size_];
    ptr_children_ = new long **[node_size_];
    std::vector<long> *ptr_node_vector_temp = new std::vector<long>;
    auto end = ptr_children_tmp_->end();
    for(auto it = ptr_children_tmp_->begin(); it!=end; ++it){
        ptr_node_vector_temp->push_back((*it).first);
    }
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
    auto end = ptr_children_tmp_->end();
    for(auto it = ptr_children_tmp_->begin(); it!=end; ++it){
        long parent = (*it).first;
        //get the parent index;
        int parent_index = ptr_nodeId2Index_map_tmp->find(parent)->second;
        //get all hypereges for a parent;
        std::list<std::vector<long>> childrens = (*it).second;
        if(childrens.empty()){
            ptr_children_[parent_index] = NULL;
        }else{
            int childrens_size = childrens.size();
            ptr_childrens_size_[parent_index] = childrens_size;
            //allocate the space for all hyper-edges rooted by node parent.
            ptr_children_[parent_index] = new long*[ptr_childrens_size_[parent_index]];

            auto list_end = (*it).second.end();
            //k is the index of hyperedge.
            int k = 0;
            for(auto itt = childrens.begin(); itt!=childrens.end(); ++itt){
                std::vector<long> children_vec = (*itt);
                for(auto ittt = (*it).second.begin(); ittt != list_end; ++ittt){
                    int children_size = (*ittt).size();
                    //ptr_children_[parent_index][k][]
                }
                k++;
            }
        }
    }
    //release the memory;
    delete ptr_nodeId2Index_map_tmp;
}




