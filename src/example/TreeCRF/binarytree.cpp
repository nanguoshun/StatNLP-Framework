//
// Created by  ngs on 06/07/2018.
//

#include "binarytree.h"

BinaryTree::BinaryTree(std::string str) {
    //ptr_str_vector_ = new std::vector<std::string>;
    ptr_x_vector_ = new std::vector<std::string>;
    //ConvertToStringVector(str);
    index_offset_ = 0;
    ptr_root_ = BuildBinaryTree(str);
    //PrintTree(ptr_root_);
}

BinaryTree::~BinaryTree() {
    //delete ptr_str_vector_;
    delete ptr_x_vector_;
    for(std::vector<Node *>::iterator it = node_vector_.begin(); it!=node_vector_.end();++it){
        delete(*it);
    }
}

/*
void BinaryTree::ConvertToStringVector(std::string str) {
    std::stringstream ss(str);
    std::string str_tmp;
    while(ss >> str_tmp){
        ptr_str_vector_->push_back(str_tmp);
#ifdef IF_DEBUG__
        std::cout << "the vector is"<< str_tmp <<std::endl;
#endif
    }
}*/

Node *BinaryTree::BuildBinaryTree(std::string str) {
    if (0 == str.length()) {
        return NULL;
    }
    int space_index = str.find_first_of(ComParam::SPACE_STRING);
    std::string value_str = str.substr(1, space_index - 1);
    Node *ptr_node = new Node(value_str);
    ptr_node->SetStartIndex(ptr_x_vector_->size());
    int index = -1;
    if (str[0] == ComParam::LEFT_BRACKET_CHAR) {
        /*insert non-terminor label*/
        TagLabel(ptr_node,str,value_str,space_index);
        index = FindNextBracketIndex(str);
    }
    if (index != -1 && index != -2) {
        std::string left_str = str.substr(space_index + 1, index - space_index);
        ptr_node->SetLeftNode(BuildBinaryTree(left_str));
        std::string rigth_str = str.substr(index + 1, str.length() - index - 2);
        ptr_node->SetRightNode(BuildBinaryTree(rigth_str));
    }else if(index == -2){
        // the terminator
        std::string left_str = str.substr(space_index+1, str.length() - space_index -2);
        //Node *ptr_last_node = new Node(left_str);
        //ptr_last_node->SetNodeType(TreeNodeType::WORDS_NODE);
        //ptr_node->SetLeftNode(ptr_last_node);
        ptr_x_vector_->push_back(left_str);
        //Label::Get(left_str,true);
    }
    node_vector_.push_back(ptr_node);
    ptr_node->SetEndIndex(ptr_x_vector_->size() - 1);
    return ptr_node;
}

int BinaryTree::FindNextBracketIndex(std::string str) {
    if (str.length() <= 1) {
        return -1;
    }
    std::stack<char> bracket_stack;
    for (int i = 1; i < str.length(); ++i) {
        if (str[i] == ComParam::LEFT_BRACKET_CHAR) {
            bracket_stack.push(ComParam::LEFT_BRACKET_CHAR);
        } else if (str[i] == ComParam::RIGHT_BRACKET_CHAR) {
            if (bracket_stack.empty()) {
                //the emission
                return -2;
            }
            if (bracket_stack.top() == ComParam::LEFT_BRACKET_CHAR) {
                bracket_stack.pop();
                if (bracket_stack.empty()) {
                    //return the position of division
                    return i;
                }
            }
        }
    }
    //not found
    return -1;
}

bool BinaryTree::PrintTree(Node *ptr_node) {
    if (ptr_node == NULL) {
        return NULL;
    }
//    std::cout << ptr_node->GetData() << " : "<<ptr_node->GetStartIndex()<<","<<ptr_node->GetEndIndex()<< std::endl;
    PrintTree(ptr_node->GetLeftNode());
    PrintTree(ptr_node->GetRightNode());
}

Node *BinaryTree::GetRootNode() {
    return ptr_root_;
}

std::vector<std::string>* BinaryTree::GetXVector() {
    return ptr_x_vector_;
}

Node* BinaryTree::LeftNode() {
    return ptr_root_->GetLeftNode();
}

Node* BinaryTree::RightNode() {
    return ptr_root_->GetRightNode();
}

/**
 * Insert Terminal or Non-Terminal label to a label set.
 * @param str
 * @param space_index
 */

void BinaryTree::TagLabel(Node *ptr_node, std::string &str, std::string &value_str, int space_index) {
    /*if( 0 == value_str.compare("In")){
        std::cout << "the label is PRN"<<std::endl;
    }*/
    std::string sub_str = str.substr(1);
    int next_left_bracket_index = sub_str.find_first_of(ComParam::LEFT_BRACKET_CHAR);
    if(next_left_bracket_index == space_index){
        Label::Get(value_str,false);
        ptr_node->SetNodeType(TreeNodeType::NON_TERMINAL_NODE);
        //std::cout << "non-terminator is "<<value_str<<std::endl;
    } else{
        Label::Get(value_str,true);
        ptr_node->SetNodeType(TreeNodeType::TERMINAL_NODE);
        //std::cout << "terminator is "<<value_str<<std::endl;
    }
}
