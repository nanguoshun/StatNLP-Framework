//
// Created by  ngs on 31/07/2018.
//
#include "network_model.h"

NetworkModel::NetworkModel(FeatureManager *ptr_fm, NetworkCompiler *ptr_nc) {
    this->ptr_fm_ = ptr_fm;
    this->ptr_nc_ = ptr_nc;
    this->num_threads_ = ComParam::Num_Of_Threads;
    this->ptr_inst_all_ = nullptr;
    this->ptr_inst_all_du_ = nullptr;
    this->ptr_inst_all_test_ = nullptr;
    ptr_nn_learner_g_ = nullptr;
    ptr_nn_decoder_g_ = nullptr;
}

NetworkModel::~NetworkModel() {
    std::cout << "Start to release NetworkModel" << std::endl;
    for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
        delete pptr_learner_[threadId];
        delete pptr_decoder_[threadId];
    }
    delete[]pptr_learner_;
    delete[]pptr_decoder_;
    if (nullptr != ptr_learn_thread_vector_) {
        delete ptr_learn_thread_vector_;
    }
    if (nullptr != ptr_decode_thread_vector_) {
        delete ptr_decode_thread_vector_;
    }

    if (nullptr != ptr_inst_all_) {
        for (auto it = ptr_inst_all_->begin(); it != ptr_inst_all_->end(); ++it) { if (nullptr != (*it)) delete (*it); }
        delete ptr_inst_all_;
    }
    if (nullptr != ptr_inst_all_du_) {
        for (auto it = ptr_inst_all_du_->begin(); it != ptr_inst_all_du_->end(); ++it) {
            if (nullptr != (*it))
                delete (*it);
        }
        delete ptr_inst_all_du_;
    }
    /*error for release ptr_inst_vec_test*/
    if (nullptr != ptr_inst_all_test_) {
        for (auto it = ptr_inst_all_test_->begin(); it != ptr_inst_all_test_->end(); ++it) {
            if (nullptr != (*it)) { delete (*it); }
        }
        delete ptr_inst_all_test_;
    }
    delete ptr_decoding_result_;
    for (auto it = ptr_split_inst_test_->begin(); it != ptr_split_inst_test_->end(); ++it) {
        delete (*it);
    }
    delete ptr_split_inst_test_;

    if (nullptr != ptr_nn_learner_g_) {
        delete ptr_nn_learner_g_;
    }
    /*
    if (nullptr != ptr_nn_decoder_g_) {
        delete ptr_nn_decoder_g_;
    }*/

    std::cout << "Releasing NetworkModel Done!" << std::endl;

}

void NetworkModel::Train(std::vector<Instance *> *ptr_all_instances, std::vector<Instance *> *ptr_all_instances_du,
                         int max_num_interations) {
    this->num_threads_ = ComParam::Num_Of_Threads;
    this->ptr_inst_all_ = ptr_all_instances;
    this->ptr_inst_all_du_ = ptr_all_instances_du;
    for (int inst_id = 0; inst_id < ptr_all_instances->size(); ++inst_id) {
        (*ptr_inst_all_)[inst_id]->SetInstanceId(
                inst_id + 1); /* we have multiple instances and each of them will be assigned a unique ID */
    }
    //extract the features.
    pptr_learner_ = new LocalNetworkLearnerThread *[this->num_threads_];
    std::vector<std::vector<Instance *> *> *ptr_inst = this->SplitInstanceForTrain();

    //  std::cout << "the size of ptr_inst is "<<ptr_inst->size()<<std::endl;

    int before_touch_time = GetCurrentMillionSeconds();

    pre_memory_size_ = CommonTool::PrintMemoryUsed("Before Touch", pre_memory_size_);

    for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
        pptr_learner_[threadId] = new LocalNetworkLearnerThread(threadId, this->ptr_fm_, (*ptr_inst)[threadId],
                                                                this->ptr_nc_, -1);
        pptr_learner_[threadId]->Touch();
    }

    pre_memory_size_ = CommonTool::PrintMemoryUsed("After Touch", pre_memory_size_);

    std::cout << "Touch time: " << (double) (GetCurrentMillionSeconds() - before_touch_time) / (double) 1000
              << std::endl;

    //init the parameters of neural network.
    if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        ptr_nn_learner_g_ = this->ptr_fm_->GetGlobalParam()->GetNNParam();
        ptr_nn_learner_g_->SetLearningState();
        ptr_nn_learner_g_->SetLocalNetworkParams(this->ptr_fm_->GetLocalParams());
        ptr_nn_learner_g_->PrepareInputId();
        ptr_nn_learner_g_->SetInstance(ptr_all_instances);
        ptr_nn_learner_g_->AllocateOutSpaceBeforehand();
        if (NetworkConfig::USE_BATCH_TRAINING) {
            //TODO:
        }
    } else if (ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        //TODO:
    }

    this->ptr_fm_->GetGlobalParam()->LockIt();

    pre_memory_size_ = CommonTool::PrintMemoryUsed("After Lock", pre_memory_size_);

    std::cout << "The num of adding Transition is: " << this->ptr_fm_->tmp_count_t_ << std::endl;
    std::cout << "The num of adding emission  is: " << this->ptr_fm_->tmp_count_e_ << std::endl;
    std::cout << "Handcrafted Feature Size is: " << this->ptr_fm_->GetGlobalParam()->tmp_count_ << std::endl;
    double obj_old = ComParam::DOUBLE_NEGATIVE_INFINITY;
    //EM style algorithm
