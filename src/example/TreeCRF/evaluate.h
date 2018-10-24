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
        int k = 0;
        for(auto it = ptr_test_inst->begin(); it != ptr_test_inst->end(); ++it){
            TreeCRFInstance *ptr_tree_inst = (TreeCRFInstance*) (*it);
            std::vector<std::string> *ptr_gold_constituent = ptr_tree_inst->GetOutPut()->GetConstituent();
            std::vector<std::string> *ptr_predict_constituent = ptr_tree_inst->GetPrediction()->GetConstituent();
            std::cout << "prediction is: ";
            for(auto it = ptr_predict_constituent->begin(); it != ptr_predict_constituent->end(); ++it){
                std::cout << (*it) << " | ";
            }
            std::cout << std::endl;
            std::cout << "gold is: ";
            for(auto it = ptr_gold_constituent->begin(); it != ptr_gold_constituent->end(); ++it){
                std::cout << (*it) << " | ";
            }
            std::cout << std::endl;
            k++;
        }
    }
};
#endif //STATNLP_EVALUATE_H
