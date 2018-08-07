//
// Created by  ngs on 01/08/2018.
//
#ifndef STATNLP_TABLE_LOOKUP_NETWORK_H
#define STATNLP_TABLE_LOOKUP_NETWORK_H

#include <unordered_map>
#include <list>
#include "network.h"
#include <vector>

class TableLookupNetwork: public Network{
public:
    TableLookupNetwork();
    ~TableLookupNetwork();
    bool AddNode(long node);
    void AddEdge(long parent, std::vector<long> &children);
    void CheckLinkValidity(long parent, std::vector<long> &children);
    void CheckNodeValidity(long node);
    void FinalizeNetwork();
    std::vector<long> *GetAllNodes();
    std::vector<std::vector<std::vector<int>>> *GetAllChildren();
protected:
    //the node Id and its notes in a hyperedge.
    std::unordered_map<long, std::list<long*>> *ptr_children_tmp_;
    //the node Id.
    std::vector<long> *ptr_nodes_;
    //parent idx, hyper-edge, children nodes.
    std::vector<std::vector<std::vector<int>>> *ptr_children_;
//    std::unordered_map<long, std::list<long>> *ptr_children_tmp_;
};

#endif //STATNLP_TABLE_LOOKUP_NETWORK_H
