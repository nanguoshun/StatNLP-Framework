//
// Created by  ngs on 06/10/2018.
//

#ifndef STATNLP_TOKEN_H
#define STATNLP_TOKEN_H

#include <iostream>
class Token{
public:
    inline Token(std::string name){
        id_ = -1;
        form_ = name;
    }
    inline void SetId(int id){id_ = id;}
    inline int GetId(){ return id_;}
    inline std::string GetForm(){ return form_;}
protected:
    int id_;
    std::string form_;
};

#endif //STATNLP_TOKEN_H
