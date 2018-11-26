//
// Created by  ngs on 02/08/2018.
//

#include "local_network_learner_thread.h"

LocalNetworkLearnerThread::LocalNetworkLearnerThread(int threadId, FeatureManager *ptr_fm, std::vector<Instance *> *ptr_ins_vector,
                                                     NetworkCompiler *ptr_nc, int it) {
    this->thread_id_ = threadId;
    this->ptr_param_l_ = new LocalNetworkParam(threadId,ptr_fm, ptr_ins_vector->size());
    ptr_fm->SetLocalNetworkParams(threadId,ptr_param_l_);
    this->ptr_inst_vec_ = ptr_ins_vector;
    this->it_no_ = it;
    this->network_capcity_ = ComParam::NETWORK_CAPACITY;
    this->cache_networks_ = true;
    int size = ptr_ins_vector->size();
    if(this->cache_networks_){
        this->ptr_network_ = new Network*[size];
        for(int i=0; i<size; ++i){
            this->ptr_network_[i] = nullptr;
        }
    }
    this->ptr_nc_ = ptr_nc;
}

LocalNetworkLearnerThread::~LocalNetworkLearnerThread() {
    ReleaseLocalParamCache();
    ReleaseLocalParamNeuralCache();
    delete ptr_param_l_;
    if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){

    }else if(ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type){

    }
    for(int i=0; i<ptr_inst_vec_->size(); ++i){
        delete ptr_network_[i];
    }
    delete []ptr_network_;
}

void LocalNetworkLearnerThread::Touch() {
//    int tmp_cout[4] = {0,0,0,0};
    int max_size_node_count=0;
    for(int networkId=0; networkId < this->ptr_inst_vec_->size(); ++networkId){
        this->GetNetwork(networkId)->Touch();
//        tmp_cout[networkId] = this->GetNetwork(networkId)->tmp_count_;
//        tmp_cout[networkId] = ptr_param_l_->GetFeatureManager()->temp_count_;
  //      tmp_cout[networkId] = ptr_param_l_->GetFeatureManager()->GetGlobalParam()->tmp_count_;
  //      tmp_coun_value += tmp_cout[networkId];
        int num_node = this->GetNetwork(networkId)->CountNodes();
        if(num_node > max_size_node_count){
            max_size_node_count = num_node;
        }
    }
    //allocate the share array to store inside and outside value in each thread.
    this->ptr_param_l_->GetFeatureManager()->GetGlobalParam()->AllocateSharedArray(this->thread_id_,max_size_node_count);
    this->ptr_param_l_->FinalizeIt();
}

Network* LocalNetworkLearnerThread::GetNetwork(int networkId) {
    if(this->cache_networks_ && nullptr != this->ptr_network_[networkId]){
        return this-> ptr_network_[networkId];
    }
    Network *ptr_network = this->ptr_nc_->Compile(networkId,(*(this->ptr_inst_vec_))[networkId],this->ptr_param_l_);
    if(this->cache_networks_){
        this->ptr_network_[networkId] = ptr_network;
    }
    //NEED further observation for below code about capacity.
    if(ptr_network->CountNodes() > ComParam::NETWORK_CAPACITY){
        this->network_capcity_ = ptr_network->CountNodes();
    }
    return ptr_network;
}

void LocalNetworkLearnerThread::Run() {
    int size = this->ptr_inst_vec_->size();
    for(int networkId = 0; networkId < size ; ++networkId){
        Network *ptr_network = this->GetNetwork(networkId);
        ptr_network->Train();
    }
}

/**
 *
 * Release the feature array cache allocated in LocalNetworkParam.
 *
 */
void LocalNetworkLearnerThread::ReleaseLocalParamCache() {
    /* delete ptr_cache */
    std::cout << "ThreadID: "<<std::this_thread::get_id()<< " is starting to release the localNetworkParam cache"<<std::endl;
    FeatureArray ****ptr_cache = ptr_param_l_->GetCache();
    int num_of_network = ptr_inst_vec_->size();
    for(int networkID =0; networkID < num_of_network; ++networkID){
        Network *ptr_network = ptr_network_[networkID];
        int num_of_node = ptr_network->CountNodes();
        for(int node_no = 0; node_no < num_of_node; ++node_no){
            long node_id = ptr_network->GetNode(node_no);
            if(0 != node_no){ /* the first node is the Leaf node and it is a static memory, and no need release here*/
                std::vector<int> vec = NetworkIDManager::ToHybridNodeArray(node_id);
                int type = vec[vec.size()-1];
                if(type == ComType::NODE_TYPES::ROOT){ continue;} /*if the */
                //std::cout << "word, pos, tag, type"<<vec[0]<<" "<<vec[1]<<" "<<vec[2]<<" "<<vec[3]<<std::endl;
                int num_of_hyperedge = ptr_network->GetChildrens_Size(node_no);
                for(int hyperedgeNo = 0; hyperedgeNo < num_of_hyperedge; ++hyperedgeNo){
                    FeatureArray *ptr_fa = ptr_cache[networkID][node_no][hyperedgeNo];
                    if(nullptr != ptr_fa){
                        delete ptr_fa;
                    }
                }
                delete []ptr_cache[networkID][node_no];
            }
        }
        delete []ptr_cache[networkID];
    }
    delete []ptr_cache;
    std::cout << "Done!! ThreadID: "<<std::this_thread::get_id()<<" released the localNetworkParam cache"<<std::endl;
}
/**
 * Release the neural cache allocated in LocalNetworkParam
 */
void LocalNetworkLearnerThread::ReleaseLocalParamNeuralCache() {
    /*delete ptr_cache and ptr_nerualIO in localParam*/
    /*delete ptr_nerualIO*/
    if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){
        std::cout << "Start to release the ptr_nerualIO cache"<<std::endl;
        /*delete nerual IO*/
        NeuralIO ***** ptr_neuralIO = ptr_param_l_->GetNeuralIO();
        int num_of_network = ptr_inst_vec_->size();
        int neural_net_size = ptr_param_l_->GetNerualNetSize();
        for(int net = 0; net < neural_net_size; ++net){
            for(int networkID = 0; networkID < num_of_network; ++networkID){
                Network *ptr_network = ptr_network_[networkID];
                int num_of_node = ptr_network->CountNodes();
                for(int node_no = 0; node_no < num_of_node; ++node_no){
                    /*please check the leaf node, i.e, node_no ==0*/
                    long node_id = ptr_network->GetNode(node_no);
                    if(0 != node_no){
                        std::vector<int> vec = NetworkIDManager::ToHybridNodeArray(node_id);
                        int type = vec[vec.size()-1];
                        if(type == ComType::NODE_TYPES::ROOT){ continue;} /**/
                        //std::cout << "pos, tag, type "<<vec[0]<<" "<<vec[1]<<" "<<vec[2]<<" "<<std::endl;
                        int num_of_hyperedge = ptr_network->GetChildrens_Size(node_no);
                        for(int hyperedgeNo = 0; hyperedgeNo < num_of_hyperedge; ++hyperedgeNo){
                            NeuralIO *ptr_io = ptr_neuralIO[net][networkID][node_no][hyperedgeNo];
                            if(nullptr != ptr_io){
                                delete ptr_io;
                            }
                        }
                    }
                    delete []ptr_neuralIO[net][networkID][node_no];
                }
                delete []ptr_neuralIO[net][networkID];
            }
            delete []ptr_neuralIO[net];
        }
        delete []ptr_neuralIO;
        std::cout << "Releasing the ptr_nerualIO cache Done!"<<std::endl;
    } else if(ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type){
        //to be done;
    }
}