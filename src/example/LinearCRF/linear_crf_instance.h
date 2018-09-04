//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_LINEAR_CRF_INSTANCE_H
#define STATNLP_LINEAR_CRF_INSTANCE_H

#include <vector>
#include "src/common/types/base_instance.h"
#include "src/common/common.h"
//template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
class LinearCRFInstance: public BaseInstance<LinearCRFInstance, ComType::Input_Str_Matrix, ComType::Label_Str_Vector>{
public:
    LinearCRFInstance();
   // LinearCRFInstance(LinearCRFInstance *ptr_inst, Input_Str_Matrix* ptr_input, Label_Str_Vector *ptr_label, int id, double weight);
    LinearCRFInstance(int instance_id, double weight, ComType::Input_Str_Matrix *ptr_words, ComType::Label_Str_Vector *ptr_labels);
    ~LinearCRFInstance();
    int GetSize();

private:
//    Input_Str_Matrix *ptr_words_;
//    Label_Str_Vector *ptr_labels_;

};
#endif //STATNLP_LINEAR_CRF_INSTANCE_H
