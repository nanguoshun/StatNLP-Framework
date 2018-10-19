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

class BinaryTree{
public:
    BinaryTree(std::string str);
    ~BinaryTree();
    void ConvertToStringVector(std::string str);
    Node * BuildBinaryTree(std::string str);
    int FindNextBracketIndex(std::string str);
    bool PrintTree(Node *ptr_node);
    Node * GetRootNode();
    std::vector<std::string> *GetXVector();
    Node *LeftNode();
    Node *RightNode();
    BinaryTree *LeftTree();
    BinaryTree *RightTree();
private:
    Node *ptr_root_;
    std::vector<Node *> node_vector_;
    std::vector<std::string> *ptr_str_vector_;
    std::vector<std::string> *ptr_x_vector_;
    int index_offset_;
    BinaryTree *ptr_left_tree_, *ptr_right_tree_;
};

#endif //PCFG_BINARYTREE_H
