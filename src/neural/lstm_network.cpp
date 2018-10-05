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

LSTMNetwork::LSTMNetwork(LSTMSuperParam &param) {
    AddParameters(*ptr_model_,param);
    ptr_builder_ = new dynet::LSTMBuilder(param.layers_,param.input_dim_,param.hidden_dim_,*NeuralNetwork::ptr_model_);
//  ptr_builder_ = new dynet::LSTMBuilder(1,2,32,*ptr_model_);
    //ptr_builder_
    int lstm_param_size = ptr_builder_->local_model.parameter_count();
    param_size_ += lstm_param_size;
    ppptr_output_matrix_ = new std::vector<std::vector<std::vector<dynet::real>>>;
    is_alloc_space_beforehand_ = true;
}

LSTMNetwork::~LSTMNetwork() {
    delete ptr_model_;
    delete ppptr_output_matrix_;
    delete ptr_builder_;
}

void LSTMNetwork::AddParameters(dynet::ParameterCollection &model, LSTMSuperParam &param) {
    p_c_ = model.add_lookup_parameters(param.vocab_size_,{param.input_dim_});
    p_R_ = model.add_parameters({param.label_size_,param.hidden_dim_});
    p_bias_ = model.add_parameters({param.label_size_});
/*
    p_c_ = model.add_lookup_parameters(100,{2});
    p_R_ = model.add_parameters({100,32});
    p_bias_ = model.add_parameters({100});
    */
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
    NeuralNetwork::Update(count,ptr_network,parent_k,children_k_index);
}

dynet::Expression LSTMNetwork::GetOutput(const std::vector<int> &sentence, int sent_idx, dynet::ComputationGraph &cg,
                                         bool apply_dropout) {
    /*
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
    }*/
}

