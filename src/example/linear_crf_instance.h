//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_LINEAR_CRF_INSTANCE_H
#define STATNLP_LINEAR_CRF_INSTANCE_H

#include <vector>
#include "../common/types/base_instance.h"
#include "../hybridnetworks/common.h"

//template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>

class LinearCRFInstance: public BaseInstance<LinearCRFInstance, Input_Str_Matrix, Label_Str_Matrix>{
public:
    LinearCRFInstance(){

    }
    LinearCRFInstance(LinearCRFInstance &inst, Input_Str_Matrix &input, Label_Str_Matrix &label, int id, double weight){
        BaseInstance<LinearCRFInstance, Input_Str_Matrix, Label_Str_Matrix>(inst,input,label,id,weight);
    }
    ~LinearCRFInstance(){

    }
    int Size(){
        this->ptr_input->size();
    }

};

#endif //STATNLP_LINEAR_CRF_INSTANCE_H
