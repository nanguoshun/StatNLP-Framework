//
// Created by  ngs on 05/10/2018.
//

#include "linear_ne_instance.h"

LinearNEInstance::LinearNEInstance() {

}

LinearNEInstance::LinearNEInstance(int instance_id, double weight, Sentence *ptr_sent,
                                   NER_OutPut_Type *ptr_labels)
        :BaseInstance<LinearNEInstance, Sentence, NER_OutPut_Type>(this, ptr_sent, ptr_labels, instance_id, weight) {
    ptr_input_ = ptr_sent;
}

LinearNEInstance::~LinearNEInstance() {

}

int LinearNEInstance::GetSize() {
    return ptr_input_->GetLength();
}

/**
 *
 *
 */
//FIXME: this is memory expensive
ComType::Input_Str_Vector *LinearNEInstance::GetStrVect() {
    return ptr_str_vec_;
}

void LinearNEInstance::SetStrVect(ComType::Input_Str_Vector *ptr_vec) {
    ptr_str_vec_ = ptr_vec;
}

/**
 *
 * Extract a completed sentence consisted of the first word of a line of ptr_input,
 * such as "Confidence" in the line "Confidence NN B-NP"
 *
 */
void LinearNEInstance::ExtractStrVect() {
    ptr_str_vec_ = new ComType::Input_Str_Vector;
    for (int i = 0; i < ptr_input_->GetLength(); ++i) {
        std::string str = ptr_input_->Get(i)->GetForm();/* the first word is the feature we need */
        ptr_str_vec_->push_back(str);
    }
}