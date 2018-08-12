//
// Created by  ngs on 31/07/2018.
//

#include "base_instance.h"

/*
template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::BaseInstance(SELF_TYPE *ptr_self, INPUT_TYPE *ptr_input, OUTPUT_TYPE *ptr_output) {
}

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::BaseInstance() {

}

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::BaseInstance(int id, double weight) {
    Instance::Instance(id,weight);
}

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::BaseInstance(SELF_TYPE *ptr_self, INPUT_TYPE *ptr_input, OUTPUT_TYPE *ptr_output, int id, double weight){
ptr_input_ = ptr_input;
ptr_output_ = ptr_output;
Instance::Instance(id,weight);
}


template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::~BaseInstance(){

}
*/

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
OUTPUT_TYPE* BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::GetOutPut() {
    return ptr_output_;
}

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
INPUT_TYPE* BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::GetInput() {
    return ptr_input_;
}

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
OUTPUT_TYPE* BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::GetPrediction() {
    return ptr_prediction_;
}



