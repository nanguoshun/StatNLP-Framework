//
// Created by  ngs on 31/07/2018.
//

#include <random>
#include <ctime>
#include <stdlib.h>
#include "global_network_param.h"
#include "../common/opt/math_calc.h"
/***
 *
 * @param argc
 * @param argv
 * @param max_sent_size: the max length of a sentence in dataset.
 * @param num_of_sentence:
 * @param ptr_label
 * @param ptr_nf
 * @param ptr_word2int
 * @param ptr_dict
 *
 */
GlobalNetworkParam::GlobalNetworkParam(int &argc, char **&argv, int max_sent_size, int num_of_sentence, std::vector<std::string> *ptr_label, ComType::NeuralType  neural_type,std::unordered_map<std::string,int> *ptr_word2int, dynet::Dict *ptr_dict) {
    is_locked_ = false;
    h_feature_size_ = 0;
    fixed_feature_size_ = 0;
    obj_prev_= ComParam::DOUBLE_NEGATIVE_INFINITY;
    obj_current_ = ComParam::DOUBLE_NEGATIVE_INFINITY;
    is_discriminative_ = ! ComParam::TRAIN_MODE_IS_GENERATIVE;
    if(IsDiscriminative()){
        //ptr_opt_ = new CRFPP::LBFGS();
        kappa_ =NetworkConfig::L2_REGULARIZATION_CONSTANT;
    }
    ptr_featureIntMap_ = new ComType::FeatureIntMap;
    //ptr_type2InputMap_ = new ComType::Type2InputMap;
    version_= -1;
    tmp_count_ = 0;
    small_change_count = 0;
    obj_prev_ = 0;
    ptr_inside_shared_array_ = new double*[ComParam::Num_Of_Threads];
    ptr_outside_shared_array_ = new double*[ComParam::Num_Of_Threads];
    ptr_shared_array_size_ = new int[ComParam::Num_Of_Threads];
    ptr_outside_shared_array_size_ = new int[ComParam::Num_Of_Threads];

    for(int threadid = 0; threadid < ComParam::Num_Of_Threads; ++threadid){
        ptr_inside_shared_array_[threadid] = nullptr;
        ptr_outside_shared_array_[threadid] = nullptr;
        ptr_shared_array_size_[threadid] = 0;
        ptr_outside_shared_array_size_[threadid] = 0;
    }
    ptr_concat_counts_ = nullptr;
    ptr_concat_weights_ = nullptr;
    num_of_instances_ = num_of_sentence;
    ptr_nn_g_ = nullptr;
    //std::cout << "feature type is "<<NetworkConfig::Feature_Type<<std::endl;
    //std::cout << "feature test is "<<Feature_TEST <<std::endl;
    if(ComType::NeuralType::NON_NEURAL == neural_type){
        std::cout << "you are using only handcrafted features"<<std::endl;
    } else{
        if(ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type){
            std::cerr<< "You are in the hand-crafted mode  and you can not create the neural instances, please configure NetworkConfig::Feature_Type as ComParam::USE_HYBRID_NEURAL_FEATURES or ComParam::USE_PURE_NEURAL_FEATURES"<<std::endl;
            return;
            //exit (EXIT_FAILURE);
        } else if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){ /*create neural instances*/
            if(ComType::NeuralType::LSTM == neural_type) {
                ptr_nf_ = (NeuralFactory*)NeuralFactory::GetLSTMFactory();
            } else if(ComType::NeuralType::BILSTM == neural_type) {
                //todo::
            } else if(ComType::NeuralType::CNN == neural_type) {
                //todo::
            } else if(ComType::NeuralType::CNNBiLSTM == neural_type) {
               //todo
            }
            NetworkConfig::FEATURE_TOUCH_TEST = true;
            ptr_nn_g_ = new GlobalNeuralNetworkParam(true);
            ptr_nf_->SetDynetCallFunctionHelper(ptr_nn_g_->GetDynetFunctionHelper());
            /* init the super-parameters of neural network*/
            ptr_nf_->InitNNParameter(argc,argv,ptr_word2int->size(),ptr_label->size());
            /* create the neural network*/
            ptr_nf_->CreateNN();
            ptr_nn_g_->SetNeuralNetwork(ptr_nf_->GetNeuralInst());
            ptr_nn_g_->Initialization(max_sent_size,ptr_word2int,ptr_label,ptr_dict);
        } else if(ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type){
            NetworkConfig::FEATURE_TOUCH_TEST = true;
            //TODO:
        }
    }
    /*
    if(ptr_nn_vec != nullptr){
        ptr_nn_g_ = new GlobalNeuralNetworkParam(ptr_nn_vec);
    } else{
        ptr_nn_g_ = nullptr;
    }*/
}

