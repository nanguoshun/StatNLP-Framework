//
// Created by  ngs on 06/10/2018.
//

#ifndef STATNLP_WORD_TOKEN_H
#define STATNLP_WORD_TOKEN_H

#include "token.h"
class WordToken:public Token{
public:
    inline WordToken(std::string name):Token(name){
        tag_ = "";
        head_index_ = -1;
        entity_ = "O";
        atag_ = "";
    }
    inline WordToken(std::string name, std::string tag, int head_index, std::string entity):Token(name){
        tag_ = tag;
        head_index_ = head_index;
        entity_ = entity;
        atag_ = tag.substr(0,1);
    }
    inline std::string GetTag(){
        return tag_;
    }
    inline std::string GetATag(){
        return atag_;
    }
    inline std::string GetEntity(){
        return entity_;
    }
    inline int GetHead(){
        return head_index_;
    }
    inline std::string GetLabel(){
        return del_label_;
    }
    inline void SetEntity(std::string entity){
        entity_ = entity;
    }
    inline void SetTag(std::string tag){
        tag_ = tag;
    }
    inline void SetHead(int head){
        head_index_ = head;
    }
private:
    std::string tag_;
    std::string atag_;
    int head_index_;
    std::string entity_;
    std::string del_label_;
};

#endif //STATNLP_WORD_TOKEN_H
