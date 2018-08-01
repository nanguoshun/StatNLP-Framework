//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_NETWORK_H
#define STATNLP_NETWORK_H

#include "hypergraph.h"
/**
 *  inside-outside algorithm.
 */
class Network: public HyperGraph{
public:
    Network();
    ~Network();
};
#endif //STATNLP_NETWORK_H