GlobalNetworkParam::~GlobalNetworkParam() {
    std::cout << "start to release GlobalNetworkParam"<<std::endl;
    delete ptr_opt_;
    for(auto type_it = ptr_featureIntMap_->begin(); type_it != ptr_featureIntMap_->end(); ++type_it){
        for(auto output_it = (*type_it).second->begin(); output_it != (*type_it).second->end(); ++output_it){
            delete (*output_it).second;
        }
        delete (*type_it).second;
    }
    delete ptr_featureIntMap_;
    for(int threadId=0; threadId<ComParam::Num_Of_Threads; ++threadId){
        delete []ptr_inside_shared_array_[threadId];
        delete []ptr_outside_shared_array_[threadId];
    }
    /*delete feature representative*/
    for(int i = 0; i < h_feature_size_; ++i){
        delete []ptr_feature2rep[i];
    }
    delete []ptr_feature2rep;
    delete []ptr_inside_shared_array_;
    delete []ptr_outside_shared_array_;
    delete []ptr_shared_array_size_;
    delete []ptr_outside_shared_array_size_;
    if(ptr_nn_g_ != nullptr){
        delete ptr_nn_g_;
    }
    delete []ptr_weights_;
    delete []ptr_counts_;
    std::cout << " Releasing GlobalNetworkParam Done!"<<std::endl;

    delete ptr_nf_;
    //delete ptr_type2InputMap_;
}
/*
 *
 * Lock the features.
 *
 *
 */
void GlobalNetworkParam::LockIt() {
    if (this->IsLocked()) {
        return;
    }
    ExpandFeaturesForGenerativeModelDuringTesting();
    //the neural feature size should be set beforehand.
    AllocateSpace();
    //note: no fixed feature size operation in current c++ version.
    if(true == NetworkConfig::READ_MODEL){
        std::ifstream ifs(NetworkConfig::MODEL_FILE);
        double weight = 0;
        int index = 0;
        while (ifs >> weight){
            if(index >= h_feature_size_){
                std::cout << "Read Model ERROR: the size of model weight in file is larger than current model"<<std::endl;
            }
            ptr_weights_[index] = weight;
            ++index;
        }
        ifs.close();
    } else {
        for (int feature_no = this->fixed_feature_size_; feature_no < this->h_feature_size_; ++feature_no) {
            double random_value = DoubleRandom();
            random_value = (random_value - 0.5) / 10;
            ptr_weights_[feature_no] = 0.01;// ComParam::RANDOM_INIT_WEIGHT ? random_value:ComParam::FEATURE_INIT_WEIGHT;
#ifdef DEBUG
            std::cout <<feature_no<<"th feature weight is: "<< this->ptr_weights_[feature_no]<<std::endl;
#endif
        }
    }
    this->ResetCountsAndObj(); // neural;
    this->ptr_feature2rep = new std::string *[h_feature_size_];
    for (auto type_it = ptr_featureIntMap_->begin(); type_it != ptr_featureIntMap_->end(); ++type_it) {
        std::string type = type_it->first;
        for (auto output_it = (*type_it).second->begin(); output_it != (*type_it).second->end(); ++output_it) {
            std::string output = output_it->first;
            for (auto input_it = (*output_it).second->begin(); input_it != (*output_it).second->end(); ++input_it) {
                std::string input = input_it->first;
                int featureId = input_it->second;
                this->ptr_feature2rep[featureId] = new std::string[3]{type, output, input};
            }
        }
    }
    this->version_ = 0;
    //NOT_DONE:
    //		int numWeights = this._weights.length + this.getAllNNParamSize();//neural
    //TODO: change to factory mode
    this->ptr_opt_ = new CRFPP::LBFGS();
    this->is_locked_ = true;
}

