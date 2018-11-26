//
// Created by  ngs on 03/09/2018.
//

#include "global_neural_network_param.h"
#include "../common/opt/math_calc.h"
#include "../hybridnetworks/local_network_param.h"

/***
 *
 * @param max_len: the max length of the sentence in a dataset.
 *
 */

GlobalNeuralNetworkParam::GlobalNeuralNetworkParam(bool is_training) {
    ptr_call_dynet_ = new DynetFunctionHelper();
    thread_num_ = ComParam::Num_Of_Threads;//????
    ptr_all_NNInput2Id_ = new ComType::Neural_Input_Map_Vect;
    pptr_param_l_ = nullptr;
    ptr_nn_vec_ = nullptr;
    is_training_ = is_training;
}

GlobalNeuralNetworkParam::GlobalNeuralNetworkParam(std::vector<NeuralNetwork *> *ptr_nn_vec, DynetFunctionHelper *ptr_dynet_helper, bool is_training) {
    ptr_nn_vec_ = ptr_nn_vec;
    ptr_call_dynet_ = ptr_dynet_helper;
    thread_num_ = ComParam::Num_Of_Threads;//????
    ptr_all_NNInput2Id_ = new ComType::Neural_Input_Map_Vect;
    pptr_param_l_ = nullptr;
    is_training_ = is_training;
}

GlobalNeuralNetworkParam::~GlobalNeuralNetworkParam() {
    if(is_training_){
        /*this instance is allocated in training phase*/
        delete ptr_call_dynet_;
    }
    for (auto it = ptr_all_NNInput2Id_->begin(); it != ptr_all_NNInput2Id_->end(); ++it) {
        /* caution: only release the map */
        if(nullptr != (*it)){
            delete (*it);
        }
    }
    if(nullptr != ptr_all_NNInput2Id_){
        delete ptr_all_NNInput2Id_;
    }
}

//set the learning state
void GlobalNeuralNetworkParam::SetLearningState() {
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        (*it)->SetTraining(true);
    }
}

void GlobalNeuralNetworkParam::InitNetwork() {
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        // (*it)->Initialize();
    }
}

double GlobalNeuralNetworkParam::GetNNScore(Network *ptr_network, int parent_k, int children_k_index) {
    double score = 0;
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        score += (*it)->GetScore(ptr_network, parent_k, children_k_index);
    }
    return score;
}

/**
 *
 * Update the gradient of the NN's features.
 *
 * @param count: the gradient for the children_k_index th hyper-edge that is rooted by parent_k
 * @param ptr_network: the graphical network.
 * @param parent_k: the rooted node.
 * @param children_k_index: the no of the hyper-edge that is rooted by parent_k.
 *
 */
void GlobalNeuralNetworkParam::UpdateNNGradientOutput(double count, Network *ptr_network, int parent_k,
                                                      int children_k_index) {
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        //set the output of NN
        (*it)->Update(count, ptr_network, parent_k, children_k_index);
    }
}

void GlobalNeuralNetworkParam::ResetAllNNGradient() {
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        (*it)->ResetGrad();
    }
}

/**
 * Init the neural network.
 *
 * @param ptr_nn_vec
 * @param max_len
 * @param ptr_word2int_map
 * @param ptr_label
 * @param ptr_dict
 */
void GlobalNeuralNetworkParam::Initialization(int max_len, std::unordered_map<std::string, int> *ptr_word2int_map,
                                              std::vector<std::string> *ptr_label, dynet::Dict *ptr_dict) {
    int id = 0;
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        /*set the id of each neural network*/
        (*it)->SetNNID(id);
        /*allocate the space for output and the corresponding gradient*/
        (*it)->AllocateNeuralParamSpace();
        /* sum the feature size of all neural networks, i.e., the output_size*/
        nn_params_size_ += (*it)->GetParamSize();
        (*it)->SetMaxSentenceLength(max_len);
        (*it)->SetWord2IntMap(ptr_word2int_map);
        (*it)->SetLabelSize(ptr_label->size());
        (*it)->SetDict(ptr_dict);
        ++id;
    }
}

void GlobalNeuralNetworkParam::SetNeuralNetwork(std::vector<NeuralNetwork *> *ptr_nn_vec) {
    ptr_nn_vec_ = ptr_nn_vec;
}

void GlobalNeuralNetworkParam::Forward() {
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        (*it)->Forward();
    }
}

void GlobalNeuralNetworkParam::Backward() {
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        (*it)->BackWard();
    }
}

void
GlobalNeuralNetworkParam::InitNNParameter(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    ptr_call_dynet_->Initialize(argc, argv, random_seed, shared_parameters);
}

DynetFunctionHelper *GlobalNeuralNetworkParam::GetDynetFunctionHelper() {
    return ptr_call_dynet_;
}

void GlobalNeuralNetworkParam::SetLocalNetworkParams(LocalNetworkParam **pptr_param_l) {
    pptr_param_l_ = pptr_param_l;
    auto end = ptr_nn_vec_->end();
    for (auto it = ptr_nn_vec_->begin(); it != end; ++it) {
        (*it)->SetLocalNetworkParams(pptr_param_l);
    }
}

/**
 *  copy all the maps of NNInputId from local to global, and we need to merge
 *  all maps.
 *
 *  All copies are based on C++ pointer and hence it is much more efficient than value copy.
 */
