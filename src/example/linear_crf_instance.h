//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_LINEAR_CRF_INSTANCE_H
#define STATNLP_LINEAR_CRF_INSTANCE_H

#include "../common/types/base_instance.h"

template <class SELF_TYPE, class INPUT_TYPE, class OUTPUT_TYPE>

class LinearCRFInstance: public BaseInstance<SELF_TYPE, INPUT_TYPE, OUTPUT_TYPE>{
public:
    LinearCRFInstance(SELF_TYPE a, INPUT_TYPE b, OUTPUT_TYPE c, int d, double e){
    }
    ~LinearCRFInstance(){
    }
};

#endif //STATNLP_LINEAR_CRF_INSTANCE_H