bool GlobalNetworkParam::Update() {
    bool done;
#ifdef NON_LOCKER
    std::lock_guard<std::mutex> mtx_locker(mtx);
#endif
    if(this->IsDiscriminative()){
        done = this->UpdateDiscriminative();
    } else{
        done = this->UpdateGenerative();
    }
    this->obj_prev_ = this->obj_current_;
    return done;
}

/**
 * Update the weight;
 * @return
 */

bool GlobalNetworkParam::UpdateDiscriminative() {
    //use lbfgs
    /**
     * go_on_training equals 0: training finished.
     * go_on_training equals 1: go on next training.
     */
    // we have allocated the space for vectors of hand-crafted and neural beforehand in GlobalNetworkParam::LockIt() during
    // finalizing the features, and it will be much faster without real-time arraycopy as the java-version did.
    int go_on_training =  1;

    if(ComParam::OPT_LBFGS == ComParam::OPTIMIZER){
        /*noted that the parameter orthant should be set as false to avoid duplicated regularization */
        go_on_training = this->ptr_opt_->optimize(params_size_,ptr_weights_,-obj_current_,ptr_counts_,false, 0);
    } else if(ComParam::OPT_SGD == ComParam::OPTIMIZER){
        for(int i=0; i<h_feature_size_; ++i){
            //std::cout << i <<"th weights before is: "<<ptr_weights_[i]<<"  gradient is: "<<ptr_counts_[i]<<std::endl;
            ptr_weights_[i] = ptr_weights_[i] - ComParam::LEARNING_RATE * ptr_counts_[i];
            //std::cout << i << "th weights after is: "<<ptr_weights_[i]<<std::endl;
        }
    }

    double diff = this->obj_current_ - this->obj_prev_;
    //if the objective function converged, finish training
    if(diff >=0 && diff < ComParam::OBJTOL){
        go_on_training = 0;
    }
    //if there are 3 consecutive times the decrease in objective function is less than 0.01%.
    double diff_ratio = std::abs(diff/obj_prev_);
    if(diff_ratio < 1e-4){
        small_change_count +=1;
    } else{
        small_change_count = 0;
    }
    if(small_change_count ==3){
        go_on_training = 0;
    }

    //changeflag: no memory copy here as the java-version did and will be much more efficient.

    //the iteration num.
    this->version_++;
    if(0 == go_on_training){
        return true;
    } else{
        return false;
    }
}

bool GlobalNetworkParam::UpdateGenerative() {
  //TODO:
}

bool GlobalNetworkParam::IsDiscriminative() {
    return is_discriminative_;
}

double GlobalNetworkParam::GetWeight(int featureId) {
    return ptr_weights_[featureId];
}

double GlobalNetworkParam::GetOldObj() {
    return obj_prev_;
}

bool GlobalNetworkParam::IsLocked() {
    return is_locked_;
}

int GlobalNetworkParam::ToFeature(std::string type, std::string output, std::string input) {

    if(this->IsLocked()){
        if(ptr_featureIntMap_->end() == ptr_featureIntMap_->find(type)){
            //std::cerr << "ERRO: the feature is not found in the map after locked"<<std::endl;
            return  -1;
        } else{
            ComType::FeatureInMap_Value *ptr_output2input = ptr_featureIntMap_->find(type)->second;
            if(ptr_output2input->end() == ptr_output2input->find(output)){
                //std::cerr << "ERRO: the feature is not found in the map after locked"<<std::endl;
                return  -1;
            } else{
                std::unordered_map<std::string, int> *ptr_input2id = ptr_output2input->find(output)->second;
                if(ptr_input2id->end() == ptr_input2id->find(input)){
                    //std::cerr << "ERRO: the feature is not found in the map after locked"<<std::endl;
                    return  -1;
                } else{
                    int featureId = ptr_input2id->find(input)->second;
                    return featureId;
                }
            }
        }
    }
    //if no items exist, then allocate the space and insert this item into feature map;
    if(ptr_featureIntMap_->find(type) == ptr_featureIntMap_->end()){
        ComType::FeatureInMap_Value *ptr_value_new = new ComType::FeatureInMap_Value;
        ptr_featureIntMap_->insert(std::make_pair(type,ptr_value_new));
    }
    ComType::FeatureInMap_Value *ptr_map_value =  ptr_featureIntMap_->find(type)->second;
    if(ptr_map_value->find(output) == ptr_map_value->end()){
        std::unordered_map<std::string, int>* ptr_value_value_new = new std::unordered_map<std::string, int>;
        ptr_map_value->insert(std::make_pair(output,ptr_value_value_new));
    }
    std::unordered_map<std::string, int> *ptr_map_value_value = ptr_map_value->find(output)->second;
    if(ptr_map_value_value->find(input) == ptr_map_value_value->end()){
        ptr_map_value_value->insert(std::make_pair(input,this->h_feature_size_));
        //std::cout << h_feature_size_<<"th feature is"<<input<<", "<<ptr_map_value->find(output)->first<<std::endl;
        this->h_feature_size_++;
        tmp_count_++;
        //return this->h_feature_size_;
        //TODO: for type2inputMap.
    }
    int featureId =  ptr_map_value_value->find(input)->second;
    return featureId;
}

