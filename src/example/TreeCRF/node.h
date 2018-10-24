//
// Created by  ngs on 19/10/2018.
//

#ifndef PCFG_NODE_H
#define PCFG_NODE_H

#include <iostream>
#include <vector>

enum TreeNodeType{
    TERMINAL_NODE,
    NON_TERMINAL_NODE,
    WORDS_NODE
};

typedef std::pair<std::string, std::string> LabeledWord;

class Node{
public:
    Node(std::string value);
    Node(std::string value, std::string word);
    ~Node();
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
    std::vector<std::string>* GetConstituent();
    void CopyConstituentVec(std::vector<std::string>* ptr_src_vec);
    std::vector<std::string>* GetWordVector();
    std::vector<LabeledWord>* GetLeaves();
    void FillLeaves();
    void SetWord(std::string word_str);
private:
    std::string value_;
    std::string word_;
    int id_;
    Node *ptr_l_node_;
    Node *ptr_r_node_;
    int start_index_;
    int end_index_;
    TreeNodeType type_;
    std::vector<std::string> *ptr_constituents_vec_;
    std::vector<std::string> *ptr_words_vec_;
    std::vector<LabeledWord> *ptr_leaves_vec_;
    //BinaryTree *ptr_left_tree_, *ptr_right_tree_;
};

#endif //PCFG_NODE_H
