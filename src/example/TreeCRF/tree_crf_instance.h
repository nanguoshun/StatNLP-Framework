//
// Created by  ngs on 15/10/2018.
//

#ifndef STATNLP_TREE_CRF_INSTANCE_H
#define STATNLP_TREE_CRF_INSTANCE_H

#include "src/common/types/base_instance.h"
#include "src/common/types/binarytree.h"
#include <iostream>

class TreeCRFInstance: public BaseInstance<TreeCRFInstance, std::string *, BinaryTree>{
public:
    TreeCRFInstance();
    ~TreeCRFInstance();
};


#endif //STATNLP_TREE_CRF_INSTANCE_H
