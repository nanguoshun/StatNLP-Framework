//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_HPERGRAPTH_H
#define STATNLP_HPERGRAPTH_H

/**
 *  Pure virtual functions to define a hypergraph.
 */

#include <vector>
#include <iostream>

class HyperGraph {
public:
    virtual int CountNodes() = 0;
    virtual long GetNode(int k) = 0;
    virtual std::vector<int> GetNodeArray(int k) = 0;
    virtual int **GetChildren(int k) = 0;
    virtual bool IsRemovded(int k) = 0;
    virtual void Remove(int k) = 0;
    virtual bool IsRoot(int k) = 0;
    virtual bool IsLeaf(int k) = 0;
    virtual bool IsContain(long node) = 0;
    virtual int GetChildrens_Size(int node_index) = 0; // the number of hyperedge
    virtual int *GetChildren_Size(int node_index) = 0; // the number of hyperedge, the pointer points an array that stores the size of each hyperedge.
};


#endif //STATNLP_HPERGRAPTH_H