dynet::Expression LSTMNetwork::BuildLSTMGraph(const std::vector<int> &sentence, dynet::ComputationGraph &cg,

                                         bool apply_dropout) {
    /*      const unsigned slen = sentence.size();
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
     return dynet::sum(errs);*/
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
/***
 *
 * Build LSTM computation graph based the targeting dataset.
 *
 * @param pptr_sent: all sentences of the training dataset.
 * @return
 */

dynet::Expression LSTMNetwork::BuildLMGraph2(const vector<float>& sent, dynet::ComputationGraph* ptr_cg, bool apply){
    ptr_builder_->new_graph(*ptr_cg);
    ptr_builder_->start_new_sequence();
    dynet::Expression R = parameter(*ptr_cg,p_R_);
    std::cout << "the dim of R is "<<R.dim()<<std::endl;
    dynet::Expression bias = parameter(*ptr_cg,p_bias_);
    std::cout << "the dim of bias is "<<bias.dim()<<std::endl;
    vector<dynet::Expression> err_vec;
    int size =  sent.size();
    for(int i = 0; i< sent.size(); ++i){
        dynet::Expression x_t = lookup(*ptr_cg,p_c_,sent[i]);
        dynet::Expression h_t = ptr_builder_->add_input(x_t);
        std::cout << "the dim of h_t is "<<h_t.dim()<<std::endl;
        err_vec.push_back(h_t);
    }
    dynet::Expression final_exp = concatenate(err_vec);
    std::cout << "the dim of final_exp is "<<final_exp.dim()<<std::endl;
    return  final_exp;
}

dynet::Expression LSTMNetwork::BuildForwardGraph(std::vector<std::vector<std::string>*> *pptr_sent) {
    /*std::vector<float> temp;
    std::string good[3] = {"I", "am", "Good"};
    for(int i = 0; i< 3; i++){
//        std::string word = good[i];
        //float a = dict_.convert(word);
        temp.push_back(i);
    }

    for(int i = 0; i< 3; ++i){
        if(dict_.contains(good[i])){
            //p_c_.initialize(dict_.convert(good[i]),temp);
        } else{
            std::cout<<"dic doesn't contain the word"<<std::endl;
        }
    }
    return BuildLMGraph2(temp,ptr_cg_,false);
    */
    dynet::Expression r_exp = dynet::parameter(*ptr_cg_,p_R_);
    dynet::Expression bais_exp = dynet::parameter(*ptr_cg_,p_bias_);
    //std::cout << "dim of r_exp is:"<<r_exp.dim() <<std::endl;
    //std::cout << "dim of bais_exp is"<<bais_exp.dim() <<std::endl;
    ptr_builder_->new_graph(*ptr_cg_); // reset RNN builder for the new graph
    ptr_builder_->start_new_sequence();
    std::vector<dynet::Expression> final_vec;
    int num_of_sentence = pptr_sent->size();
    int word_id = 0;
    for(int pos = 0; pos < max_len_; ++pos){
        std::vector<unsigned int> word_id_vec;
        for(int i = 0; i < num_of_sentence; ++i){
            int sent_size = (*pptr_sent)[i]->size();
            if(pos > sent_size-1){
                word_id = 0;
                word_id_vec.push_back(word_id);
            } else{
                std::string word = (*((*pptr_sent)[i]))[pos];
                //dict_.convert(word);
                auto it = ptr_word2int_map_->find(word);
                if(it != ptr_word2int_map_->end()){
                    word_id = (*it).second;
                }else{
                    word_id = ptr_word2int_map_->find(ComParam::UNK)->second;
                }
                word_id_vec.push_back(word_id);
            }
        }
        //std::cout << "dim of lookup p_c is "<<p_c_.dim()<<std::endl;
        /*get the embedding of the word_id_vec at a position*/
        dynet::Expression expr = dynet::lookup(*ptr_cg_,p_c_,word_id_vec);
        //std::cout << "dim of lookup expression for position "<<pos<<" th is "<<expr.dim()<<std::endl;
        dynet::Expression predict_exp = ptr_builder_->add_input(expr);
        //std::cout << "dim of predict expression is "<<predict_exp.dim()<<std::endl;
        dynet::Expression output_exp = r_exp * predict_exp + bais_exp;
        //std::cout<< "dim of output expression is "<<output_exp.dim()<<"iteration count is"<<iteration_count_<<std::endl;
        final_vec.push_back(output_exp);
    }
    //dynet::Expression result = dynet::transpose(dynet::concatenate_cols(final_vec));
    dynet::Expression result = dynet::concatenate(final_vec);
    //dynet::transpose(result);
    //std::cout << "dim of result is "<<result.dim()<<std::endl;
    return result;
}

/**
 * Return the index of an edge input, which consists of a sentence and the position. Here
 * position indicates the word position when the feature was extracted.
 *
 *         S_1, S_2, S_3..., S_N
 *
 *  Pos_1  idx
 *  Pos_2
 *  ...
 *
 *
 *
 * @param ptr_edgeInput: a pair which consists of a sentence vector and a position (actually a word) in the sentence.
 * @return
 */
int LSTMNetwork::HyperEdgeInput2OutputRowIndex(void *ptr_edgeInput,int output_label) {
    ComType::Neural_Input *ptr_sent_and_pos = (ComType::Neural_Input *)ptr_edgeInput;
    int senId = GetNNInputID(ptr_sent_and_pos->first);
    int pos = ptr_sent_and_pos->second;
    int nn_input_size = GetNNInputSize();
    int pos_index = pos * label_size_ * nn_input_size;
    int rest_index = output_label * nn_input_size + senId;
    int row = pos_index + rest_index;
    //int row = pos * nn_input_size + senId;
    return row;
}

/***
 *
 * Return the sentence vector in the edge input.
 *
 * @param ptr_edgeInput: a pair which consists of a sentence vector and a position (actually a word) in the sentence.
 * @return
 */
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
 * Extract the sentence matrix from ptr_inst, which contains multiple sentence vectors.
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
            //std::cout << "feature is "<<feature1<<std::endl;
            ptr_vec->push_back(feature1);
        }
        pptr_sent_->push_back(ptr_vec);
    }
}
