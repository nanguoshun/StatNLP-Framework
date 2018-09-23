//
// Created by  ngs on 01/09/2018.
//

#include <assert.h>
#include <fstream>
#include "lstm_network.h"
//#include </usr/local/include/thunder/tensor.hpp>
//#include <thunder/tensor.hpp>
#include "dynet/dynet/tensor.h"

LSTMNetwork::LSTMNetwork() {

}

LSTMNetwork::LSTMNetwork(LSTMSuperParam &param):dynet::LSTMBuilder(param.layers_,param.input_dim_,param.hidden_dim_,*NeuralNetwork::ptr_model_) {
    AddParameters(*NeuralNetwork::ptr_model_,param);
    ppptr_output_matrix_ = new std::vector<std::vector<std::vector<dynet::real>>>;
}

LSTMNetwork::~LSTMNetwork() {
    delete ptr_model_;
    delete ppptr_output_matrix_;
}

void LSTMNetwork::AddParameters(dynet::ParameterCollection &model, LSTMSuperParam &param) {
    p_c_ = model.add_lookup_parameters(param.vocab_size_,{param.input_dim_});
    p_R_ = model.add_parameters({param.vocab_size_,param.hidden_dim_});
    p_bias_ = model.add_parameters({param.vocab_size_});
    lstm_param_ = param;
    param_size_ = model.parameter_count();
}

void LSTMNetwork::Initialize(int &argc, char **&argv) {
    dynet::DynetParams dyparams = ExtractParam(argc,argv);
    NeuralNetwork::Initialize();
    Params params;
    get_args(argc,argv,params,Task::TRAIN);
    NetworkConfig::kSOS = dict_.convert("<s>");
    NetworkConfig::kEOS = dict_.convert("</s>");
    ptr_model_ = new dynet::ParameterCollection();
    ptr_training_ = new std::vector<std::vector<int>>;
    {
        std::string train_file_name = params.train_file;
        std::cerr << "Reading training data from " << train_file_name << " ...\n";
        int ttoks = ReadData(train_file_name, *ptr_training_);
        int tlc = ptr_training_->size();
        std::cerr << tlc << " lines, " << ttoks << " tokens, " << dict_.size() << " types\n";
    }
    dict_.freeze();
        dict_.set_unk("<unk>");
    //AddParameters(*ptr_model_,dict_.size(),params.LAYERS,params.INPUT_DIM,params.HIDDEN_DIM,params.dropout_rate);

}

dynet::DynetParams LSTMNetwork::ExtractParam(int &argc, char **&argv) {
    return dynet::extract_dynet_params(argc,argv);
}

void LSTMNetwork::Touch() {
    NeuralNetwork::Touch();
}


/**
 *
 * step1: update the parameter of LSTM model.
 * step2: calculate the output of LSTM network.
 *
 */
/*
void LSTMNetwork::Forward() {
   //copy the parameter from the optimizer and then get the forward results.
   //dynet::ParameterStorage storage();
   //thunder::DoubleTensor *p =  new thunder::DoubleTensor(3,9,7,10);
   //double a[2] = {1,2};
   //p->storage().get()->copy(a);
   if(is_training_ || NetworkConfig::STATUS == NetworkConfig::ModelStatus::TESTING || NetworkConfig::STATUS == NetworkConfig::ModelStatus::DEV_IN_TRAINING){

   }
   std::vector<std::shared_ptr<dynet::ParameterStorage>> param_storage = ptr_model_->get_storage().params;
   //param_storage[0].get()->copy()
   //param_storage[1]
   //dynet::ParameterStorage
   //std::shared_ptr<dynet::ParameterStorage> *ptr = param_storage[0].get()->copy();
//   std::vector<long> vec;
//    dynet::Dim *ptr_d = new dynet::Dim(vec);
//    dynet::Tensor *ptr_tensor= new dynet::Tensor(&ptr_d,);
   for(unsigned i = 0; i<report_every_i_; ++i){
       //build a computation graph for each sentence.
       dynet::ComputationGraph cg;
       //construct the parameters of NN model.
       //dynet::Parameter pw = ptr_model_->add_parameters({8,2});
       //construct an expression of loss function for each sentence.
//      dynet::Expression w = dynet::parameter(cg,);
      // dynet::Expression loss_exr = BuildLSTMGraph((*ptr_training_)[i],cg, lstm_param_.dropout_rate_ > 0.f);
       //call LSTM forward from dynet and get the output.
       ptr_output_ = nullptr; // get the output from LSTM output.
       output_size_ = 0; // should be a value.
       if(NeuralNetwork::is_training_){
           if(nullptr == ptr_output_grad_){
               ptr_output_grad_ = new double[output_size_];
               GetOutput((*ptr_training_)[i],i,cg, lstm_param_.dropout_rate_ > 0.f);
           }
       }
       int slen = (*ptr_training_)[i].size();
       std::vector<std::vector<dynet::real>> grad = (*ppptr_gradient_)[i];
       std::vector<dynet::real> grad_i = grad[i];
       std::vector<dynet::Expression> err(slen+1);
       for(int k=0; k<slen; ++k){

       }
   }
}
*/

