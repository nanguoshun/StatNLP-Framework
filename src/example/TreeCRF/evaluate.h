//
// Created by  ngs on 19/10/2018.
//

#ifndef STATNLP_EVALUATE_H
#define STATNLP_EVALUATE_H

#include <vector>
#include "tree_crf_instance.h"
class Evaluate{
public:
    inline Evaluate(){

    }
    inline ~Evaluate(){

    }
    inline static void EvaluateResult(std::vector<Instance *> *ptr_test_inst){
        int corr = 0;
        int total_gold = 0;
        int total_pred = 0;
        for(auto it = ptr_test_inst->begin(); it != ptr_test_inst->end(); ++it){
            TreeCRFInstance *ptr_tree_inst = (TreeCRFInstance*) ptr_test_inst;
            //todo::
        }
    }
};
#endif //STATNLP_EVALUATE_H