//    long start_time = clock();
    int start_time = GetCurrentMillionSeconds();


    ptr_learn_thread_vector_ = new std::vector<std::thread>;

    std::cout << "Totally, we have " << num_threads_ << " threads" << std::endl;
    for (int i = 0; i <= max_num_interations; ++i) {
//        long time = clock();
        int time = GetCurrentMillionSeconds();
        //calc the neural's output, which will be used as the features of CRF.
        if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
            ptr_nn_learner_g_->Forward();
            ptr_nn_learner_g_->ResetAllNNGradient();
        } else if (ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
            //TODO:
        }
        for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
            //this->pptr_learner_[threadId]
            ptr_learn_thread_vector_->push_back(std::thread(&LocalNetworkLearnerThread::Run, pptr_learner_[threadId]));
        }
        for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
            //this->pptr_learner_[threadId]
            (*ptr_learn_thread_vector_)[threadId].join();
        }
        bool last_iteration = (i == max_num_interations);
        bool done = false;
        //OKya, we have the gradient of all parameters, and then update the weight by optimizer.
        if (last_iteration) {
            done = this->ptr_fm_->Update(true);
        } else {
            done = this->ptr_fm_->Update(false);
        }
        double obj = this->ptr_fm_->GetGlobalParam()->GetOldObj();
        //time = clock() - time;
        time = GetCurrentMillionSeconds() - time;
        /*
        std::cout << "Iteration: " << i << " Obj: " << obj << " Time: " << (double) time / (double)CLOCKS_PER_SEC <<
                  " Convergence: " << obj / obj_old << " Total Time: " << (double) (clock() - start_time) / (double)CLOCKS_PER_SEC << std::endl;
        */
        std::cout << "Iteration: " << i << " Obj: " << std::setprecision(16) << obj << " Time: "
                  << (double) time / (double) 1000 <<
                  " Convergence: " << obj / obj_old << " Total Time: "
                  << (double) (GetCurrentMillionSeconds() - start_time) / (double) 1000 << std::endl;
        obj_old = obj;
        if (last_iteration) {
            std::cout << "Training complete after " << i << " iterations" << std::endl;
            break;
        }
        if (done) {
            std::cout << "Training complete after " << i << " iterations" << std::endl;
            break;
        }
        ptr_learn_thread_vector_->clear();
        //save the model every 30 iteration.
        if (0 == i % 30 && 0 != i) {
            if (true == NetworkConfig::SAVE_MODEL) {
                std::cout << "saving the model to disk....." << std::endl;
                SaveModel();
            }
        }
    }
    if (true == NetworkConfig::SAVE_MODEL) {
        std::cout << "saving the model to disk....." << std::endl;
        SaveModel();
    }
}

std::vector<Instance *> *NetworkModel::Decode(std::vector<Instance *> *ptr_test_instences, bool is_cache_features) {

 //   std::vector<NeuralNetwork*> *ptr_vec = ptr_nn_learner_g_->GetNNVect();
//    std::cout << "neural parameter before decode"<<std::endl;
//    (*ptr_vec)[0]->CopyParams2Dynet(true);
    this->ptr_inst_all_test_ = ptr_test_instences;
    this->num_threads_ = ComParam::Num_Of_Threads;
    this->pptr_decoder_ = new LocalNetworkDecoderThread *[this->num_threads_];
    SplitInstanceForTest();
    for (int threadid = 0; threadid < this->num_threads_; ++threadid) {
        if (is_cache_features || NetworkConfig::FEATURE_TOUCH_TEST) {
            //TODO:
        } else {
            pptr_decoder_[threadid] = new LocalNetworkDecoderThread(threadid, (*ptr_split_inst_test_)[threadid],
                                                                    ptr_fm_, ptr_nc_);
        }
    }
    ptr_decode_thread_vector_ = new std::vector<std::thread>;
    pre_memory_size_ = CommonTool::PrintMemoryUsed("Before Decode", pre_memory_size_);
    if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        LocalNetworkParam **ptr_param_l = new LocalNetworkParam *[num_threads_];

        for (int thread_no = 0; thread_no < num_threads_; ++thread_no) {
            ptr_param_l[thread_no] = new LocalNetworkParam(thread_no, ptr_fm_, ptr_inst_all_test_->size());
            ptr_fm_->SetLocalNetworkParams(thread_no, ptr_param_l[thread_no]);
        }
    }
    for (int threadid = 0; threadid < this->num_threads_; ++threadid) {
        pptr_decoder_[threadid]->Touch();
    }
