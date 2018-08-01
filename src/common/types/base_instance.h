//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_BASE_INSTANCE_H
#define STATNLP_BASE_INSTANCE_H

#include "instance.h"
/**
 *
 * @tparam T0: its self type
 * @tparam T1: input type
 * @tparam T2: output type
 */

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>

class BaseInstance: public Instance{
public:
    BaseInstance(){

    }
    BaseInstance(SELF_TYPE self, INPUT_TYPE input, OUTPUT_TYPE output){

    }
    BaseInstance(SELF_TYPE self, INPUT_TYPE input, OUTPUT_TYPE output, int id, double weight){
        Instance::Instance(id,weight);
    }
    ~BaseInstance(){

    }
};

#endif //STATNLP_BASE_INSTANCE_H
