//
// Created by  ngs on 31/07/2018.
//

#include "linear_crf_instance.h"
/*
LinearCRFInstance::LinearCRFInstance(LinearCRFInstance *ptr_inst, Input_Str_Matrix *ptr_input,
                                     Label_Str_Vector *ptr_label, int id, double weight) {
    BaseInstance<LinearCRFInstance, Input_Str_Matrix, Label_Str_Vector>(id, weight);
}*/

LinearCRFInstance::LinearCRFInstance(int instance_id, double weight, ComType::Input_Str_Matrix *ptr_words,
                                     ComType::Label_Str_Vector *ptr_labels):BaseInstance<LinearCRFInstance, ComType::Input_Str_Matrix, ComType::Label_Str_Vector>(this,ptr_words,ptr_labels,instance_id, weight) {
    //ptr_words_ = ptr_words;
    //ptr_labels_ = ptr_labels;
    //ptr_input_ = ptr_words;
    ptr_str_vec_ = nullptr;

}

LinearCRFInstance::LinearCRFInstance() {
    //ptr_words_ = nullptr;
    //ptr_output_ = nullptr;
    ptr_str_vec_ = nullptr;
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

ComType::Input_Str_Vector* LinearCRFInstance::GetStrVect() {
    if(nullptr == ptr_str_vec_){
        ptr_str_vec_ = new ComType::Input_Str_Vector;
        for(auto it = ptr_input_->begin(); it != ptr_input_->end(); ++it){
            std::string str = (*it)[0];/* the first word is the feature we need */
            ptr_str_vec_->push_back(str);
        }
    }
    return ptr_str_vec_;
}