void GlobalNetworkParam::ExpandFeaturesForGenerativeModelDuringTesting() {
    if(this->IsDiscriminative()){
        return;
    }
    //TODO: for the generative models.
}

double GlobalNetworkParam::DoubleRandom() {
    /*
    std::uniform_real_distribution<double> unif(min, max);
    std::default_random_engine re;
    return unif(re);
    */
    double random_value = rand() / double (RAND_MAX);
    return random_value;

}

void GlobalNetworkParam::ResetCountsAndObj() {
#ifdef GLOBAL
    std::lock_guard<std::mutex> mtx_locker(mtx);
#endif
    double coef = 1.0;
    if(NetworkConfig::USE_BATCH_TRAINING){
        coef = batchSize_ * 1.0 / num_of_instances_;
        if(coef > 1){
            coef = 1.0;
        }
    }
    /*gradient regularization*/
    if(IsDiscriminative()){
        for(int feature_no = 0; feature_no < h_feature_size_; ++feature_no){
            ptr_counts_[feature_no] = 0.0;
            //for regulation, update the gradient value;
            if(kappa_ > 0){
                if(feature_no >= fixed_feature_size_){
                    ptr_counts_[feature_no] += 2 * coef * kappa_ * ptr_weights_[feature_no];
                }
            }
#ifdef DEBUG
            std::cout << feature_no<<"th gradient is: "<<this->ptr_counts_[feature_no]<<std::endl;
#endif
        }
        /*caution:: added by nan, no regularization in java version*/
//        if(kappa_ > 0){
//            if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){
//                ptr_nn_g_->Regularization(coef, kappa_);
//            } else if(ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type){
//                ptr_nn_g_->Regularization(coef, kappa_);
//            }
//        }
    }
    obj_current_ = 0.0;
    //for regulation, update the objective value;
    if(IsDiscriminative() && kappa_ > 0){
        //regulation for the graphical model.
        obj_current_ += - (kappa_ * MathCalc::SquareVector(ptr_weights_, h_feature_size_));
        //regulation for the neural part.
        if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){
            std::vector<NeuralNetwork *> *ptr_nn_vec = ptr_nn_g_->GetNNVect();
            for(auto it = ptr_nn_vec->begin(); it != ptr_nn_vec->end(); ++it){
                (*it)->SetScale(coef);
                if(NetworkConfig::REGULARIZE_NEURAL_FEATURES){
//                    obj_current_ += (*it)->GetL2Param();
                    obj_current_ +=  - (kappa_ * (*it)->GetL2Param()); //FIXME: need further confirmation.
                }
            }
        } else if(ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type){
            //todo:
        }
        //FIXME:need to confirmation the minus sign before coef.
        //this->obj_current_ *= -coef * this->kappa_;
    }
}

void GlobalNetworkParam::AddObj(double obj) {
#ifdef NON_LOCKER
    std::lock_guard<std::mutex> lock(mtx);
#endif
    this->obj_current_ += obj;
}

void GlobalNetworkParam::AddCount(int feature_index, double count) {
#ifdef NON_LOCKER
    std::lock_guard<std::mutex> mtx_locker(mtx);
#endif
    if(isnan(count)){
        std::cerr<<"Error: the count is NAN: @GlobalNetworkParam::AddCount"<<std::endl;
        return;
    }
    //FIXME: need further discussion for fixed definition
    if(this->IsFixed(feature_index)){
        return;
    }
    /**
     *
     * We set the gradient summation as negative, since the LBFGS that will be used,
     * aims to minimize the objective function.
     *
     */
    if(this->IsDiscriminative()){
        this->ptr_counts_[feature_index] -= count;
    } else {
        this->ptr_counts_[feature_index] += count;
    }
}

