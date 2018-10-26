//
// Created by  ngs on 19/10/2018.
//

#ifndef STATNLP_EVALUATE_H
#define STATNLP_EVALUATE_H

#include <vector>
#include "parsing_crf_instance.h"
class Evaluate{
public:
    inline Evaluate(){

    }
    inline ~Evaluate(){

    }
    /**
     * Evaluate the results and compute the precision, recall and f1 score.
     *
     * @param ptr_test_inst
     */
    inline static void EvaluateResult(std::vector<Instance *> *ptr_test_inst){
        int total_corr = 0;
        int total_gold = 0;
        int total_pred = 0;
        int sent_no = 0;
        for(auto it = ptr_test_inst->begin(); it != ptr_test_inst->end(); ++it){
            sent_no++;
            ParsingCRFInstance *ptr_tree_inst = (ParsingCRFInstance*) (*it);
            std::vector<std::string> *ptr_gold_constituent = ptr_tree_inst->GetOutPut()->GetConstituent();
            std::vector<std::string> *ptr_predict_constituent = ptr_tree_inst->GetPrediction()->GetConstituent();
            int num_of_gold = ptr_gold_constituent->size();
            int num_of_pred = ptr_predict_constituent->size();
            total_gold += num_of_gold;
            total_pred += num_of_pred;
            int num_of_correct = CompareConstituent(ptr_gold_constituent,ptr_predict_constituent);
            total_corr += num_of_correct;
            std::cout << sent_no<< "th sentence:"<<std::endl;
            std::cout <<"Correct constituents are: "<<num_of_correct<<std::endl;
            std::cout <<"Gold constituents are: "<<num_of_gold<<std::endl;
            std::cout <<"Predicted constituents are: "<<num_of_pred<<std::endl;
            double precision = (double)num_of_correct / (double)num_of_pred;
            double recall = (double)num_of_correct / (double)num_of_gold;
            double f1 = 2 / ((1/precision) + (1/recall));
            std::cout << "Precision, Recall and F1,  P:"<<precision<<" R: "<<recall<<" F: "<<f1<<std::endl;
            std::cout << std::endl;
//            PrintConstituent(ptr_gold_constituent,ptr_predict_constituent);
        }
        std::cout <<"Totally, "<< sent_no <<" sentences are decoded."<<std::endl;
        std::cout <<"Total correct constituents are: "<<total_corr<<std::endl;
        std::cout <<"Total gold constituents are: "<<total_gold<<std::endl;
        std::cout <<"Total predicted constituents are: "<<total_pred<<std::endl;
        double precision = (double)total_corr / (double)total_pred;
        double recall = (double)total_corr / (double)total_gold;
        double f1 = 2 / ((1/precision) + (1/recall));
        std::cout << "Overall Precision, Recall and F1,  P: "<<precision<<" R: "<<recall<<" F: "<<f1<<std::endl;
    }
    /**
     *
     * Get the overlap counts between golden constituents and predicted constituents.
     *
     * @param ptr_gold
     * @param pr_predict
     * @return
     */
    inline static int CompareConstituent(std::vector<std::string> *ptr_gold, std::vector<std::string> *ptr_predict){
        int corr = 0;
        auto end_pred = ptr_predict->end();
        auto begin_gold = ptr_gold->begin();
        auto end_gold = ptr_gold->end();
        for(auto it = ptr_predict->begin(); it != end_pred; ++it){
            std::string constituent_str = (*it);
            auto pos = std::find(begin_gold,end_gold,constituent_str);
            if(end_gold != pos){
                corr++;
            }
        }
        return corr;
    }

    /**
     * Print the golden and predicted constituents.
     *
     * @param ptr_gold
     * @param pr_predict
     */
    inline static void PrintConstituent(std::vector<std::string> *ptr_gold, std::vector<std::string> *pr_predict){
        std::cout << "prediction is: ";
        for(auto it = ptr_gold->begin(); it != ptr_gold->end(); ++it){
            std::cout << (*it) << " | ";
        }
        std::cout << std::endl;
        std::cout << "gold is: ";
        for(auto it = pr_predict->begin(); it != pr_predict->end(); ++it){
            std::cout << (*it) << " | ";
        }
        std::cout << std::endl;
    }
};
#endif //STATNLP_EVALUATE_H
