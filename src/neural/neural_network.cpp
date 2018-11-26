//
// Created by  ngs on 31/08/2018.
//

#include <cstring>
#include "src/common/opt/math_calc.h"
#include "neural_network.h"
#include "src/hybridnetworks/network.h"

NeuralNetwork::NeuralNetwork() {
    is_training_ = false;
    ptr_params_ = nullptr;
    ptr_params_grad_ = nullptr;
    ptr_output_ = nullptr;
    ptr_output_grad_ = nullptr;
    ptr_cg_ = nullptr;
    is_create_grad_ = false;
    ptr_model_ = new dynet::ParameterCollection;
    ptr_nnInput2Id_ = new ComType::Neural_Input_Map;
    pptr_sent_ = new std::vector<std::vector<std::string> *>;
    ptr_output_grad_vec_ = new std::vector<float>;
    is_alloc_space_beforehand_ = false;
    started_forward_ = false;
    iteration_count_ = 0;
}

NeuralNetwork::~NeuralNetwork() {
    delete ptr_model_;
    delete ptr_nnInput2Id_;
    delete []ptr_output_;
    delete []ptr_output_grad_vec_;
    delete pptr_sent_;
    delete []ptr_params_;
    delete []ptr_params_grad_;
}

void NeuralNetwork::Initialize(int &argc, char **&argv, unsigned int random_seed, bool shared_parameters) {
    //dynet::initialize(argc,argv);
}

void NeuralNetwork::AllocateNeuralParamSpace() {
    ptr_params_ = new double[param_size_];
    ptr_params_grad_ = new double[param_size_];
    //initialize the value of params and corresponding gradient.
    for (int i = 0; i < param_size_; ++i) {
        ptr_params_[i] = 0;
        ptr_params_grad_[i] = 0;
    }
}

void NeuralNetwork::AddParam() {

}

/**
 * return the feature size: actually the output size.
 * @return
 */
int NeuralNetwork::GetNNOutputSize() {
    return output_size_;
}

void NeuralNetwork::Touch() {

}


void NeuralNetwork::Forward() {
    iteration_count_++;
    //call back the forward function
//    std::cout <<  NetworkConfig::STATUS <<std::endl;
//    std::cout <<     ComType::ModelStatus::TESTING <<std::endl;
//    if(is_training_ || NetworkConfig::STATUS == ComType::ModelStatus::TESTING || NetworkConfig::STATUS == ComType::ModelStatus::DEV_IN_TRAINING){
    //copy parames from statNLP to dynet.
    if (started_forward_) {
        CopyParams2Dynet();
    } else {
        started_forward_ = true;
    }
    //  }
    if (NetworkConfig::USE_BATCH_TRAINING && is_training_) {
        //TODO:
    } else {
        if(nullptr != ptr_cg_){
            delete ptr_cg_;
            ptr_cg_ = nullptr;
        }
        ptr_cg_ = new dynet::ComputationGraph();//DynetFunctionHelper::NewGraph(ptr_cg_);
        output_expression_ = BuildForwardGraph(pptr_sent_); // is it a completed sentence???
        dynet::Tensor output_tensor = ptr_cg_->forward(output_expression_);
#ifdef DEBUG_NN
        std::cout << "the dim of output_tensor "<<output_tensor.d<<std::endl;
        std::cout << "the acess element of the tensor is "<<dynet::TensorTools::access_element(output_tensor,1)<<std::endl;
#endif
        std::vector<dynet::real> output_vect = dynet::as_vector(output_tensor);
#ifdef DEBUG_NN
        std::cout << "the size of output vector is "<<output_vect.size()<<std::endl;
#endif
        SetOutputArray(output_vect);
    }
}

