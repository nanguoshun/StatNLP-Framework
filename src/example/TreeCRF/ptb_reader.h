//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_READER_H
#define STATNLP_TREE_READER_H

#include <iostream>
#include <unordered_map>
#include "src/common/types/instance.h"
#include "tree_crf_instance.h"
#include "binarytree.h"
#include <fstream>

typedef std::vector<Instance *> InstenceVector;

class PTBReader {
public:
    inline static int max_len_ = 0; // max length for a sentence;
    inline static std::pair<InstenceVector *, InstenceVector *>
    ReadPTB(std::string file_name, std::unordered_map<std::string, int> *ptr_word2int_map,
            bool is_train) {
        InstenceVector *ptr_inst_vec = new InstenceVector;
        InstenceVector *ptr_inst_vec_dup = nullptr;
        if(is_train){
            ptr_inst_vec_dup = new InstenceVector;
        }
        std::ifstream ifs(file_name);
        std::string line_str;
        int instanceId = 1;
        while (std::getline(ifs,line_str)){
            BinaryTree *ptr_tree = new BinaryTree(line_str);
            if(max_len_ < ptr_tree->GetXVector()->size()){
                max_len_ = ptr_tree->GetXVector()->size();
            }
            TreeCRFInstance *ptr_tree_crf_inst = new TreeCRFInstance(instanceId,1.0,ptr_tree->GetXVector(),ptr_tree);
            ptr_tree_crf_inst->SetLabeled();
            ptr_inst_vec->push_back(ptr_tree_crf_inst);
            instanceId++;
            if(is_train){
                TreeCRFInstance *ptr_tree_crf_inst_dup =new TreeCRFInstance(-instanceId,-1.0,ptr_tree->GetXVector(), nullptr);
                ptr_tree_crf_inst_dup->SetUnlabeled();
                ptr_inst_vec_dup->push_back(ptr_tree_crf_inst_dup);
            }
        }
        std::pair<InstenceVector *, InstenceVector *> pair_pointer = std::make_pair(ptr_inst_vec,ptr_inst_vec_dup);
        return pair_pointer;
    }
};

#endif //STATNLP_TREE_READER_H
