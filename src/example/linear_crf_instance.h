//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_LINEAR_CRF_INSTANCE_H
#define STATNLP_LINEAR_CRF_INSTANCE_H

#include <vector>
#include "../common/types/base_instance.h"
#include "../hybridnetworks/common.h"
//template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>
class LinearCRFInstance: public BaseInstance<LinearCRFInstance, Input_Str_Vector, Label_Str_Vector>{
public:
    LinearCRFInstance();
   // LinearCRFInstance(LinearCRFInstance *ptr_inst, Input_Str_Vector* ptr_input, Label_Str_Vector *ptr_label, int id, double weight);
    LinearCRFInstance(int instance_id, double weight, Input_Str_Vector *ptr_words, Label_Str_Vector *ptr_labels);
    ~LinearCRFInstance();
    int Size();
};
#endif //STATNLP_LINEAR_CRF_INSTANCE_H