void NeuralNetwork::BackWard() {
    //call the back propagation of NN.
    ResetGradient();
    //copy the gradient of output to ptr_output_grad_vec_
    for (int i = 0; i < output_size_; ++i) {
//        ptr_output_grad_vec_->push_back(1);
        ptr_output_grad_vec_->push_back((float) ptr_output_grad_[i]);
#ifdef DEBUG_NN
        std::cout << "the value of gradient is"<<ptr_output_grad_[i]<<std::endl;
#endif
    }
#ifdef DEBUG_NN
    std::cout << "dim of output_expression_ is" <<output_expression_.dim()<<std::endl;
#endif
    dynet::Expression output_grad_exp = dynet::input(*ptr_cg_, output_expression_.dim(), *ptr_output_grad_vec_);
#ifdef DEBUG_NN
    std::cout << "dim of output_grad_exp is" <<output_grad_exp.dim()<<std::endl;
#endif
    dynet::Expression loss_exp = dynet::sum_elems(
            dynet::sum_batches(dynet::cmult(output_expression_, output_grad_exp)));
//    dynet::Expression cmult_exp = dynet::cmult(output_expression_,output_grad_exp);
#ifdef DEBUG_NN
    std::cout << "dim of cmult_exp is" <<loss_exp.dim()<<std::endl;
#endif
//    dynet::Expression loss_exp = dynet::sum_elems(cmult_exp);
//    dynet::Expression final_loss = dynet::(loss_exp);
#ifdef DEBUG_NN
    std::cout << "dim of loss_exp is" <<loss_exp.dim()<<std::endl;
#endif
    dynet::Tensor tensor = ptr_cg_->forward(loss_exp);
    dynet::real value = dynet::as_scalar(tensor);
#ifdef DEBUG_NN
    std::cout << "value of loss is" <<value<<std::endl;
#endif
    ptr_cg_->backward(loss_exp);
    //copy gradient from dynet to our framework.
    CopyGradientFromDynet();
    //L2 regularization.
    if (NetworkConfig::REGULARIZE_NEURAL_FEATURES) {
        AddL2ParamGrad();
    }
    delete ptr_cg_;
    ptr_cg_ = nullptr;
}

/**
 * Copy params from statNLP to neural params of dynet.
 */
void NeuralNetwork::CopyParams2Dynet(bool printtest) {
    std::vector<std::shared_ptr<dynet::LookupParameterStorage>> lookup_params = ptr_model_->lookup_parameters_list();
    std::vector<std::shared_ptr<dynet::ParameterStorage>> params = ptr_model_->parameters_list();
    int param_index = 0;/* the index of output gradient */
#ifdef DEBUG_NN
    int all_param_size = ptr_model_->size();
    std::cout << "the param size of the model is "<<all_param_size<<std::endl;
    int count = ptr_model_->parameter_count();
    std::cout << "the param cout of the model is "<<count<<std::endl;
    int all_lookup_size = 0;
    for(auto it = lookup_params.begin(); it != lookup_params.end(); ++it) {
        dynet::LookupParameterStorage *ptr_lps = (*it).get();
        int size = ptr_lps->size();
        std::cout << "lookup params size is: "<<size<<std::endl;
        all_lookup_size += size;
    }
    std::cout << "all lookup size is "<<all_lookup_size<<std::endl;
    int param_size = 0;
    for(auto it = params.begin(); it != params.end(); ++it){
        dynet::ParameterStorage *ptr_ps = (*it).get();
        int size = ptr_ps->size();
        std::cout <<"param size is: "<<size<<std::endl;
        param_size += size;
    }
    std::cout << "params size is: "<<param_size<<std::endl;
#endif
    int size_lookup = 0;
    // copy lookup params to neural params.
    for (auto it = lookup_params.begin(); it != lookup_params.end(); ++it) {
        dynet::LookupParameterStorage *ptr_lps = (*it).get();
        dynet::Tensor tensor = ptr_lps->all_values;
        int lps_size = ptr_lps->size();
        for (int i = 0; i < lps_size; ++i) {
            ++size_lookup;
            //std::cout << size_lookup <<"th lookupparams, param before: "<<ptr_params_[param_index];
            if (printtest) {
                if (size_lookup < 10) {
                    std::cout << size_lookup << "th params before: " << ptr_params_[param_index]<<std::endl;
                }
            } else {
       //             std::cout << size_lookup << "th params is: " << ptr_params_[param_index] <<std::endl;
                    dynet::TensorTools::set_element(tensor, i, ptr_params_[param_index]);
            }
            ++param_index;
            }
        }
        //std::cout << "all lookup size is "<<size_lookup<<std::endl;
        size_lookup = 0;
        //copy params to neural params.
        for (auto it = params.begin(); it != params.end(); ++it) {
            dynet::ParameterStorage *ptr_ps = (*it).get();
            dynet::Tensor tensor = ptr_ps->values;
            int ps_size = ptr_ps->size();
            for (int i = 0; i < ps_size; ++i) {
                ++size_lookup;
                if (printtest) {
                    if (size_lookup < 10) {
                        std::cout << size_lookup << "th params before: " << ptr_params_[param_index]<<std::endl;
                    }
                } else {
                    dynet::TensorTools::set_element(tensor, i, ptr_params_[param_index]);
                }
                // std::cout << ", params after: "<<ptr_params_[param_index]<<std::endl;
                ++param_index;
            }
        }
        //std::cout << "all params size is "<<size_lookup<<std::endl;
    }