void GlobalNeuralNetworkParam::PrepareInputId() {
    int net_size = ptr_nn_vec_->size();
    for(int net_id =0; net_id < net_size; ++net_id){
        ptr_all_NNInput2Id_->push_back(new ComType::Neural_Input_Map);
    }
    for (int thread_no = 0; thread_no < thread_num_; ++thread_no) {
        for (int net_id = 0; net_id < net_size; ++net_id) {
            /* we need to copy all maps of NN2ID from LocalNetworkParam */
            LocalNetworkParam *ptr_pram_l = pptr_param_l_[thread_no];
            ComType::Neural_Input_Map_Vect *ptr_local = ptr_pram_l->GetLocalNNInput2Id();
            ComType::Neural_Input_Map *ptr_local_map = (*ptr_local)[net_id];
#ifdef DEBUG_NN_
            for (auto it = ptr_local_map->begin(); it != ptr_local_map->end(); ++it) {
                std::cout << "prepareInput: map value is " << (*it).second << " , sentence is: ";
                for (auto itt = (*it).first->begin(); itt != (*it).first->end(); ++itt) {
                    std::cout << (*itt) << " ";
                }
                std::cout << std::endl;
            }
#endif
            /* copy from ptr_local_map to global map (*ptr_all_NNInput2Id_)[net_id] */
            (*ptr_all_NNInput2Id_)[net_id]->insert(ptr_local_map->begin(), ptr_local_map->end());
        }
    }
    /* copy all map of NNInput2Id to the neural network */
    for (int net_id = 0; net_id < net_size; ++net_id) {
        ComType::Neural_Input_Map *ptr_map = (*ptr_nn_vec_)[net_id]->GetNNInput2IdMap();
        ptr_map->clear();
        ComType::Neural_Input_Map *ptr_all_map_id = (*ptr_all_NNInput2Id_)[net_id];
        int input_id = 0;
        for (auto it = ptr_all_map_id->begin(); it != ptr_all_map_id->end(); ++it) {
            ComType::Input_Str_Vector *ptr_input = (*it).first;
//            for (auto itt = (*it).first->begin(); itt != (*it).first->end(); ++itt) {
//                std::cout << (*itt) << " ";
//            }
//            std::cout << std::endl;
            ptr_map->insert(std::make_pair(ptr_input, input_id));
            ++input_id;
        }
    }
}

std::vector<NeuralNetwork *> *GlobalNeuralNetworkParam::GetNNVect() {
    return ptr_nn_vec_;
}

bool GlobalNeuralNetworkParam::IsLearningState() {
    bool flag = (*ptr_nn_vec_)[0]->IsTraining();
    return flag;
}

int GlobalNeuralNetworkParam::GetNeuralOutputSize() {
    return nn_output_size_;
}

int GlobalNeuralNetworkParam::GetMaxSentenceLength(int netId) {
    int max_len = (*ptr_nn_vec_)[netId]->GetMaxSentenceLength();
    return max_len;
}

void GlobalNeuralNetworkParam::SetInstance(std::vector<Instance *> *ptr_inst) {
    std::cout << "the size of sentence is"<<ptr_inst->size()<<std::endl;
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        (*it)->SetInstance(ptr_inst);
    }
}

void GlobalNeuralNetworkParam::SetMaxSentenceLength(int max_len) {
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        (*it)->SetMaxSentenceLength(max_len);
    }
}

/**
 *
 * Allocate the memory for the output array of the Neural network
 *
 */
void GlobalNeuralNetworkParam::AllocateOutSpaceBeforehand() {
    nn_output_size_ = 0;
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it) {
        if ((*it)->IsAllocateOutputBeforehand()) {
            (*it)->AllocateOutputSpace();
        }
        nn_output_size_ += (*it)->GetNNOutputSize();
    }
    std::cout << "nn_output_size_ is "<<nn_output_size_<<std::endl;
}

int GlobalNeuralNetworkParam::GetNeuralParamsSize() {
    return nn_params_size_;
}

void GlobalNeuralNetworkParam::Regularization(double coef, double kappa) {
    for (auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        (*it)->Regularization(coef, kappa);
    }
}

/**
 * Set the memory address of param array and gradient array.
 *oti
 * @param ptr_param
 * @param ptr_param_grad
 */
void GlobalNeuralNetworkParam::SetMemoryOfParamAndGradient(double *ptr_param, double *ptr_param_grad) {
    int i = 0;
    int size = ptr_nn_vec_->size();
    while (true){
        (*ptr_nn_vec_)[i]->SetMemoryOfParamAndGradient(ptr_param,ptr_param_grad);
        if(i >= size-1){
            return;
        } else{
            //caution: this is very dangerous due to the directed memory operation, and a conditional
            //check $if(i >= size-1)$ is able to secure a valid memory access.
            ptr_param += (*ptr_nn_vec_)[i]->GetParamSize();
            ptr_param_grad += (*ptr_nn_vec_)[i]->GetParamSize();
            ++i;
        }
    }
}

/**
 * set the decoding flag in the decoding phase
 */
void GlobalNeuralNetworkParam::SetDecodeState() {
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        (*it)->SetTraining(false);
    }
}

void GlobalNeuralNetworkParam::ResetNNBeforeDecode() {
    for (auto it = ptr_all_NNInput2Id_->begin(); it != ptr_all_NNInput2Id_->end(); ++it) {
        /* caution: only release the map */
        delete (*it);
    }
    ptr_all_NNInput2Id_->clear();
    for (int net_id = 0; net_id <  ptr_nn_vec_->size(); ++net_id) {
        ComType::Neural_Input_Map *ptr_map = (*ptr_nn_vec_)[net_id]->GetNNInput2IdMap();
        ptr_map->clear();
    }
    for(auto it = ptr_nn_vec_->begin(); it != ptr_nn_vec_->end(); ++it){
        (*it)->ReleaseOutputGradSpace();
    }
}