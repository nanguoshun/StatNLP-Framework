//
// Created by  ngs on 06/10/2018.
//

#ifndef STATNLP_SENTENCE_H
#define STATNLP_SENTENCE_H

#include "token_array.h"
#include "word_token.h"
#include "dependency_tree.h"
#include <iostream>
#include <vector>

class Sentence: public TokenArray{
public:
    inline Sentence(WordToken **pptr_tokens, int word_token_size):TokenArray((Token**)pptr_tokens,word_token_size){
        pptr_tokens_ = pptr_tokens;
        ptr_str_vec_ = nullptr;
    }
    /*release the allocated sentence*/
    inline ~Sentence(){
        for(int i = 0; i < length_; ++i){
            delete pptr_tokens_[i];
        }
        delete pptr_tokens_;
        if(nullptr != ptr_str_vec_){
            delete ptr_str_vec_;
            ptr_str_vec_ = nullptr;
        }
    }
    inline WordToken *Get(int index) override {
        return (WordToken*)(pptr_tokens_[index]);
    }
    /* get the overall sentence string */
    inline std::vector<std::string> * GetSentence(){
        if(nullptr == ptr_str_vec_){
            ptr_str_vec_ = new std::vector<std::string>;
            for(int i = 0; i < length_; ++i){
                ptr_str_vec_->push_back(pptr_tokens_[i]->GetForm());
            }
            return ptr_str_vec_;
        } else{
            return ptr_str_vec_;
        }
    }
private:
    std::vector<std::string> *ptr_str_vec_;
    DependencyTree *ptr_dtree_;
    WordToken **pptr_tokens_;
};
#endif //STATNLP_SENTENCE_H
