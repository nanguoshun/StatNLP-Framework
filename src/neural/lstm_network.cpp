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
    ptr_builder_ = new dynet::LSTMBuilder(param.layers_,param.input_dim_,param.hidden_dim_,*NeuralNetwork::ptr_model_); //1123
    int lstm_param_size = ptr_builder_->local_model.parameter_count();//1123
    param_size_ += lstm_param_size; //1123
    ppptr_output_matrix_ = new std::vector<std::vector<std::vector<dynet::real>>>;
    is_alloc_space_beforehand_ = true;
}

LSTMNetwork::~LSTMNetwork() {
//    delete ptr_model_;
    delete ppptr_output_matrix_;
    delete ptr_builder_;
//    delete ptr_training_;
}

void LSTMNetwork::AddParameters(dynet::ParameterCollection &model, LSTMSuperParam &param) {
    p_c_ = model.add_lookup_parameters(param.vocab_size_,{param.input_dim_}); // 1123

//    p_c_ = model.add_lookup_parameters(param.vocab_size_,{param.label_size_});

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

/**
 * we didn't call this function in currrent implementation.
 *
 * @param argc
 * @param argv
 */
//void LSTMNetwork::Initialize(int &argc, char **&argv) {
//    dynet::DynetParams dyparams = ExtractParam(argc,argv);
//    dyparams.random_seed = 0;
//    NeuralNetwork::AllocateNeuralParamSpace();
//    Params params;
//    get_args(argc,argv,params,Task::TRAIN);
//    ptr_model_ = new dynet::ParameterCollection();
//    NetworkConfig::kSOS = dict_.convert("<s>");
//    NetworkConfig::kEOS = dict_.convert("</s>");
//    ptr_training_ = new std::vector<std::vector<int>>;
//    {
//        std::string train_file_name = params.train_file;
//        std::cerr << "Reading training data from " << train_file_name << " ...\n";
//        int ttoks = ReadData(train_file_name, *ptr_training_);
//        int tlc = ptr_training_->size();
//        std::cerr << tlc << " lines, " << ttoks << " tokens, " << dict_.size() << " types\n";
//    }
//    dict_.freeze();
//    dict_.set_unk("<unk>");
//}

dynet::DynetParams LSTMNetwork::ExtractParam(int &argc, char **&argv) {
    return dynet::extract_dynet_params(argc,argv);
}

void LSTMNetwork::Touch() {
    NeuralNetwork::Touch();
}

/**
 *  1. manually set the output gradient of the LSTM
 *
 */
void LSTMNetwork::BackWard() {
    NeuralNetwork::BackWard();
}

void LSTMNetwork::Update(double count, Network *ptr_network, int parent_k, int children_k_index) {
    NeuralNetwork::Update(count,ptr_network,parent_k,children_k_index);
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
    dynet::Expression r_exp = dynet::parameter(*ptr_cg_,p_R_); //1123
    dynet::Expression bais_exp = dynet::parameter(*ptr_cg_,p_bias_);//1123
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
//        final_vec.push_back(expr);
//
//        continue;

//below is commented 1123f
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
 *  Pos_1  l_1
 *         l_2
 *         ...
 *         l_M
 *  Pos_2  l_1
 *         l_2
 *         ...
 *         l_M
 *  ...
 *
 * Pos_L   ....
 *
 *
 * @param ptr_edgeInput: a pair which consists of a sentence vector and a position (actually a word) in the sentence.
 * @param output_label: the label of the word.
 * @return
 */
 /**
  *
  * @param ptr_edgeInput
  * @return
  */
int LSTMNetwork::HyperEdgeInput2OutputRowIndex(NeuralIO *ptr_io) {
     ComType::Neural_Input *ptr_sent_and_pos = ptr_io->GetInput();
     int output_label = ptr_io->GetOutput();
#ifdef DEBUG_NN_
     ComType::Input_Str_Vector *ptr_input_vec = ptr_sent_and_pos->first;
            std::cout << "get score input is: ";
            for(auto it = ptr_input_vec->begin(); it != ptr_input_vec->end(); ++it){
               std::cout <<(*it)<<" ";
            }
            std::cout <<std::endl;
#endif
    int senId = GetNNInputID(ptr_sent_and_pos->first);
    int pos = ptr_sent_and_pos->second;
    int nn_input_size = GetNNInputSize();
    int pos_index = pos * label_size_ * nn_input_size;
    int rest_index = output_label * nn_input_size + senId;
    int row = pos_index + rest_index;
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
    pptr_sent_->clear();
    for(auto it = ptr_inst->begin(); it != ptr_inst->end(); ++it){
        LSTMNetwork *ptr_base_inst = (LSTMNetwork *)(*it);
//        ComType::Input_Str_Matrix *ptr_input = ptr_base_inst->GetInput();
        Sentence *ptr_input = ptr_base_inst->GetInput();
        pptr_sent_->push_back(ptr_input->GetSentence());
        //below code is for linear CRF.
/*
        int sent_len = ptr_input->size();
        std::vector<std::string> *ptr_vec = new std::vector<std::string>;
        for(auto itt = ptr_input->begin(); itt != ptr_input->end(); ++itt){
            std::string feature1 = (*itt)[0];
            //std::cout << "feature is "<<feature1<<std::endl;
            ptr_vec->push_back(feature1);
        }
        pptr_sent_->push_back(ptr_vec);
*/
        //above code is for linear CRF
    }

}
