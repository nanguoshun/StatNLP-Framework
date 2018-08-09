//
// Created by  ngs on 01/08/2018.
//

#include <iostream>
#include "table_lookup_network.h"

TableLookupNetwork::TableLookupNetwork() {

    ptr_children_tmp_ = new std::unordered_map<long, std::list<long*>>;
   // ptr_children_tmp_ = new std::unordered_map<long,std::list<long>>;
    ptr_children_ = new std::vector<std::vector<std::vector<int>>>;
    ptr_nodes_ = new std::vector<long>;
}

TableLookupNetwork::~TableLookupNetwork() {

    //delete ptr_children_temp.
    for(auto it = ptr_children_tmp_->begin(); it!=ptr_children_tmp_->end(); ++it){
        std::list<long*> childrens_list = (*it).second;
        for(auto itt = childrens_list.begin(); itt != childrens_list.end(); ++itt){
            delete (*itt);
        }
        delete &childrens_list;
    }
    delete ptr_children_tmp_;
    delete ptr_children_;
    delete ptr_nodes_;
}

bool TableLookupNetwork::AddNode(long nodeId) {
    if(ptr_children_tmp_->find(nodeId) != ptr_children_tmp_->end()){
        return false;
    }
    //note that null_list is meaningless.
    std::list<long*> null_list;
    null_list.push_back(NULL);
    ptr_children_tmp_->insert(std::make_pair(nodeId,null_list));
    return true;
}

void TableLookupNetwork::AddEdge(long parent, std::vector<long> &children) {
    this->CheckLinkValidity(parent,children);
    if(ptr_children_tmp_->find(parent) == ptr_children_tmp_->end()){
//to be done
        std::list<long *> *ptr_children_list = new std::list<long *>;
        //TODO:
        //ptr_children_tmp_->insert(std::make_pair(parent,))
    }

//to be done
}

void TableLookupNetwork::CheckLinkValidity(long parent, std::vector<long>& children) {
    for(int i=0; i<children.size(); ++i){
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

std::vector<long>* TableLookupNetwork::GetAllNodes() {
    return this->ptr_nodes_;
}

std::vector<std::vector<std::vector<int>>>* TableLookupNetwork::GetAllChildren() {
    return this->ptr_children_;
}

void TableLookupNetwork::FinalizeNetwork() {
    //to be done...

}


