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
 *
 */
template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
class BaseInstance: public Instance{
public:
    BaseInstance(){
        ptr_input_ = nullptr;
        ptr_output_ = nullptr;
        ptr_prediction_ = nullptr;
    }

    BaseInstance(SELF_TYPE *ptr_self, INPUT_TYPE *ptr_input, OUTPUT_TYPE *ptr_output){
        ptr_input_ = nullptr;
        ptr_output_ = nullptr;
        ptr_prediction_ = nullptr;
    }

    BaseInstance(int id, double weight):Instance(id,weight){
        ptr_input_ = nullptr;
        ptr_output_ = nullptr;
        ptr_prediction_ = nullptr;
    }

    BaseInstance(SELF_TYPE *ptr_self, INPUT_TYPE *ptr_input, OUTPUT_TYPE *ptr_output, int id, double weight):Instance(id,weight){
        ptr_input_ = ptr_input;
        ptr_output_ = ptr_output;
        ptr_prediction_ = nullptr;
    }

    ~BaseInstance(){
        if(nullptr != ptr_input_){
            delete ptr_input_;
            ptr_input_ = nullptr;
        }
        if(nullptr != ptr_output_){
            delete ptr_output_;
            ptr_output_ = nullptr;
        }
        if(nullptr != ptr_prediction_){
            delete ptr_prediction_;
            ptr_prediction_= nullptr;
        }
    }

    SELF_TYPE* Duplicate(INPUT_TYPE *ptr_input, OUTPUT_TYPE *ptr_output, OUTPUT_TYPE *ptr_prediction);

    OUTPUT_TYPE *GetOutPut();

    INPUT_TYPE *GetInput();

    void SetInput(INPUT_TYPE *ptr_input);

    OUTPUT_TYPE *GetPrediction();

    void SetPrediction(OUTPUT_TYPE *ptr_prediction);

    /**
     * this function is only for duplicate instances to avoid the memory releasing problem.
     */
    void SetAllPointerNull();
protected:
    INPUT_TYPE *ptr_input_;
    OUTPUT_TYPE *ptr_output_;
    OUTPUT_TYPE *ptr_prediction_;
};

//template <class SELF_TYPE = BaseInstance <SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>,class INPUT_TYPE, class OUTPUT_TYPE>

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

template<class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
void BaseInstance<SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::SetInput(INPUT_TYPE *ptr_input){
    ptr_input_ = ptr_input;
}

template<class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
SELF_TYPE* BaseInstance<SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::Duplicate(INPUT_TYPE *ptr_input, OUTPUT_TYPE *ptr_output, OUTPUT_TYPE *ptr_prediction) {
    /*
    Instance *ptr_duplicate = new Instance();
    this->ptr_input_ = ptr_input;
    this->ptr_output_ = ptr_output;
    this->ptr_prediction_ = ptr_prediction;
    return ptr_duplicate;*/
}

template<class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
void BaseInstance<SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::SetPrediction(OUTPUT_TYPE *ptr_prediction) {
    ptr_prediction_ = ptr_prediction;
}

template<class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
void BaseInstance<SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>::SetAllPointerNull() {
    ptr_input_ = nullptr;
    ptr_output_ = nullptr;
    ptr_prediction_ = nullptr;
}

#endif //STATNLP_BASE_INSTANCE_H
