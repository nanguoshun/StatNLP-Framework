//
// Created by  ngs on 06/07/2018.
//

#ifndef PCFG_NODE_H
#define PCFG_NODE_H

#include <iostream>

class Node{
public:
    Node(std::string value);
    std::string GetData();
    void SetLeftNode(Node *ptr_node);
    void SetRightNode(Node *ptr_node);
    Node * GetLeftNode();
    Node * GetRightNode();
    void SetStartIndex(int start);
    void SetEndIndex(int stop);
    int GetStartIndex();
    int GetEndIndex();
private:
    std::string value_;
    Node *ptr_l_node_;
    Node *ptr_r_node_;
    int start_index_;
    int end_index_;
};

#endif //PCFG_NODE_H