bool GlobalNetworkParam::IsFixed(int f_global) {
    return f_global < this->fixed_feature_size_;
}

double** GlobalNetworkParam::GetInsideSharedArray() {
    return ptr_inside_shared_array_;
}

double** GlobalNetworkParam::GetOutsideSharedArray() {
    return ptr_outside_shared_array_;
}

double* GlobalNetworkParam::GetInsideSharedArray(int threadId) {
    return ptr_inside_shared_array_[threadId];
}

double* GlobalNetworkParam::GetOutsideSharedArray(int threadId) {
    return ptr_outside_shared_array_[threadId];
}

int GlobalNetworkParam::GetSharedArraySize(int threadId) {
    return ptr_shared_array_size_[threadId];
}

int GlobalNetworkParam::GetOutsideSharedArraySize(int threadId) {
    return ptr_outside_shared_array_size_[threadId];
}

/*
int* GlobalNetworkParam::GetInsideSharedArraySize() {
    return ptr_inside_shared_array_size_;
}*/

int* GlobalNetworkParam::GetOutsideSharedArraySize() {
    return ptr_outside_shared_array_size_;
}

void GlobalNetworkParam::AllocateSharedArray(int threadid, int node_count) {
#ifdef NON_LOCKER
    std::lock_guard<std::mutex> mtx_locker(mtx);
#endif
    ptr_inside_shared_array_[threadid] = new double[node_count];
    ptr_outside_shared_array_[threadid] = new double[node_count];
    ptr_shared_array_size_[threadid] = node_count;
}

/**
 * Allocate space for weight and gradient vector.
 */
void GlobalNetworkParam::AllocateSpace() {
    //hybrid features.
    if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        nn_output_size_ = ptr_nn_g_->GetNeuralOutputSize();
        nn_params_size_ = ptr_nn_g_->GetNeuralParamsSize();
        //feature_size_ = h_feature_size_ + nn_output_size_;
        params_size_ = h_feature_size_ + nn_params_size_;
    } else if (ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        nn_output_size_ = ptr_nn_g_->GetNeuralOutputSize();
        nn_params_size_ = ptr_nn_g_->GetNeuralParamsSize();
        //feature_size_ = nn_output_size_; /*pure neural features.*/
        params_size_ = nn_params_size_;
    } else if(ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type){
        //feature_size_ = h_feature_size_; /*pure hand-crafted features*/
        params_size_ = h_feature_size_; /*the feature size equals to parameter size*/
    }
    /* allocate the space */
    ptr_weights_ = new double[params_size_];
    ptr_counts_ = new double[params_size_];
    /*this is a trick to avoid real-time memory copy, we concatenate two consective memory*/
    if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){
        double *ptr_nn_param = ptr_weights_ + h_feature_size_;
        double *ptr_nn_param_grad = ptr_counts_ + h_feature_size_;
        ptr_nn_g_->SetMemoryOfParamAndGradient(ptr_nn_param,ptr_nn_param_grad);
    }
}

GlobalNeuralNetworkParam* GlobalNetworkParam::GetNNParam() {
    return ptr_nn_g_;
}

void GlobalNetworkParam::SetOldObj(double obj) {
    obj_prev_ = obj;
}

double GlobalNetworkParam::GetCurrentObj() {
    return obj_current_;
}

void GlobalNetworkParam::SetNNParameter(GlobalNeuralNetworkParam *ptr_nn_g) {
    ptr_nn_g_ = ptr_nn_g;
}

void GlobalNetworkParam::InitNNParameter(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    ptr_nn_g_->InitNNParameter(argc,argv,random_seed,shared_parameters);
}

int GlobalNetworkParam::GetHandCraftFeatureSize() {
    return h_feature_size_;
}

int GlobalNetworkParam::GetModelParamSize() {
    return params_size_;
}

double* GlobalNetworkParam::GetModelWeight() {
    return ptr_weights_;
}