/**
 *
 * Copy gradient from dynet to StatNLP.
 *
 */
    void NeuralNetwork::CopyGradientFromDynet() {
        std::vector<std::shared_ptr<dynet::LookupParameterStorage>> lookup_params = ptr_model_->lookup_parameters_list();
        std::vector<std::shared_ptr<dynet::ParameterStorage>> params = ptr_model_->parameters_list();
        int param_index = 0; /* the index of output gradient */
        //copy the gradient of lookupparam to gradient vector.
        for (auto it = lookup_params.begin(); it != lookup_params.end(); ++it) {
            dynet::LookupParameterStorage *ptr_lps = (*it).get();
            dynet::Tensor grad_tensor = ptr_lps->all_grads;
            std::vector<dynet::real> grad_vec = dynet::as_vector(grad_tensor);
            int size = grad_vec.size();
            for (int i = 0; i < size; ++i) {
                double grad_value = (double) grad_vec[i];
#ifdef DEBUG_NN_
                // std::cout << i <<"th lookup gradient value is "<<grad_value << std::endl;
#endif
                ptr_params_grad_[param_index++] = grad_value;
            }
        }
        //copy the gradient of params to gradient vector

        for (auto it = params.begin(); it != params.end(); ++it) {
            dynet::ParameterStorage *ptr_ps = (*it).get();
            dynet::Tensor grad_tensor = ptr_ps->g;
            std::vector<dynet::real> grad_vec = dynet::as_vector(grad_tensor);
            int size = grad_vec.size();
            for (int i = 0; i < size; ++i) {
                double grad_value = (double) grad_vec[i];
#ifdef DEBUG_NN_
                // std::cout << i <<"th lookup gradient value is "<<grad_value << std::endl;
#endif
                ptr_params_grad_[param_index++] = grad_value;
            }
        }
        //std::cout << "the size of gradient is "<<param_index<<std::endl;
    }

/**
 * Update the gradient of the output.
 *
 * @param count
 * @param ptr_network
 * @param parent_k
 * @param children_k_index
 */
    void NeuralNetwork::Update(double count, Network *ptr_network, int parent_k, int children_k_index) {
        double val = 0;
        NeuralIO *ptr_io = GetHyperEdgeInputOutput(ptr_network, parent_k, children_k_index);
        if (nullptr != ptr_io) {
            int idx = HyperEdgeInput2OutputRowIndex(ptr_io);
            ptr_output_grad_[idx] -= count; //caution: may have scyornlized issues for MLP
        }
    }

    void NeuralNetwork::SetTraining(bool istraining) {
        is_training_ = istraining;
    }
