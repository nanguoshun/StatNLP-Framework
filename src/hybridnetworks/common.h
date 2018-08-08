//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_COMMON_H
#define STATNLP_COMMON_H

const int MAX_LENGTH = 200;
const int Num_Of_Threads = 1;
const bool _CACHE_FEATURES_DURING_TRAINING = false;
const double DOUBLE_NEGATIVE_INFINITY = std::numeric_limits<double>::infinity();
const double TRAIN_MODE_IS_GENERATIVE = false;
//typedef std::vector<std::vector<std::string>> Input_Str_Vector;
//typedef std::vector<std::vector<std::string>> Label_Str_Vector;
typedef std::vector<std::string> Input_Str_Vector;
typedef std::vector<std::string> Label_Str_Vector;

enum NODE_TYPES{
    LEAF, NODE, ROOT
};

#endif //STATNLP_COMMON_H
