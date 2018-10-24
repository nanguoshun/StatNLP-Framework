//
// Created by  ngs on 24/10/2018.
//

#ifndef STATNLP_NEW_BASE_INSTANCE_H
#define STATNLP_NEW_BASE_INSTANCE_H

#include "instance.h"
class NewBaseInstance: public Instance{
public:
    inline NewBaseInstance(int id, int weight){
        id_ = id;
        weight_ = weight;
    }
    inline ~NewBaseInstance(){

    }
protected:
    int id_;
    int weight_;
};

#endif //STATNLP_NEW_BASE_INSTANCE_H