/**
 *  1. manually set the output gradient of the LSTM
 *
 */
void LSTMNetwork::BackWard() {
    NeuralNetwork::BackWard();
    //ppptr_gradient_[]
    //dynet::Expression expr = gradient * output
}

void LSTMNetwork::Update(double count, Network *ptr_network, int parent_k, int children_k_index) {

}

dynet::Expression LSTMNetwork::GetOutput(const std::vector<int> &sentence, int sent_idx, dynet::ComputationGraph &cg,
                                         bool apply_dropout) {
    const unsigned slen = sentence.size();
    if(apply_dropout){
        dynet::LSTMBuilder::set_dropout(drop_out_);
    } else{
        dynet::LSTMBuilder::disable_dropout();
    }
    dynet::LSTMBuilder::new_graph(cg); // reset RNN builder for the new graph
    dynet::LSTMBuilder::start_new_sequence();
    //dynet::Expression R = dynet::parameter(cg,p_R_);
    //dynet::Expression bias = dynet::parameter(cg,p_bias_);
    //std::vector<std::vector<dynet::real>> output(slen + 1);
    dynet::Expression h_t;// = dynet::LSTMBuilder::add_input(dynet::lookup(cg,p_c_,NetworkConfig::kSOS));
    for(unsigned t = 0; t < slen; ++t){
//        dynet::Expression u_t = dynet::affine_transform({bias,R,h_t});
        dynet::Expression x_t = dynet::lookup(cg,p_c_,sentence[t]);
        h_t = dynet::LSTMBuilder::add_input(x_t);
        std::vector<dynet::real> output = dynet::as_vector(h_t.value());
        (*ppptr_output_matrix_)[sent_idx].push_back(output);
    }
}

dynet::Expression LSTMNetwork::BuildLSTMGraph(const std::vector<int> &sentence, dynet::ComputationGraph &cg,
                                              bool apply_dropout) {
    const unsigned slen = sentence.size();
    if(apply_dropout){
        dynet::LSTMBuilder::set_dropout(drop_out_);
    } else{
        dynet::LSTMBuilder::disable_dropout();
    }
    dynet::LSTMBuilder::new_graph(cg); // reset RNN builder for the new graph
    dynet::LSTMBuilder::start_new_sequence();
    dynet::Expression R = dynet::parameter(cg,p_R_);
    dynet::Expression bias = dynet::parameter(cg,p_bias_);
    std::vector<dynet::Expression> errs(slen + 1);
    //add the start as the input
    dynet::Expression h_t = dynet::LSTMBuilder::add_input(dynet::lookup(cg,p_c_,NetworkConfig::kSOS));
    for(unsigned t = 0; t < slen; ++t){
        dynet::Expression u_t = dynet::affine_transform({bias,R,h_t});//bias + R * h_t;
        errs[t] = dynet::pickneglogsoftmax(u_t,sentence[t]);
        dynet::Expression x_t = dynet::lookup(cg,p_c_,sentence[t]);
        //add input for each word?
        h_t = dynet::LSTMBuilder::add_input(x_t);
        //get the output for each input x_t;
        std::vector<dynet::real> output = dynet::as_vector(h_t.value());
        (*ppptr_output_matrix_)[t].push_back(output);
    }
    dynet::Expression u_last = dynet::affine_transform({bias,R,h_t});
    errs.back() = dynet::pickneglogsoftmax(u_last,NetworkConfig::kEOS);
    return dynet::sum(errs);
}

