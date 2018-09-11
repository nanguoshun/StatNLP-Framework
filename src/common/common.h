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

//#define DEBUG = 2;

#ifndef GLOBAL__
#define GLOBAL__
#endif

#ifndef GLOBAL_
#define GLOBAL_
#endif

namespace ComParam{
    const int USE_HANDCRAFTED_FEATURES = 0;
    //use hand-crafted features and neural features.
    const int USE_HYBRID_NEURAL_FEATURES = 1;
    //use pure neural features.
    const int USE_PURE_NEURAL_FEATURES = 2;
    const int OPT_SGD = 0;
    const int OPT_LBFGS = 1;
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
    const double OBJTOL = 10e-15;
    const int OPTIMIZER = OPT_LBFGS;
    const double LEARNING_RATE = 0.005;

}

namespace ComType{
    typedef std::vector<std::vector<std::string>> Input_Str_Matrix;
    typedef std::vector<std::string> Label_Str_Vector;
    typedef std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, int>*>*> FeatureIntMap;
    typedef std::unordered_map<std::string, std::unordered_map<std::string, int>*> FeatureInMap_Value;
    typedef std::unordered_map<std::string, std::list<std::string>*> Type2InputMap;
    enum NODE_TYPES{
        LEAF, NODE, ROOT
    };
}

namespace NetworkConfig{
    // the feature type is set as hand-crafted defaultly.
    static int Feature_Type = ComParam::USE_HANDCRAFTED_FEATURES;
    static bool FEATURE_TOUCH_TEST = false;

    /*neural network related*/
    static int kSOS = 0;
    static int kEOS = 0;
    static float DROP_OUT = 0.3;
    static unsigned HIDDREN_SIZE = 32;
    static unsigned INPUT_DIM = 2;
    static unsigned LSTM_INPUT_DIM = 0;
    static float DECAY_RATE = 0;
    static unsigned LAYERS = 1;
    static unsigned DECAY_ONSET_EPOCH = 10;
    static unsigned VACB_SIZE = 0;
}

//typedef std::vector<std::vector<std::string>> Input_Str_Matrix;
//typedef std::vector<std::vector<std::string>> Label_Str_Vector;
//typedef std::string** Input_Str_Matrix;
//typedef std::string** Label_Str_Vector;
//typedef std::vector<std::string> Input_Str_Matrix;
//typedef std::vector<std::string> Label_Str_Vector;

//typedef std::list<std::vector<std::string>> Input_Str_Matrix;
//typedef std::list<std::string> Label_Str_Vector;


/*
enum FeatureType{
    WORD,
    WORD_BIGRAM,
    TAG,
    TAG_BIGRAM,
    TRANSITION
};*/



#endif //STATNLP_COMMON_H
