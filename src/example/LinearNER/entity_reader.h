//
// Created by  ngs on 06/10/2018.
//

#ifndef STATNLP_ENTITY_READER_H
#define STATNLP_ENTITY_READER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "src/common/types/instance.h"
#include "src/common/common.h"
#include "linear_ne_instance.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include "src/common/types/word_token.h"
#include "entity.h"

typedef std::vector<Instance *> InstenceVector;

class EntityReader {
public:
    inline static int max_len = 0;
    inline static std::vector<std::string> all_labels; /*all unique labels*/
    /**
     *
     * Read the dataset and convert them into a specific labeling schemes,
     * such as IOB1, IOB2, IOE1, IOE2, IOBES and IO formats.
     *
     * @param file_name: the file name of dataset
     * @param ptr_inst_vec:
     * @param ptr_inst_vec_dup:
     * @param encoding: labeling format
     *
     */
    inline static std::pair<InstenceVector *, InstenceVector *>
    ReadData(std::string file_name, std::string encoding, int number, bool setLabel = true) {
        InstenceVector *ptr_inst_vec = new InstenceVector; /* will be released in main function */
        InstenceVector *ptr_inst_vec_dup = new InstenceVector; /*will be released in main function */
        std::ifstream ifs(file_name);
        std::string line_str;
        std::string prev_line = "O";
        std::string prev_entity_str = "O";
        std::vector<std::string> *ptr_entity_vec = new std::vector<std::string>;
        std::vector<WordToken *> word_token;
        int sent_index = 1;
        while (std::getline(ifs, line_str)) { /* read each line in the dataset */
            /* the first line of conll2003 */
            if (boost::starts_with(line_str, "-DOCSTART-")) {
                prev_line = "-DOCSTART-";
                continue;
            }
            if (0 == line_str.compare("") &&
                0 == prev_line.compare("-DOCSTART-")) { /*the first blank after the "-DOCSTART-"*/
                prev_line = "";
                prev_entity_str = "0";
                continue;
            }
            /* the end token of a sentence */
            if (0 == line_str.compare("") && 1 == prev_line.compare("-DOCSTART-")) {
                LinearNEInstance *ptr_inst = CreateInstance(encoding, word_token, sent_index, ptr_entity_vec, setLabel);
                ptr_inst_vec->push_back(ptr_inst);
                word_token.clear();
                ptr_entity_vec = new std::vector<std::string>;
                prev_line = "";
                prev_entity_str = "O";
                if (number != -1 && ptr_inst->GetSize() == number) break;
                continue;
            }
            std::vector<std::string> str_vec;
            boost::split(str_vec, line_str, boost::is_any_of(" "));/*split the line into tokens*/
            std::string raw_current_entity_str = str_vec[3];
            std::string current_entity_str = "";
            SetIOBEncoding(encoding, raw_current_entity_str, current_entity_str, prev_entity_str);
            word_token.push_back(new WordToken(str_vec[0], str_vec[1], -1, current_entity_str));
            ptr_entity_vec->push_back(current_entity_str);
            prev_line = line_str;
            prev_entity_str = current_entity_str;
        }
        return std::make_pair(ptr_inst_vec, ptr_inst_vec_dup);
    }

    /***
     *
     * Create an instance of LinearNEInstance, and initialize it.
     *
     * @param encoding
     * @param word_token
     * @param sent_index
     * @param pptr_entity_vec
     * @param setLabel
     * @return
     *
     */
    inline static LinearNEInstance *
    CreateInstance(std::string encoding, std::vector<WordToken *> &word_token, int &sent_index,
                   std::vector<std::string> *ptr_entity_vec, bool setLabel) {
        WordToken **pptr_word = new WordToken *[word_token.size()]; /*delete in the sentence instance*/
        std::copy(word_token.begin(), word_token.end(), pptr_word);
        /* it will be released in BaseInstance */
        Sentence *ptr_sent = new Sentence(pptr_word, word_token.size());
        LinearNEInstance *ptr_inst = new LinearNEInstance(sent_index++, 1.0, ptr_sent, ptr_entity_vec);
        if (1 == encoding.compare("NONE")) SetEncoding(ptr_inst, encoding);
        if (setLabel) {
            ptr_inst->SetLabeled();
        } else {
            ptr_inst->SetUnlabeled();
        }
        /* it will be released in BaseInstance */
        return ptr_inst;
    }

