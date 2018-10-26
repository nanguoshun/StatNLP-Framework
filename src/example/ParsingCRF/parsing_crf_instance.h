//
// Created by  ngs on 15/10/2018.
//

#ifndef STATNLP_TREE_CRF_INSTANCE_H
#define STATNLP_TREE_CRF_INSTANCE_H

#include "src/common/types/base_instance.h"
#include "binarytree.h"
#include <iostream>

class ParsingCRFInstance : public BaseInstance<ParsingCRFInstance, std::vector<std::string>, BinaryTree> {
public:
    inline ParsingCRFInstance();

    inline ParsingCRFInstance(int instance_id, double weight, std::vector<std::string> *ptr_input, BinaryTree *ptr_tree)
            : BaseInstance<ParsingCRFInstance, std::vector<std::string>, BinaryTree>(this, ptr_input,
                                                                                  ptr_tree, instance_id, weight) {
        ptr_input_ = ptr_input;

    }

    inline ~ParsingCRFInstance() {

    }

    inline int GetSize(){
        return ptr_input_->size();
    }
};


#endif //STATNLP_TREE_CRF_INSTANCE_H
