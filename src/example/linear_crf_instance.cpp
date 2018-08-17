//
// Created by  ngs on 31/07/2018.
//

#include "linear_crf_instance.h"
/*
LinearCRFInstance::LinearCRFInstance(LinearCRFInstance *ptr_inst, Input_Str_Matrix *ptr_input,
                                     Label_Str_Vector *ptr_label, int id, double weight) {
    BaseInstance<LinearCRFInstance, Input_Str_Matrix, Label_Str_Vector>(id, weight);
}*/

LinearCRFInstance::LinearCRFInstance(int instance_id, double weight, Input_Str_Matrix *ptr_words,
                                     Label_Str_Vector *ptr_labels):BaseInstance<LinearCRFInstance, Input_Str_Matrix, Label_Str_Vector>(this,ptr_words,ptr_labels,instance_id, weight) {
    //ptr_words_ = ptr_words;
    //ptr_labels_ = ptr_labels;
    //ptr_input_ = ptr_words;

}

LinearCRFInstance::LinearCRFInstance() {
    //ptr_words_ = nullptr;
    //ptr_output_ = nullptr;
}

LinearCRFInstance::~LinearCRFInstance() {
    /*
    if(ptr_words_ != nullptr){
        //release the vector firstly
        for(auto it = ptr_words_->begin(); it!=ptr_words_->end(); ++it){
            delete &(*it);
        }
        //release the list.
        delete ptr_words_;
    }
    if(ptr_output_ != nullptr){
        delete ptr_output_;
    }*/
}

int LinearCRFInstance::GetSize() {
    return ptr_input_->size();
}
