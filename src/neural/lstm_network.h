//
// Created by  ngs on 01/09/2018.
//

#ifndef STATNLP_LSTM_H
#define STATNLP_LSTM_H

#include "dynet/dynet/model.h"
#include "dynet/dynet/expr.h"
#include "dynet/dynet/dynet.h"
#include "dynet/dynet/dict.h"
#include "neural_network.h"
#include "vanilla_rnn.h"
#include "dynet/lstm.h"
#include "src/common/common.h"
//#include "dynet/examples/cpp-utils/cl-args.h"
#include "dynet_interface.h"
#include "src/common/types/base_instance.h"

class LSTMNetwork: public VanillaRNN, public dynet::LSTMBuilder,public BaseInstance<LSTMNetwork, ComType::Input_Str_Matrix, ComType::Label_Str_Vector>{
public:
    LSTMNetwork(LSTMSuperParam &param);
    LSTMNetwork();
    //LSTMNetwork(int& argc, char**& argv);
    ~LSTMNetwork();
    void Touch() override;
    //void Forward();
    void BackWard() override;
    void Update(double count, Network *ptr_network, int parent_k, int children_k_index) override;
    dynet::Expression GetOutput(const std::vector<int> &sentence, int sent_idx,dynet::ComputationGraph &cg,
                                                  bool apply_dropout);
    dynet::Expression BuildLSTMGraph(const std::vector<int>& sentence, dynet::ComputationGraph &cg, bool apply_dropout);
    unsigned ReadData(const std::string& filename, std::vector<std::vector<int>>& data);
    void Initialize(int &argc, char **&argv);
    dynet::DynetParams ExtractParam(int& argc, char**& argv);
    void AddParameters(dynet::ParameterCollection &model, LSTMSuperParam &param);

    dynet::Expression BuildForwardGraph(std::vector<std::vector<std::string>*> *ptr_input) override;

    int HyperEdgeInput2OutputRowIndex(void *ptr_edgeInput,int output_label) override;

    ComType::Input_Str_Vector *HyperEdgeInput2NNInput(void *ptr_edgeInput) override;

    void SetInstance(std::vector<Instance *> *ptr_inst) override;
    dynet::Expression BuildLMGraph2(const vector<int>& sent, dynet::ComputationGraph* ptr_cg, bool apply);

private:
    dynet::LookupParameter p_c_;
    dynet::Parameter p_R_;
    dynet::Parameter p_bias_;
    dynet::Expression *ptr_loss_expr_;
    dynet::ParameterCollection *ptr_model_;
    dynet::Dict dict_;
    float drop_out_;
    unsigned report_every_i_;
    std::vector<std::vector<int>> *ptr_training_, *ptr_dev_, *ptr_test_;
    LSTMSuperParam lstm_param_;
    std::vector<std::vector<std::vector<dynet::real>>> *ppptr_output_matrix_;
    std::vector<std::vector<std::vector<dynet::real>>> *ppptr_gradient_;
};


#endif //STATNLP_LSTM_H
