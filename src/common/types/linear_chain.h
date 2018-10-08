//
// Created by  ngs on 06/10/2018.
//

#ifndef STATNLP_LINEAR_CHAIN_H
#define STATNLP_LINEAR_CHAIN_H

#include "token.h"

class LinearChain{
public:
    inline LinearChain(){};
    inline ~LinearChain(){};
    virtual Token *Get(int id) = 0;
    virtual int GetLength() = 0;
};

#endif //STATNLP_LINEAR_CHAIN_H
