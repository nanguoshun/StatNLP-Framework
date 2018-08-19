//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_COMMON_H
#define STATNLP_COMMON_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <cmath>

const int MAX_LENGTH = 300;
const int Num_Of_Threads = 1;
const bool _CACHE_FEATURES_DURING_TRAINING = true;
const double DOUBLE_NEGATIVE_INFINITY = -std::numeric_limits<double>::infinity();
const double TRAIN_MODE_IS_GENERATIVE = false;
const double  L2_REGULARIZATION_CONSTANT = 0.01;
const bool RANDOM_INIT_WEIGHT = true;
const double FEATURE_INIT_WEIGHT = 0.0;

const int FOREST_MAX_HEIGHT = 10000;
const int FOREST_MAX_WIDTH = 10000;
const int NETWORK_MAX_DEPTH = 901;
const int NETWORK_CAPACITY = 1000000;


//typedef std::vector<std::vector<std::string>> Input_Str_Matrix;
//typedef std::vector<std::vector<std::string>> Label_Str_Vector;
//typedef std::string** Input_Str_Matrix;
//typedef std::string** Label_Str_Vector;
//typedef std::vector<std::string> Input_Str_Matrix;
//typedef std::vector<std::string> Label_Str_Vector;

//typedef std::list<std::vector<std::string>> Input_Str_Matrix;
//typedef std::list<std::string> Label_Str_Vector;

typedef std::vector<std::vector<std::string>> Input_Str_Matrix;
typedef std::vector<std::string> Label_Str_Vector;

typedef std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, int>*>*> FeatureIntMap;
typedef std::unordered_map<std::string, std::unordered_map<std::string, int>*> FeatureInMap_Value;
typedef std::unordered_map<std::string, std::list<std::string>*> Type2InputMap;

enum NODE_TYPES{
    LEAF, NODE, ROOT
};
/*
enum FeatureType{
    WORD,
    WORD_BIGRAM,
    TAG,
    TAG_BIGRAM,
    TRANSITION
};*/



#endif //STATNLP_COMMON_H
