//
// Created by  ngs on 06/07/2018.
//
#include "node.h"

Node::Node(std::string value) {
    value_ = value;
    ptr_l_node_ = ptr_r_node_ = NULL;
    start_index_ = 0;
    end_index_ = 0;
}

Node* Node::GetLeftNode() {
    return ptr_l_node_;
}

Node* Node::GetRightNode() {
    return ptr_r_node_;
}

void Node::SetLeftNode(Node *ptr_node) {
    ptr_l_node_ = ptr_node;
}

void Node::SetRightNode(Node *ptr_node) {
    ptr_r_node_ = ptr_node;
}

std::string Node::GetData() {
    return value_;
}

void Node::SetStartIndex(int start) {
    start_index_ = start;
}

void Node::SetEndIndex(int stop) {
    end_index_ = stop;
}

int Node::GetStartIndex() {
    return start_index_;
}

int Node::GetEndIndex() {
    return  end_index_;
}

int Node::GetId() {
    return id_;
}

void Node::SetId(int id) {
    id_ = id;
}