    /**
     *
     * Set B- and I- prefix of an entity.
     *
     * @param encoding
     * @param raw_current_entity_str
     * @param current_entity_str
     * @param prev_entity_str
     *
     */
    inline static void
    SetIOBEncoding(std::string encoding, std::string &raw_current_entity_str, std::string &current_entity_str,
                   std::string &prev_entity_str) {
        /* if encoding is not NONE */
        if (1 == encoding.compare("NONE")) {
            if (0 == raw_current_entity_str.compare("O")) {
                current_entity_str = "O";
            } else {
                std::string sub_str = raw_current_entity_str.substr(2);
                if (0 == prev_entity_str.compare("O")) {
                    current_entity_str = "B-" + sub_str;
                } else if (0 == prev_entity_str.substr(2).compare(sub_str)) {
                    /* for multiple I- labels, just set the same value */
                    if (0 == prev_entity_str.compare(raw_current_entity_str)) {
                        current_entity_str = prev_entity_str;
                    } else {
                        /* set as the I- if the previous label is B- */
                        if (0 == prev_entity_str.compare("B-")) {
                            current_entity_str = "I-" + sub_str;
                        } else {
                            current_entity_str = "B-" + sub_str;
                        }
                    }
                    current_entity_str = "I-" + sub_str;
                } else {
                    current_entity_str = "B-" + sub_str;
                }
            }
        } else {
            current_entity_str = raw_current_entity_str;
        }
    }

    /**
     *
     * @param ptr_inst
     * @param encoding
     *
     */
    static void SetEncoding(LinearNEInstance *ptr_inst, std::string encoding) {
        std::string prev_entity_str = "O";
        if (0 == encoding.compare("IOBES")) {
            std::vector<std::string> *ptr_output = ptr_inst->GetOutPut();
            Sentence *ptr_input = ptr_inst->GetInput();
            int size = ptr_inst->GetSize();
            for(int pos = 0; pos < size; ++pos){
                std::string cur_entity_str = (*ptr_output)[pos];
                std::string nex_entity_str = pos <  size - 1 ? (*ptr_output)[pos+1]:"O";
                std::string new_cur_entity_str = cur_entity_str;
                if(0 == prev_entity_str.compare("O")){ /* if the previous entity is "O"*/
                    if(1 == cur_entity_str.compare("O")){
                        /*to set B- or S-, the latter indicates a single entity without B, E, and I*/
                        std::string sub_str = cur_entity_str.substr(2);
                        if(boost::starts_with(nex_entity_str,"I-") && 0 == nex_entity_str.substr(2).compare(sub_str)){
                            new_cur_entity_str = "B-"+sub_str;
                        }else{
                            new_cur_entity_str = "S-"+sub_str;
                        }
                    }
                }else if(1 == prev_entity_str.compare("O") && 1 == cur_entity_str.compare("O")){ /* if the previous entity is not "O"*/
                    new_cur_entity_str = GetNewCurrentEntity(prev_entity_str,cur_entity_str,nex_entity_str);
                }
                (*ptr_output)[pos] = new_cur_entity_str;
                ptr_input->Get(pos)->SetEntity(new_cur_entity_str);
                Entity::Get(new_cur_entity_str);
                prev_entity_str = cur_entity_str;
            }
        }
    }
    /***
     *
     * @param prev_entity_str
     * @param cur_entity_str
     * @param nex_entity_str
     * @return
     */
    static std::string GetNewCurrentEntity(std::string &prev_entity_str, std::string &cur_entity_str, std::string &nex_entity_str){
        std::string new_cur_entity_str;
        std::string pre_sub_str = prev_entity_str.substr(2);
        std::string cur_sub_str = cur_entity_str.substr(2);
        if(0 == pre_sub_str.compare(cur_sub_str)){ /*if the suffix of previous entity equals to current*/
            if(boost::starts_with(prev_entity_str,"E-") || boost::starts_with(prev_entity_str,"S-")){
                std::string next_sub_str = nex_entity_str.substr(2);
                if(boost::starts_with(nex_entity_str,"I-") && 0 == next_sub_str.compare(cur_sub_str)){
                    new_cur_entity_str = "B-"+cur_sub_str;
                } else{
                    new_cur_entity_str = "S-"+cur_sub_str;
                }
            } else if(boost::starts_with(prev_entity_str,"I-")){
                std::string next_sub_str = nex_entity_str.substr(2);
                if(boost::starts_with(cur_entity_str,"I-")){
                    if(nex_entity_str.size() >2 && 0 == nex_entity_str.compare(cur_sub_str) &&
                       boost::starts_with(nex_entity_str,"I-")){
                        new_cur_entity_str = cur_entity_str;
                    } else{
                        new_cur_entity_str = "E-"+cur_sub_str;
                    }
                }else{
                    new_cur_entity_str = cur_entity_str;
                }
            }else if(boost::starts_with(prev_entity_str,"B-")){
                if(0 == nex_entity_str.compare(cur_entity_str)){
                    new_cur_entity_str = cur_entity_str;
                } else{
                    new_cur_entity_str = "E-"+cur_sub_str;
                }
            }
        } else{
            std::string next_sub_str = nex_entity_str.substr(2);
            if(false == boost::starts_with(nex_entity_str,"I-") || 1 == next_sub_str.compare(cur_sub_str)){
                new_cur_entity_str = "S-"+cur_sub_str;
            } else{
                new_cur_entity_str = "B-"+cur_sub_str;
            }
        }
        return new_cur_entity_str;
    }
};

//int EntityReader::max_len = 0;

#endif //STATNLP_ENTITY_READER_H
