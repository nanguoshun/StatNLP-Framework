//
// Created by  ngs on 31/07/2018.
//

#include "linear_crf_instance.h"
/*
LinearCRFInstance::LinearCRFInstance(LinearCRFInstance *ptr_inst, Input_Str_List_Vector *ptr_input,
                                     Label_Str_List *ptr_label, int id, double weight) {
    BaseInstance<LinearCRFInstance, Input_Str_List_Vector, Label_Str_List>(id, weight);
}*/

LinearCRFInstance::LinearCRFInstance(int instance_id, double weight, Input_Str_List_Vector *ptr_words,
                                     Label_Str_List *ptr_labels):BaseInstance<LinearCRFInstance, Input_Str_List_Vector, Label_Str_List>(this,ptr_words,ptr_labels,instance_id, weight) {
    ptr_words_ = ptr_words;
    ptr_labels_ = ptr_labels;
}

LinearCRFInstance::LinearCRFInstance() {
    ptr_words_ = nullptr;
    ptr_output_ = nullptr;
}

LinearCRFInstance::~LinearCRFInstance() {

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
    }
}

int LinearCRFInstance::Size() {
    ptr_input_->size();
}
