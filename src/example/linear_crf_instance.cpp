//
// Created by  ngs on 31/07/2018.
//

#include "linear_crf_instance.h"
/*
LinearCRFInstance::LinearCRFInstance(LinearCRFInstance *ptr_inst, Input_Str_Vector *ptr_input,
                                     Label_Str_Vector *ptr_label, int id, double weight) {
    BaseInstance<LinearCRFInstance, Input_Str_Vector, Label_Str_Vector>(id, weight);
}*/

LinearCRFInstance::LinearCRFInstance(int instance_id, double weight, Input_Str_Vector *ptr_words,
                                     Label_Str_Vector *ptr_labels):BaseInstance<LinearCRFInstance, Input_Str_Vector, Label_Str_Vector>(this,ptr_words,ptr_labels,instance_id, weight) {

}

LinearCRFInstance::LinearCRFInstance() {

}

LinearCRFInstance::~LinearCRFInstance() {

}

int LinearCRFInstance::Size() {
    ((Input_Str_Vector *) ptr_input_)->size();
}