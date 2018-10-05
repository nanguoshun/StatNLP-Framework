//
// Created by  ngs on 04/09/2018.
//

#include "cnn_factory.h"

CNNFactory::CNNFactory() {

}

CNNFactory::~CNNFactory() {

}

NeuralNetwork *CNNFactory::CreateNN() {
    return nullptr;
}

void CNNFactory::InitNNParameter(int &argc, char **&argv, int vocab_size, int label_size, unsigned int random_seed,
                                 bool shared_parameters) {
    ptr_call_dynet_->Initialize(argc,argv,random_seed,shared_parameters);
    ptr_call_dynet_->SetParameters(argc,argv);
    CNNTest();
}

std::vector<NeuralNetwork *> *CNNFactory::GetNeuralInst() {
    //todo:
    return nullptr;
}

void CNNFactory::CNNTest() {
    dynet::ParameterCollection *ptr_model = new dynet::ParameterCollection();
    int vac_size = 4;
    unsigned filter_size = 64;/* how many filters */
    unsigned emb_size = 32;/* the size of embedding vector is 32 */
    unsigned win_size = 3;/* 3*32 for a filter  */
//    dynet::LookupParameter w_emb = ptr_model->add_lookup_parameters(vac_size, {emb_size});
    dynet::LookupParameter w_emb = ptr_model->add_lookup_parameters(vac_size, {1,1,emb_size});
    std::cout << "the dim of w_emb is"<<w_emb.dim()<<std::endl;
    /* channel, H = window size( how many words at the same time), W = embedding size, num of F */
    dynet::Parameter w_cnn = ptr_model->add_parameters({1, win_size, emb_size, filter_size});
//    dynet::Parameter w_cnn = ptr_model->add_parameters({win_size, emb_size, 1, filter_size});
    std::cout << "the dim of w_cnn is"<<w_cnn.dim()<<std::endl;
    dynet::Parameter b_cnn = ptr_model->add_parameters({filter_size});
    std::cout << "the dim of b_cnn is"<<b_cnn.dim()<<std::endl;
    dynet::ComputationGraph cg;
    std::vector<dynet::Expression> emb_vec;
    /*
    int sent_max_length = 39;
    int num_of_sent = 1;
    for (int l = 0; l < num_of_sent; ++l) {
        std::vector<unsigned int> word_id_vec;
        for (int i = 0; i < sent_max_length; ++i) {
            word_id_vec.push_back(i);
        }
        dynet::Expression w_emb_expr = dynet::lookup(cg, w_emb, word_id_vec);
        std::cout << "dim of w_emb_expr is " << w_emb_expr.dim() << std::endl;
        emb_vec.push_back(w_emb_expr);
    }
    */
    for(int i = 0; i < vac_size; ++i){
        dynet::Expression w_emb_expr = dynet::lookup(cg, w_emb, i);
        std::cout << "dim of w_emb_expr is " << w_emb_expr.dim() << std::endl;
        emb_vec.push_back(w_emb_expr);
    }
    dynet::Expression cnn_in = dynet::concatenate_cols(emb_vec);
    std::cout << "dim of cnn_in is " << cnn_in.dim() << std::endl;
    dynet::Expression w_cnn_expr = dynet::parameter(cg, w_cnn);
    std::cout << "dim of w_cnn_expr is " << w_cnn_expr.dim() << std::endl;
    dynet::Expression b_cnn_expr = dynet::parameter(cg, b_cnn);
    std::cout << "dim of b_cnn_expr is " << b_cnn_expr.dim() << std::endl;
    std::vector<unsigned> stride_vec;
    for(int i = 0; i<2; ++i){
        stride_vec.push_back(1);
    }
    dynet::Expression ccn_out = dynet::conv2d(cnn_in,w_cnn_expr,b_cnn_expr,stride_vec, false);
    std::cout << "dim of ccn_out is " << ccn_out.dim() << std::endl;
    dynet::Expression pool_out = dynet::max_dim(ccn_out,1);
    std::cout << "dim of pool_out is " << pool_out.dim() << std::endl;
    //dynet::maxpooling2d()

}