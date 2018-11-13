//
// Created by  ngs on 01/11/2018.
//

#ifndef STATNLP_DATA_READER_H
#define STATNLP_DATA_READER_H

#include <iostream>
#include "src/example/LinearCRF/linear_crf_instance.h"
#include <fstream>
#include <sstream>

class DataReader {
public:
    inline DataReader() {}
    inline  ~DataReader() {}
    static std::vector<std::string> *ptr_all_labels_; /*all unique labels*/
    static int max_len_;
    /**
 *
 * @param file_name
 * @param ptr_inst_vec_all : the vector of all instances.
 * @param withLabels
 * @param isLabeled
 *
 */
    inline static void ReadData(std::string file_name, std::vector<Instance *> *ptr_inst_vec_all,
                                std::vector<Instance *> *ptr_inst_vec_all_duplicate, bool istest, bool withLabels,
                                bool isLabeled, std::unordered_map<std::string, int> *ptr_word2int) {
        std::ifstream ifs(file_name);
        std::string str;
        std::vector<std::string> *ptr_labels = nullptr;
        std::vector<std::string> *ptr_labels_du = nullptr;
        int instance_id = 0;
        bool is_allocate_vector = true;
        ComType::Input_Str_Matrix *ptr_vec_matrix;
        ComType::Input_Str_Matrix *ptr_list_vect_du;
        while (std::getline(ifs, str)) {
            //allocate the space for each instance at the beginning.
            if (is_allocate_vector) {
                ptr_vec_matrix = new ComType::Input_Str_Matrix;
                ptr_labels = new std::vector<std::string>;
                //ptr_list_vect_du = new ComType::Input_Str_Matrix;
                ptr_labels_du = new std::vector<std::string>;
                is_allocate_vector = false;
            }
            if (0 == str.length()) {
                if (ptr_vec_matrix->size() > max_len_) {
                    max_len_ = ptr_vec_matrix->size();
                }
                LinearCRFInstance *ptr_crf_inst = new LinearCRFInstance(instance_id, 1.0, ptr_vec_matrix, ptr_labels);
                ptr_crf_inst->ExtractStrVect();
                LinearCRFInstance *ptr_crf_inst_du = nullptr;
                if (!istest) {
                    ptr_crf_inst_du = new LinearCRFInstance(-instance_id, -1.0, ptr_vec_matrix, ptr_labels_du);
                    ptr_crf_inst_du->SetStrVect(ptr_crf_inst->GetStrVect());
                }
                if (isLabeled) {
                    ptr_crf_inst->SetLabeled();
                } else {
                    ptr_crf_inst->SetUnlabeled();
                }
                instance_id++;
                ptr_inst_vec_all->push_back(ptr_crf_inst);
                if (!istest) {
                    ptr_inst_vec_all_duplicate->push_back(ptr_crf_inst_du);
                }
                is_allocate_vector = true;
            } else {
                std::stringstream ss(str);
                std::string feature1, feature2;
                std::vector<std::string> words_vec;
                std::vector<std::string> words_vec_du;
                ss >> feature1;
                ss >> feature2;
                words_vec.push_back(feature1);
                words_vec.push_back(feature2);
            //    ptr_dict->convert(feature1);
                ptr_vec_matrix->push_back(words_vec);
                if (withLabels) {
                    std::string label;
                    ss >> label;
                    auto it = std::find(ptr_all_labels_->begin(), ptr_all_labels_->end(), label);
                    if (it == ptr_all_labels_->end()) {
                        ptr_all_labels_->push_back(label);
                        //ptr_dict->convert(label);
                    }
                    ptr_labels->push_back(label);
                    if (!istest) {
                        ptr_labels_du = nullptr;
                    }
                }
                if (!istest) {
                    auto it = ptr_word2int->find(feature1);
                    if (it == ptr_word2int->end()) {
                        ptr_word2int->insert(std::make_pair(feature1, ptr_word2int->size()));
                    }
                }
            }
        }
        //duplicate the data
        if (!istest) {
            auto it = ptr_word2int->find(ComParam::UNK);
            if (it == ptr_word2int->end()) {
                ptr_word2int->insert(std::make_pair(ComParam::UNK, ptr_word2int->size()));
            }
        }
    }
};

int DataReader::max_len_ = 0;
std::vector<std::string> * DataReader::ptr_all_labels_ = new std::vector<std::string>;
#endif //STATNLP_DATA_READER_H
