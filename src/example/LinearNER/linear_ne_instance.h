//
// Created by  ngs on 05/10/2018.
//

#ifndef STATNLP_LINEAR_NE_INSTANCE_H
#define STATNLP_LINEAR_NE_INSTANCE_H

#include "src/common/types/base_instance.h"
#include "src/common/types/common.h"
#include "src/common/types/sentence.h"

typedef ComType::Label_Str_Vector NER_OutPut_Type;

class LinearNEInstance:public BaseInstance<LinearNEInstance, Sentence, NER_OutPut_Type>{
public:
    LinearNEInstance();
    LinearNEInstance(int instance_id, double weight, Sentence *ptr_sent, NER_OutPut_Type *ptr_labels);
    ~LinearNEInstance();
    int GetSize();
    ComType::Input_Str_Vector * GetStrVect();
    void SetStrVect(ComType::Input_Str_Vector *ptr_vec);
    void ExtractStrVect();

private:
    //Sentence *ptr_sent_;
    ComType::Input_Str_Vector *ptr_str_vec_;
};

#endif //STATNLP_LINEAR_NE_INSTANCE_H
