//
// Created by  ngs on 19/10/2018.
//

#ifndef STATNLP_RULE_H
#define STATNLP_RULE_H

#include <iostream>
#include <unordered_map>
#include <set>
#include <boost/functional/hash.hpp>
#include "binarytree.h"
#include "label.h"
#include "src/common/types/base_instance.h"
#include "tree_crf_instance.h"

typedef std::pair<std::string, std::pair<std::string, std::string>> CFG_Rule;
typedef std::unordered_map<Label *, std::set<CFG_Rule> *> CFG_Rule_Label_Map;
const std::string TERMINATOR_FLAG = "TERNMINATOR";

enum NodeType{
    SINK,
    NODE,
    ROOT,
    SUPER_ROOT
};
class CFGRule {
public:
    inline static CFG_Rule_Label_Map *ptr_rule_label_map_ = new CFG_Rule_Label_Map;
    inline CFGRule(Label *ptr_left_side, Label *ptr_first_right, Label *ptr_second_right) {
        ptr_left_side_ = ptr_left_side;
        ptr_first_right_ = ptr_first_right;
        ptr_second_right_ = ptr_second_right;
    }

    inline ~CFGRule() {

    }

    inline static void BuildCFGRules(std::vector<Instance *> *ptr_inst){
        int sent_no = 0;
        for(auto it = ptr_inst->begin(); it != ptr_inst->end(); ++it){
            TreeCRFInstance *ptr_tree_inst = (TreeCRFInstance *)(*it);
            BinaryTree *ptr_tree = ptr_tree_inst->GetOutPut();
            //std::cout << sent_no <<"th sentence CFG rules "<<std::endl;
            BuildCFGRules(ptr_tree->GetRootNode());
            sent_no++;
        }
    }
    /**
     *
     * Build rules in ptr_label_rule_map, if it does not store in the map, then insert it into the map.
     *
     * @param ptr_label_rule_map
     * @param ptr_tree
     */
    inline static void BuildCFGRules(Node *ptr_root) {
        if (nullptr == ptr_root) return;
        if (nullptr == ptr_root->GetLeftNode()) return;
        if (TreeNodeType::NON_TERMINAL_NODE != ptr_root->GetNodeType()) return;
/*        std::string test_str = ptr_root->GetData();
        if (0 == test_str.compare("PP")){
           std::cout << "PRN"<<std::endl;
        }*/
        std::string left_str = ptr_root->GetData();
        std::string right1_str = ptr_root->GetLeftNode()->GetData();
        /*if the rule is terminator*/
        std::string right2_str = TERMINATOR_FLAG;
        if (nullptr != ptr_root->GetRightNode()){
            right2_str = ptr_root->GetRightNode()->GetData();
        }
        Label *ptr_label  = Label::Get(left_str);
        Label *ptr_right1 = Label::Get(right1_str);
        Label *ptr_right2 = Label::Get(right2_str);
        std::set<CFG_Rule> *ptr_rule_set = nullptr;
        /* check whether the rule set that rooted by ptr_label exists in the map,
         * if it does not exit, then create it and then insert to the map */
        if (ptr_rule_label_map_->end() == ptr_rule_label_map_->find(ptr_label)) {
            ptr_rule_set = new std::set<CFG_Rule>;
            ptr_rule_label_map_->insert(std::make_pair(ptr_label, ptr_rule_set));
        }
        /* get the rule set of label ptr_label */
        ptr_rule_set = ptr_rule_label_map_->find(ptr_label)->second;
        ptr_rule_set->insert(std::make_pair(left_str,std::make_pair(right1_str,right2_str)));
        BuildCFGRules(ptr_root->GetLeftNode());
        BuildCFGRules(ptr_root->GetRightNode());
    }

    inline Label *GetLeft(){
        return ptr_left_side_;
    }
    inline Label *GetFirstRight(){
        return ptr_first_right_;
    }
    inline Label *GetSecondRight(){
        return  ptr_second_right_;
    }


private:
    /* A -> B, C,  then A is the leftside, B and C are first right and second right respectively*/
    Label *ptr_left_side_;
    Label *ptr_first_right_;
    Label *ptr_second_right_;
};

#endif //STATNLP_RULE_H