// Read the dataset, returns the number of tokens
unsigned LSTMNetwork::ReadData(const std::string &filename, std::vector<std::vector<int>> &data) {
    unsigned num_tokens = 0;
    std::ifstream in(filename);
    assert(in);
    size_t lc = 0;
    std::string line;
    while (std::getline(in, line)) {
        ++lc;
        data.push_back(dynet::read_sentence(line, dict_));
        num_tokens += data.back().size();
        if (data.back().front() == NetworkConfig::kSOS || data.back().back() == NetworkConfig::kEOS) {
            std::cerr << "sentence in " << filename << ":" << lc << " started with <s> or ended with </s>\n";
            abort();
        }
    }
    return num_tokens;
}

dynet::Expression LSTMNetwork::BuildForwardGraph(std::vector<std::vector<std::string>*> *pptr_sent) {
    dynet::Expression r_exp = dynet::parameter(*ptr_cg_,p_R_);
    dynet::Expression bais_exp = dynet::parameter(*ptr_cg_,p_bias_);
    dynet::LSTMBuilder::new_graph(*ptr_cg_); // reset RNN builder for the new graph
    dynet::LSTMBuilder::start_new_sequence();
    std::vector<dynet::Expression> err_vec;
    int num_of_sentence = pptr_sent->size();
    int word_id = 0;
    for(int pos = 0; pos < max_len_; ++pos){
        std::vector<unsigned int> word_id_vec;
        for(int i = 0; i < num_of_sentence; ++i){
            int sent_size = (*pptr_sent)[i]->size();
            if(pos > sent_size){
                word_id = 0;
                word_id_vec.push_back(word_id);
            } else{
                std::string word = (*((*pptr_sent)[i]))[pos];
                auto it = ptr_word2int_map_->find(word);
                if(it != ptr_word2int_map_->end()){
                    word_id = (*it).second;
                }else{
                    word_id = ptr_word2int_map_->find(ComParam::UNK)->second;
                }
                word_id_vec.push_back(word_id);
            }
        }
        dynet::Expression expr = dynet::lookup(*ptr_cg_,p_c_,word_id_vec);
        dynet::Expression expr_predict = dynet::LSTMBuilder::add_input(expr);
        err_vec.push_back(expr_predict);
    }
    dynet::Expression err_sum = dynet::sum(err_vec);
    return err_sum;
}

int LSTMNetwork::HyperEdgeInput2OutputRowIndex(void *ptr_edgeInput) {
    //TODO:
    return 0;
}

ComType::Input_Str_Vector *LSTMNetwork::HyperEdgeInput2NNInput(void *ptr_edgeInput) {
    ComType::Neural_Input *ptr_sent_and_pos = (ComType::Neural_Input *)ptr_edgeInput;
#ifdef DEBUG_NN
    std::cout << "sentence is: ";
    ComType::Input_Str_Vector *ptr_str_vec = ptr_sent_and_pos->first;
    for(auto it = ptr_str_vec->begin(); it != ptr_str_vec->end(); ++it){
        std::cout << (*it) << " ";
    }
    std::cout << std::endl;
#endif
    return ptr_sent_and_pos->first;
}

/**
 *
 * @param ptr_inst
 */
void LSTMNetwork::SetInstance(std::vector<Instance *> *ptr_inst) {
    for(auto it = ptr_inst->begin(); it != ptr_inst->end(); ++it){
        LSTMNetwork *ptr_base_inst = (LSTMNetwork *)(*it);
        ComType::Input_Str_Matrix *ptr_input = ptr_base_inst->GetInput();
        int sent_length = ptr_input->size();
        std::vector<std::string> *ptr_vec = new std::vector<std::string>;
        for(auto itt = ptr_input->begin(); itt != ptr_input->end(); ++itt){
            std::string feature1 = (*itt)[0];
            std::cout << "feature is "<<feature1<<std::endl;
            ptr_vec->push_back(feature1);
        }
        pptr_sent_->push_back(ptr_vec);
    }
}
