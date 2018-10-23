//
// Created by  ngs on 06/07/2018.
//

#ifndef PCFG_BINARYTREE_H
#define PCFG_BINARYTREE_H

#include <iostream>
#include <vector>
#include <stack>
#include "node.h"
#include "src/common/common.h"
#include <sstream>
#include "label.h"

struct LabelWord{
    Label label_;
    std::string word_;
};

class BinaryTree{
public:
    BinaryTree(std::string str);
    BinaryTree();
    //BinaryTree(std::string str, Node *ptr_node);
    ~BinaryTree();
    //void ConvertToStringVector(std::string str);
    Node * BuildBinaryTree(std::string str);
    void TagLabel(Node *ptr_node,std::string &str, std::string &value_str,int space_index);
    int FindNextBracketIndex(std::string str);
    bool PrintTree(Node *ptr_node);
    Node * GetRootNode();
    std::vector<std::string> *GetXVector();
    Node *LeftNode();
    Node *RightNode();
    void SetRootNode(Node *ptr_root_node);
private:
    Node *ptr_root_;
    //std::vector<Node *> node_vector_;
    //std::vector<std::string> *ptr_str_vector_;
    /* vector that stores the sentence */
    std::vector<std::string> *ptr_x_vector_;
    //int index_offset_;
    //BinaryTree *ptr_left_tree_, *ptr_right_tree_;
};

#endif //PCFG_BINARYTREE_H