/**
 * Get the L2 param. Reguliraztion for internal params of NN.
 * @return
 */
    double NeuralNetwork::GetL2Param() {
        if (nullptr != ptr_params_) {
            return MathCalc::SquareVector(ptr_params_, param_size_); //FIXME: WHY?
        } else {
            return 0.0;
        }
    }

/**
 * Reset all gradient to zero.
 */

    void NeuralNetwork::ResetGrad() {
        //reset the gradient of features that used in CRF
        if (nullptr != ptr_output_grad_) {
            memset(ptr_output_grad_, 0, sizeof(double) * output_size_);
        }
        //reset the neural internal gradient.
        if (nullptr != ptr_params_grad_) {
            memset(ptr_params_grad_, 0, sizeof(double) * param_size_);
        }
    }

/**
 *
 * Get the neural score of a hyper-edge.
 *
 * @param ptr_network
 * @param parent_k: the parent node of the hyperedge
 * @param childeren_k_index: the  childeren_k_index th hyperedge.
 * @return: the score
 *
 */
    double NeuralNetwork::GetScore(Network *ptr_network, int parent_k, int childeren_k_index) {
        double val = 0;
        NeuralIO *ptr_io = GetHyperEdgeInputOutput(ptr_network, parent_k, childeren_k_index);
        if (nullptr != ptr_io) {
            int index = HyperEdgeInput2OutputRowIndex(ptr_io);// * label_size_ + output_label ;
            val = ptr_output_[index];
        }
        return val;
    }
/**
 * Copy the output from dynet to an array
 *
 * @param output_vec
 */

    void NeuralNetwork::SetOutputArray(std::vector<dynet::real> &output_vec) {
        //release the space allocated beforehand.
        if (!is_alloc_space_beforehand_) {
            if (nullptr != ptr_output_) {
                delete ptr_output_;
            }
            if (nullptr != ptr_output_grad_) {
                delete ptr_output_grad_;
            }
            output_size_ = output_vec.size();
            ptr_output_ = new double[output_size_];
            ptr_output_grad_ = new double[output_size_];
        }
        int i = 0;
        auto end = output_vec.end();
        for (auto it = output_vec.begin(); it != end; ++it) {
            double value = (*it);
            //std::cout << i<<"th value of the output is "<<value<<std::endl;
            ptr_output_[i++] = (*it);
        }
    }

    void NeuralNetwork::ReleaseOutputGradSpace() {
        delete [] ptr_output_;
        delete [] ptr_output_grad_;
    }

    /**
     * Allocate space for output array and gradient array.
     */
    void NeuralNetwork::AllocateOutputSpace() {
        int num_of_sentence = pptr_sent_->size();
        output_size_ = num_of_sentence * max_len_ * label_size_;
        ptr_output_ = new double[output_size_];
        ptr_output_grad_ = new double[output_size_];
        for (int i = 0; i < output_size_; ++i) {
            ptr_output_[i] = 0;
            ptr_output_grad_[i] = 0;
        }
        std::cout << "output size is: "<<output_size_<<std::endl;
    }