//        ComType::Input_Str_Vector *ptr_nn_input = (*ptr_nn_vec)[0]->HyperEdgeInput2NNInput(ptr_edge_input);
//        ComType::Neural_Input_Map *ptr_input_map = (*ptr_localNNInput2IdMap_vect_)[netId];
//        std::cout << "add nerual sentence is: ";
//        for(auto it = ptr_nn_input->begin(); it != ptr_nn_input->end(); ++it){
//            std::cout << (*it) << " ";
//        }
//        std::cout << std::endl;
    if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        ptr_nn_decoder_g_ = new GlobalNeuralNetworkParam(ptr_nn_learner_g_->GetNNVect(),
                                                         ptr_nn_learner_g_->GetDynetFunctionHelper(),false);
        ptr_nn_decoder_g_->SetLocalNetworkParams(this->ptr_fm_->GetLocalParams());
        ptr_nn_decoder_g_->ResetNNBeforeDecode();
        ptr_nn_decoder_g_->PrepareInputId();
        ptr_nn_decoder_g_->SetDecodeState();
        ptr_nn_decoder_g_->SetInstance(ptr_test_instences);
        ptr_nn_decoder_g_->AllocateOutSpaceBeforehand();
        if (NetworkConfig::USE_BATCH_TRAINING) {
            //TODO:
        }
        ptr_fm_->GetGlobalParam()->SetNNParameter(ptr_nn_decoder_g_);
//        std::cout << NetworkConfig::STATUS <<std::endl;
//        NetworkConfig::STATUS = ComType::ModelStatus::TESTING;
//        std::cout << NetworkConfig::STATUS <<std::endl;
//        std::cout << "neural parameter after decode"<<std::endl;
//        ptr_vec = ptr_nn_decoder_g_->GetNNVect();
//        (*ptr_vec)[0]->CopyParams2Dynet(true);
        ptr_fm_->GetGlobalParam()->GetNNParam()->Forward();
    } else if (ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type) {
        //TODO:
    }
    for (int threadid = 0; threadid < this->num_threads_; ++threadid) {
        ptr_decode_thread_vector_->push_back(std::thread(&LocalNetworkDecoderThread::Run, pptr_decoder_[threadid]));
    }
    for (int threadid = 0; threadid < this->num_threads_; ++threadid) {
        (*ptr_decode_thread_vector_)[threadid].join();
    }
    pre_memory_size_ = CommonTool::PrintMemoryUsed("After Decode", pre_memory_size_);
    std::cout << "decode done" << std::endl;
    ptr_decoding_result_ = new std::vector<Instance *>;
    for (int threadid = 0; threadid < this->num_threads_; ++threadid) {
        std::vector<Instance *> *ptr_outputs = pptr_decoder_[threadid]->GetOutPuts();
        for (auto it = ptr_outputs->begin(); it != ptr_outputs->end(); ++it) {
            ptr_decoding_result_->push_back((*it));
        }
    }
    return ptr_decoding_result_;
}

/**
 *
 * Allocate training instances to different threads.
 *
 * @return
 */
void NetworkModel::SplitInstanceForTest() {
    int inst_size = ptr_inst_all_test_->size();
    ptr_split_inst_test_ = new std::vector<std::vector<Instance *> *>;
    std::vector<std::vector<int>> thread_inst_vec(this->num_threads_);
    //uniformly allocate instances to different threads by its id.
    int threadId = 0;
    for (int instId = 0; instId < inst_size; ++instId) {
        thread_inst_vec[threadId].push_back(instId);
        threadId = (threadId + 1) % this->num_threads_;
    }
    for (int threadId = 0; threadId < this->num_threads_; ++threadId) {
        int size = thread_inst_vec[threadId].size();
        std::vector<Instance *> *ptr_vec_inst = new std::vector<Instance *>;
        for (int i = 0; i < size; ++i) {
            int inst_no = thread_inst_vec[threadId][i];
            Instance *ptr_inst = (*ptr_inst_all_test_)[inst_no];
            ptr_vec_inst->push_back(ptr_inst);
        }
        ptr_split_inst_test_->push_back(ptr_vec_inst);
    }
}

int NetworkModel::GetCurrentMillionSeconds() {
    milliseconds ms = duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
    );
    return ms.count();
}

void NetworkModel::SetPreMemorySize(long size) {
    pre_memory_size_ = size;
}

void NetworkModel::SaveModel() {
    std::ofstream ofs(NetworkConfig::MODEL_FILE);
    int model_param_size = ptr_fm_->GetGlobalParam()->GetModelParamSize();
    double *ptr_weight = ptr_fm_->GetGlobalParam()->GetModelWeight();
    for (int i = 0; i < model_param_size; ++i) {
        ofs << ptr_weight[i];
        ofs << std::endl;
    }
    ofs.close();
}