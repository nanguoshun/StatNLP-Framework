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
    BaseInstance(SELF_TYPE *ptr_self, INPUT_TYPE *ptr_input, OUTPUT_TYPE *ptr_output){

    }
    BaseInstance(int id, double weight){

    }
    BaseInstance(SELF_TYPE *ptr_self, INPUT_TYPE *ptr_input, OUTPUT_TYPE *ptr_output, int id, double weight){
        ptr_input_ = ptr_input;
        ptr_output_ = ptr_output;
        Instance::Instance(id,weight);
    }
    ~BaseInstance(){

    }
    OUTPUT_TYPE *GetOutPut();

    INPUT_TYPE *GetInput();

    OUTPUT_TYPE *GetPrediction();
protected:
    INPUT_TYPE *ptr_input_;
    OUTPUT_TYPE *ptr_output_;
    OUTPUT_TYPE *ptr_prediction_;
};

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
OUTPUT_TYPE* BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::GetOutPut() {
    return ptr_output_;
}

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
OUTPUT_TYPE* BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::GetPrediction() {
    return ptr_prediction_;
}

template<class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
INPUT_TYPE *BaseInstance<SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::GetInput() {
    return ptr_input_;
    //return (void*) ptr_input_;
}


#endif //STATNLP_BASE_INSTANCE_H
