//
// Created by  ngs on 06/07/2018.
//

#ifndef PCFG_NODE_H
#define PCFG_NODE_H
enum TreeNodeType{
    TERMINAL_NODE,
    NON_TERMINAL_NODE,
    WORDS_NODE
};

#include <iostream>
class Node{
public:
    Node(std::string value);
    Node(std::string value, std::string word);
    std::string GetData();
    void SetLeftNode(Node *ptr_node);
    void SetRightNode(Node *ptr_node);
    Node * GetLeftNode();
    Node * GetRightNode();
    int GetId();
    void SetId(int id);
    void SetStartIndex(int start);
    void SetEndIndex(int stop);
    int GetStartIndex();
    int GetEndIndex();
    int GetHeight();
    TreeNodeType GetNodeType();
    void SetNodeType(TreeNodeType type);
private:
    std::string value_;
    std::string word_;
    int id_;
    Node *ptr_l_node_;
    Node *ptr_r_node_;
    int start_index_;
    int end_index_;
    TreeNodeType type_;
    //BinaryTree *ptr_left_tree_, *ptr_right_tree_;
};

#endif //PCFG_NODE_H
