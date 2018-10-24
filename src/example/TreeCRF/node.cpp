//
// Created by  ngs on 19/10/2018.
//
#include "node.h"
//#include "binarytree.h"

Node::Node(std::string value) {
    value_ = value;
    ptr_l_node_ = ptr_r_node_ = nullptr;
    start_index_ = 0;
    end_index_ = 0;
    type_ = TreeNodeType::NON_TERMINAL_NODE;
    ptr_constituents_vec_ = ptr_words_vec_ = nullptr;
    ptr_leaves_vec_ = nullptr;
}

Node::Node(std::string value, std::string word) {
    value_ = value;
    word_ = word;
    ptr_l_node_ = ptr_r_node_ = nullptr;
    start_index_ = 0;
    end_index_ = 0;
    type_ = TreeNodeType::NON_TERMINAL_NODE;
    ptr_constituents_vec_ = ptr_words_vec_ = nullptr;
    ptr_leaves_vec_ = nullptr;
}

Node::~Node() {
    if(nullptr != ptr_constituents_vec_){
        delete ptr_constituents_vec_;
    }
    if(nullptr != ptr_words_vec_){
        delete ptr_words_vec_;
    }
    if(nullptr != ptr_leaves_vec_){
        delete ptr_leaves_vec_;
    }
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

int Node::GetHeight() {
    int height = end_index_ - start_index_;
    if(height < 0){
        std::cerr<< "Error: the start_index(left side branch) is bigger than end_index (right side branch) in a tree"<<std::endl;
    }
    return height;
}

void Node::SetNodeType(TreeNodeType type) {
    type_ = type;
}

TreeNodeType Node::GetNodeType() {
    return type_;
}

/**
 * Get the constituent of the node.
 * @return
 */
std::vector<std::string>* Node::GetConstituent() {
    ptr_constituents_vec_ = new std::vector<std::string>;
    if(nullptr == ptr_l_node_){
        std::string str = value_ + " " + word_;
        ptr_constituents_vec_->push_back(str);
    } else{
        CopyConstituentVec(ptr_l_node_->GetConstituent());
        CopyConstituentVec(ptr_r_node_->GetConstituent());
        std::string constituent_str = "";
        std::vector<std::string>* ptr_words_vec = GetWordVector();
        auto end =  ptr_words_vec->end();
        for(auto it = ptr_words_vec->begin(); it != end; ++it){
            if(constituent_str.size() != 0){
                constituent_str += " ";
            }
            constituent_str += (*it);
        }
        std::string final_constituent_str = value_ + " " + constituent_str;
        ptr_constituents_vec_->push_back(final_constituent_str);
    }
    return ptr_constituents_vec_;
}


void Node::CopyConstituentVec(std::vector<std::string> *ptr_src_vec) {
    for(auto it = ptr_src_vec->begin(); it != ptr_src_vec->end(); ++it){
        std::string str = (*it);
        ptr_constituents_vec_->push_back(str);
    }
}

/**
 * Get the word vector of the node.
 */

std::vector<std::string>* Node::GetWordVector() {
    if(nullptr == ptr_words_vec_){
        std::vector<LabeledWord> *ptr_leaves_vec = GetLeaves();
        ptr_words_vec_ = new std::vector<std::string>;
        auto end = ptr_leaves_vec->end();
        for(auto it = ptr_leaves_vec->begin(); it != end; ++it){
            std::string word = (*it).second;
            ptr_words_vec_->push_back(word);
        }
    }
    return ptr_words_vec_;
}

std::vector<LabeledWord>* Node::GetLeaves() {
    if(nullptr == ptr_leaves_vec_){
        FillLeaves();
    }
    return ptr_leaves_vec_;
}

void Node::FillLeaves() {
    if(nullptr == ptr_l_node_ || nullptr == ptr_r_node_){
        ptr_leaves_vec_ = new std::vector<LabeledWord>{std::make_pair(value_,word_)};
        return;
    }
    std::vector<LabeledWord> *ptr_l_leaves = ptr_l_node_->GetLeaves();
    std::vector<LabeledWord> *ptr_r_leaves = ptr_r_node_->GetLeaves();
    ptr_leaves_vec_ = new std::vector<LabeledWord>;
    auto l_end = ptr_l_leaves->end();
    for(auto it = ptr_l_leaves->begin(); it != l_end; ++it ){
        ptr_leaves_vec_->push_back((*it));
    }
    auto r_end = ptr_r_leaves->end();
    for(auto it = ptr_r_leaves->begin(); it != r_end; ++it){
        ptr_leaves_vec_->push_back((*it));
    }
}

void Node::SetWord(std::string word_str) {
    word_ = word_str;
}

