//
// Created by  ngs on 06/10/2018.
//

#ifndef STATNLP_TOKEN_ARRAY_H
#define STATNLP_TOKEN_ARRAY_H

#include "linear_chain.h"

class TokenArray: public LinearChain{
public:
    inline TokenArray(Token **pptr_tokens, int size){
        pptr_tokens_ = pptr_tokens;
        length_ = size;
    }
    inline ~TokenArray(){
    }
    Token *Get(int index) override {
        return pptr_tokens_[index];
    }
    int GetLength() override {
        return length_;
    }

protected:
    Token **pptr_tokens_;
    int length_;
};
#endif //STATNLP_TOKEN_ARRAY_H
