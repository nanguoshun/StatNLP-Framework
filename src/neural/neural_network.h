//
// Created by  ngs on 31/08/2018.
//
#ifndef STATNLP_NEURAL_LAYER_H
#define STATNLP_NEURAL_LAYER_H

#include <dynet/dynet/dynet.h>
#include "../../dynet/dynet/init.h"
#include "src/common/types/common.h"
#include "dynet_interface.h"
#include "neural_input_output.h"
#include <boost/functional/hash.hpp>
#include "src/common/types/instance.h"
#include "../../dynet/dynet/dict.h"

class Network;
class LocalNetworkParam;
namespace neural_layer{
    enum NeuralType{
        LSTM,CNN
    };
}

class NeuralNetwork{
public:
    NeuralNetwork();
    ~NeuralNetwork();
    void Initialize(int &argc, char **&argv, unsigned random_seed = 0, bool shared_parameters = false);
    void AllocateNeuralParamSpace();
    int GetNNOutputSize();
    void ReleaseOutputGradSpace();
    void AddParam();
    virtual void Touch();
    virtual void Forward();
    //back propogation;
    virtual void BackWard();
    //update the model
    virtual void Update(double count, Network *ptr_network, int parent_k, int children_k_index);
    void SetTraining(bool istraining);
    double GetL2Param();
    void ResetGrad();
    double GetScore(Network *ptr_network, int parent_k, int childeren_k_index);
    void CopyParams2Dynet(bool printtest = false);
    void CopyGradientFromDynet();
    virtual dynet::Expression BuildForwardGraph(std::vector<std::vector<std::string>*> *pptr_sent) = 0;
    void SetOutputArray(std::vector<dynet::real> &output_vec);
    void AllocateOutputSpace();
    void ResetGradient();
    void AddL2ParamGrad();
    int GetParamSize();
    void SetScale(double scale);
    void SetLocalNetworkParams(LocalNetworkParam **ppr_params);
    virtual int HyperEdgeInput2OutputRowIndex(NeuralIO *ptr_io) = 0;
    virtual ComType::Input_Str_Vector* HyperEdgeInput2NNInput(void *ptr_edgeInput) = 0;
    NeuralIO *GetHyperEdgeInputOutput(Network *ptr_network, int parent_k, int children_k_index);
    int GetNNInputID(ComType::Input_Str_Vector *ptr_input);
    ComType::Neural_Input_Map *GetNNInput2IdMap();
    bool IsTraining();
    int GetNNID();
    void SetNNID(int id);
    void SetMaxSentenceLength(int max_len);
    int GetMaxSentenceLength();
    virtual void SetInstance(std::vector<Instance *> *ptr_inst) = 0;
    void SetWord2IntMap(std::unordered_map<std::string, int> *ptr_map);
    int GetNNInputSize();
    void SetLabelSize(int size);
    bool IsAllocateOutputBeforehand();
    void SetAllocateOutputBeforehand(bool flag);
    void SetDict(dynet::Dict *ptr_dict);
    dynet::Dict *GetDict();
    void Regularization(double coef, double kappa);
    void SetMemoryOfParamAndGradient(double *ptr_param, double *ptr_param_grad);
    //int GetNNInputId(ComType::Neural_Input *ptr_input);
protected:
    //double *ptr_counts_;
    //double *ptr_weights_;
    LocalNetworkParam **pptr_param_l_;
    bool is_training_;
    int argc_;
    char **argv_;
    double *ptr_params_, *ptr_params_grad_;
    int param_size_;
    double *ptr_output_;
    double *ptr_output_grad_;
    std::vector<float> *ptr_output_grad_vec_;
    //output array and its size
    //actually it is the feature size
    int output_size_;
    int label_size_;
    dynet::ComputationGraph *ptr_cg_;
    dynet::Expression output_expression_;
    std::vector<std::vector<std::string>*> *pptr_sent_;
    dynet::ParameterCollection *ptr_model_;
    bool is_create_grad_;
    ComType::Neural_Input_Map *ptr_nnInput2Id_;
    //std::unordered_map<ComType::NeuralInput, int, boost::hash<ComType::NeuralInput>> *ptr_nnInput2Id_;
    /**
	 * The coefficient used for regularization, i.e., batchSize/totalInstNum.
	 */
    double scale_;
    /*the id fo this neural network*/
    int net_id_;
    int max_len_;
    std::unordered_map<std::string, int> *ptr_word2int_map_;
    bool is_alloc_space_beforehand_;
    dynet::Dict *ptr_dict_;
    bool started_forward_;
    int iteration_count_;
};

#endif //STATNLP_NEURAL_LAYER_H
