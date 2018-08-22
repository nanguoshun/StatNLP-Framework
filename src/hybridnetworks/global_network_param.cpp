//
// Created by  ngs on 31/07/2018.
//

#include <random>
#include "global_network_param.h"

GlobalNetworkParam::GlobalNetworkParam() {
    is_locked_ = false;
    size_ = 0;
    fixed_feature_size_ = 0;
    obj_prev_= ComParam::DOUBLE_NEGATIVE_INFINITY;
    obj_current_ = ComParam::DOUBLE_NEGATIVE_INFINITY;
    is_discriminative_ = ! ComParam::TRAIN_MODE_IS_GENERATIVE;
    if(IsDiscriminative()){
        //ptr_opt_ = new CRFPP::LBFGS();
        kappa_ =ComParam::L2_REGULARIZATION_CONSTANT;
    }
    ptr_featureIntMap_ = new ComType::FeatureIntMap;
    ptr_type2InputMap_ = new ComType::Type2InputMap;
    version_= -1;
    tmp_count_ = 0;
    small_change_count = 0;
}

GlobalNetworkParam::~GlobalNetworkParam() {
    delete ptr_opt_;
    for(auto type_it = ptr_featureIntMap_->begin(); type_it != ptr_featureIntMap_->end(); ++type_it){
        for(auto output_it = (*type_it).second->begin(); output_it != (*type_it).second->end(); ++output_it){
            for(auto input_it = (*output_it).second->begin(); input_it != (*output_it).second->end(); ++input_it){
                delete &(*input_it);
            }
            delete &(*output_it);
        }
        delete &(*type_it);
    }
    delete ptr_featureIntMap_;

    delete ptr_type2InputMap_;
}
/*
 * Lock the features.
 */
void GlobalNetworkParam::LockIt() {
    if(this->IsLocked()){
        return;
    }
    this->ExpandFeaturesForGenerativeModelDuringTesting();
    this->ptr_counts_ = new double[size_];
    this->ptr_weights_ = new double[size_];
    //note: no fixed feature size operation in current c++ version.
    for(int feature_no = this->fixed_feature_size_; feature_no < this->size_; ++feature_no ){
        double random_value = DoubleRandom(0.0,1.0);
        random_value = (random_value - 0.5) / 10;
        this->ptr_counts_[feature_no] =  ComParam::RANDOM_INIT_WEIGHT ? random_value:ComParam::FEATURE_INIT_WEIGHT;
    }
    this->ResetCountsAndObj();
    this->ptr_feature2rep = new std::string*[size_];
    for(auto type_it = ptr_featureIntMap_->begin(); type_it != ptr_featureIntMap_->end(); ++type_it){
        std::string type = type_it->first;
        for(auto output_it = (*type_it).second->begin(); output_it != (*type_it).second->end(); ++output_it){
            std::string output = output_it->first;
            for(auto input_it = (*output_it).second->begin(); input_it != (*output_it).second->end(); ++input_it){
                std::string input = input_it->first;
                int featureId = input_it->second;
                this->ptr_feature2rep[featureId] = new std::string[3]{type,output,input};
            }
        }
    }
    this->version_ = 0;
    this->ptr_opt_ = new CRFPP::LBFGS();
    this->is_locked_ = true;
}

bool GlobalNetworkParam::Update() {
    bool done;
    if(this->IsDiscriminative()){
        done = this->UpdateDiscriminative();
    } else{
        done = this->UpdateGenerative();
    }
    this->obj_prev_ = this->obj_current_;
    return done;
}

bool GlobalNetworkParam::UpdateDiscriminative() {
    //use lbfgs
    /**
     * go_on_training equals 0: training finished.
     * go_on_training equals 1: go on next training.
     */
    int go_on_training = this->ptr_opt_->optimize(size_,ptr_weights_,-obj_current_,ptr_counts_, true, 1.0);
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
            return  -1;
        } else{
            ComType::FeatureInMap_Value *ptr_output2input = ptr_featureIntMap_->find(type)->second;
            if(ptr_output2input->end() == ptr_output2input->find(output)){
                return  -1;
            } else{
                std::unordered_map<std::string, int> *ptr_input2id = ptr_output2input->find(output)->second;
                if(ptr_input2id->end() == ptr_input2id->find(input)){
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
        ptr_map_value_value->insert(std::make_pair(input,this->size_));
        this->size_++;
        tmp_count_++;
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

double GlobalNetworkParam::DoubleRandom(double min, double max) {
    std::uniform_real_distribution<double> unif(min, max);
    std::default_random_engine re;
    return unif(re);
}

void GlobalNetworkParam::ResetCountsAndObj() {
    for(int feature_no = 0; feature_no < size_; ++feature_no){
        this->ptr_counts_[feature_no] = 0.0;
        //for regulation
        if(this->IsDiscriminative() && this->kappa_ > 0 &&  feature_no >= this->fixed_feature_size_){
          this->ptr_counts_[feature_no] += 2 * this->kappa_ * this->ptr_weights_[feature_no];
        }
    }
    this->obj_current_ = 0.0;
    //for regulation
    if(this->IsDiscriminative() && this->kappa_ > 0){
        this->obj_current_ += - (this->kappa_ * SquareVector(ptr_weights_, size_));
    }
}

double GlobalNetworkParam::SquareVector(double *vec, int size) {
    double value = 0.0;
    for(int i = 0; i < size; ++i){
        value += vec[i] * vec[i];
    }
    return value;
}

void GlobalNetworkParam::AddObj(double obj) {
    this->obj_current_ += obj;
}

void GlobalNetworkParam::AddCount(int feature_index, double count) {
    if(isnan(count)){
        std::cerr<<"Error: the count is NAN: @GlobalNetworkParam::AddCount"<<std::endl;
        return;
    }
    //FIXME: need further discussion for fixed definition
    if(this->IsFixed(feature_index)){
        return;
    }
    /**
     * We set the gradient summation as negative, since the LBFGS that will be used as the optimizer,
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