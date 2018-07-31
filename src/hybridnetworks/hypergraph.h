//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_HPERGRAPTH_H
#define STATNLP_HPERGRAPTH_H

/**
 *  Pure virtual functions to define a hypergraph.
 */

#include <jmorecfg.h>

class HyperGraph {
public:
    virtual int CountNodes() = 0;
    virtual long GetNode(int k) = 0;
    virtual int* GetNodeArray(int k) = 0;
    virtual int ** GetChild(int k) = 0;
    virtual bool IsRemovded(int k) = 0;
    virtual void Remove(int k) = 0;
    virtual bool IsRoot(int k) = 0;
    virtual bool IsLeaf(int k) = 0;
    virtual bool IsContain(long node) = 0;
};


#endif //STATNLP_HPERGRAPTH_H
