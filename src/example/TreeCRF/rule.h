//
// Created by  ngs on 19/10/2018.
//

#ifndef STATNLP_RULE_H
#define STATNLP_RULE_H

#include <iostream>
#include <unordered_map>
#include <set>
#include <boost/functional/hash.hpp>
#include "src/common/types/binarytree.h"
#include "label.h"
#include "src/common/types/base_instance.h"
#include "tree_crf_instance.h"

typedef std::pair<std::string, std::pair<std::string, std::string>> CFG_Rule;
typedef std::unordered_map<Label *, std::set<CFG_Rule *> *> CFG_Rule_Label_Map;
typedef std::unordered_map<CFG_Rule *, int> CFG_Rule_Map;

class CFGRule {
public:
    inline static std::unordered_map<std::string, int> *ptr_non_terminator_map_ = new std::unordered_map<std::string, int>;
    inline static CFG_Rule_Label_Map *ptr_rule_label_map_ = new CFG_Rule_Label_Map;
    inline static CFG_Rule_Map *ptr_rule_map_ = new CFG_Rule_Map; /* a map that stores all rules*/
    inline CFGRule(Label *ptr_left_side, Label *ptr_first_right, Label *ptr_second_right) {
        ptr_left_side_ = ptr_left_side;
        ptr_first_right_ = ptr_first_right;
        ptr_second_right_ = ptr_second_right;
    }

    inline ~CFGRule() {

    }

    inline static void GetRule(std::vector<Instance *> *ptr_inst){
        for(auto it = ptr_inst->begin(); it != ptr_inst->end(); ++it){
            TreeCRFInstance *ptr_tree_inst = (TreeCRFInstance *)ptr_inst;
            BinaryTree *ptr_tree = ptr_tree_inst->GetOutPut();
            GetRules(ptr_tree);
        }
    }

    inline static void GetRules(BinaryTree *ptr_tree) {
        if (nullptr == ptr_tree->LeftNode()) return;
        std::string left_str = ptr_tree->GetRootNode()->GetData();
        Label *ptr_label = Label::Get(left_str);
        std::set<CFG_Rule *> *ptr_rule_set = nullptr;
        auto it = ptr_rule_label_map_->find(ptr_label);
        /*check whether the rule set that rooted by ptr_label exists in the map,
         * if it does not exit, then create it and then insert to the map */
        if (ptr_rule_label_map_->end() == it) {
            ptr_rule_set = new std::set<CFG_Rule *>;
            ptr_rule_label_map_->insert(std::make_pair(ptr_label, ptr_rule_set));
            std::string right1 = ptr_tree->LeftNode()->GetData();
            std::string right2 = ptr_tree->RightNode()->GetData();
            CFG_Rule *ptr_rule = new CFG_Rule{std::make_pair(left_str, std::make_pair(right1, right2))};
            ptr_rule_set->insert(ptr_rule);
        } else {
            ptr_rule_set = (*it).second;
            //to be done:
        }
    }

    inline static CFG_Rule *GetCFGRule(std::string left, std::string right1, std::string right2) {

    }

private:
    /* A -> B, C,  then A is the leftside, B and C are first right and second right respectively*/
    Label *ptr_left_side_;
    Label *ptr_first_right_;
    Label *ptr_second_right_;
};

#endif //STATNLP_RULE_H