/**
 *
 * Reset all gradient as zero before execute backpropagation.
 *
 */
    void NeuralNetwork::ResetGradient() {
        std::vector<std::shared_ptr<dynet::LookupParameterStorage>> lookup_params = ptr_model_->lookup_parameters_list();
        std::vector<std::shared_ptr<dynet::ParameterStorage>> params = ptr_model_->parameters_list();

        for (auto it = lookup_params.begin(); it != lookup_params.end(); ++it) {
            dynet::LookupParameterStorage *ptr_lps = (*it).get();
            dynet::TensorTools::zero(ptr_lps->all_grads);
        }

        for (auto it = params.begin(); it != params.end(); ++it) {
            dynet::ParameterStorage *ptr_ps = (*it).get();
            dynet::TensorTools::zero(ptr_ps->g);
        }
        if (ptr_output_grad_vec_->size() > 1) {
            ptr_output_grad_vec_->clear();
        }
    }

    void NeuralNetwork::AddL2ParamGrad() {
        if (param_size_ > 0) {
            double kappa = NetworkConfig::L2_REGULARIZATION_CONSTANT;
            if (kappa <= 0) {
                return;
            }
            for (int i = 0; i < param_size_; ++i) {
                ptr_params_grad_[i] += 2 * scale_ * kappa * ptr_params_[i];
            }
        }
    }

    int NeuralNetwork::GetParamSize() {
        return param_size_;
    }

    void NeuralNetwork::SetScale(double scale) {
        scale_ = scale;
    }

    void NeuralNetwork::SetLocalNetworkParams(LocalNetworkParam **ppr_params) {
        pptr_param_l_ = ppr_params;
    }

    int NeuralNetwork::GetNNInputID(ComType::Input_Str_Vector *ptr_input) {
        int id = 0;
        if (NetworkConfig::USE_BATCH_TRAINING && is_training_) {
            //TODO:
        } else {
            //int id = ptr_nnInput2Id_->find (*ptr_input)->second;
            //return id;
            id = ptr_nnInput2Id_->find(ptr_input)->second;
        }
        return id;
    }

    /**
     *
     * @param ptr_network: a graph b sentence)
     * @param parent_k
     * @param children_k_index
     * @return
     */
    NeuralIO *NeuralNetwork::GetHyperEdgeInputOutput(Network *ptr_network, int parent_k, int children_k_index) {
        return pptr_param_l_[ptr_network->GetThreadId()]->GetHyperEdgeIO(ptr_network, net_id_, parent_k,
                                                                         children_k_index);
    }

    bool NeuralNetwork::IsTraining() {
        return is_training_;
    }

    void NeuralNetwork::SetNNID(int id) {
        net_id_ = id;
    }

    int NeuralNetwork::GetNNID() {
        return net_id_;
    }

    ComType::Neural_Input_Map *NeuralNetwork::GetNNInput2IdMap() {
        return ptr_nnInput2Id_;
    }

    int NeuralNetwork::GetMaxSentenceLength() {
        return max_len_;
    }

    void NeuralNetwork::SetMaxSentenceLength(int max_len) {
        max_len_ = max_len;
    }

    void NeuralNetwork::SetWord2IntMap(std::unordered_map<std::string, int> *ptr_map) {
        ptr_word2int_map_ = ptr_map;
    }
/**
 * The num of sentence in the dataset.
 *
 * @return
 */
    int NeuralNetwork::GetNNInputSize() {
        int size = ptr_nnInput2Id_->size();
        return size;
    }

    void NeuralNetwork::SetLabelSize(int label_size) {
        label_size_ = label_size;
    }

    bool NeuralNetwork::IsAllocateOutputBeforehand() {
        return is_alloc_space_beforehand_;
    }

    void NeuralNetwork::SetAllocateOutputBeforehand(bool flag) {
        is_alloc_space_beforehand_ = flag;
    }

    void NeuralNetwork::SetDict(dynet::Dict *ptr_dict) {
        ptr_dict_ = ptr_dict;
    }

    dynet::Dict *NeuralNetwork::GetDict() {
        return ptr_dict_;
    }

    void NeuralNetwork::Regularization(double coef, double kappa) {
        for (int i = 0; i < output_size_; ++i) {
            ptr_output_grad_[i] = 0;
            ptr_output_grad_[i] += 2 * coef * kappa * ptr_output_[i];
        }
    }

    void NeuralNetwork::SetMemoryOfParamAndGradient(double *ptr_param, double *ptr_param_grad) {
        ptr_params_ = ptr_param;
        ptr_params_grad_ = ptr_param_grad;
    }
