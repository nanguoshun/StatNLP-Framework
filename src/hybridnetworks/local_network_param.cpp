//
// Created by  ngs on 31/07/2018.
//

#include "local_network_param.h"
#include "src/common/types/common.h"
#include "network.h"

LocalNetworkParam::LocalNetworkParam() {
    std::cout << "default constructor of LocalNetworkParam is called" << std::endl;
}

LocalNetworkParam::LocalNetworkParam(int threadId, FeatureManager *ptr_fm, int numNetworks) {
    this->thread_id_ = threadId;
    this->num_networks_ = numNetworks;
    this->ptr_fm_ = ptr_fm;
    //this->fs_ = NULL;
    this->isFinalized_ = false;
    this->is_global_mode_ = false;
    ptr_globalFeature2LocalFeature_ = nullptr;
    // to be done for multithread
    if (!ComParam::_CACHE_FEATURES_DURING_TRAINING) {
        this->DisableCache();
    }
    if (ComParam::Num_Of_Threads == 1) {
        this->is_global_mode_ = true;
    } else {
        this->ptr_globalFeature2LocalFeature_ = new std::unordered_map<int, int>;
    }
    is_cache_enabled_ = true;
    ptr_cache_ = nullptr;
    if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type &&
        ptr_fm_->GetGlobalParam()->GetNNParam() != nullptr) {
        neural_net_size_ = ptr_fm_->GetGlobalParam()->GetNNParam()->GetNNVect()->size();
        ptr_neural_cache_ = new NeuralIO ****[neural_net_size_];
        for (int i = 0; i < neural_net_size_; ++i) {
            ptr_neural_cache_[i] = nullptr;
        }
        /*allocate the space*/
        ptr_localNNInput2IdMap_vect_ = new ComType::Neural_Input_Map_Vect;
        for(int i = 0; i < neural_net_size_; ++i){
            ComType::Neural_Input_Map* ptr_map = new ComType::Neural_Input_Map;
            ptr_localNNInput2IdMap_vect_->push_back(ptr_map);
        }
        if(NetworkConfig::USE_BATCH_TRAINING && ptr_fm_->GetGlobalParam()->GetNNParam()->IsLearningState()){
            //TODO:
        }
    } else if (ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type &&
               ptr_fm_->GetGlobalParam()->GetNNParam() != nullptr) {
        //TODO:
    }
}

LocalNetworkParam::~LocalNetworkParam() {
    //TODO for delete
    if(nullptr != ptr_globalFeature2LocalFeature_){
        delete ptr_globalFeature2LocalFeature_;
        ptr_globalFeature2LocalFeature_ = nullptr;
    }
    //delete the ptr_localNNInput2IdMap_vect_ step by step.
    if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){
        for(int i = 0; i < neural_net_size_; ++i){
            std::unordered_map<ComType::Input_Str_Vector*, int>* ptr_map = (*ptr_localNNInput2IdMap_vect_)[i];
            for(auto it = ptr_map->begin(); it != ptr_map->end(); ++it){
                delete (*it).first; /* delete the instance of ComType::Input_Str_Vector */
            }
            delete ptr_map; /* delete the instance of std::unordered_map<ComType::Input_Str_Vector*, int>*/
        }
        delete ptr_localNNInput2IdMap_vect_; /* delete the map vector finally*/

        /*delete nerual IO*/
        for(int net = 0; net < neural_net_size_; ++net){
            for(int num_of_network = 0; num_of_network < num_networks_; ++num_of_network){
                int count_nodes = 1;//ptr_network->CountNodes();
                for(int node_no = 0; node_no < count_nodes; ++node_no){
                    //ptr_cache_[net][num_of_network][node_no]
                }
            }
            delete ptr_neural_cache_[net];
        }

    } else if(ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type){

    }
}

void LocalNetworkParam::DisableCache() {
    this->ptr_cache_ = nullptr;
    this->is_cache_enabled_ = false;
}

int LocalNetworkParam::GetThreadId() {
    return thread_id_;
}

double LocalNetworkParam::GetWeight(int featureId) {
    if (this->IsGlobalMode()) {
        return this->ptr_fm_->GetGlobalParam()->GetWeight(featureId);
    } else {
        return this->ptr_fm_->GetGlobalParam()->GetWeight(this->ptr_fs_[featureId]);
    }
}

bool LocalNetworkParam::IsGlobalMode() {
    return is_global_mode_;
}

void LocalNetworkParam::FinalizeIt() {
    if (this->IsGlobalMode()) {
        this->isFinalized_ = true;
        return;
    }
    fs_size_ = this->ptr_globalFeature2LocalFeature_->size();
    this->ptr_fs_ = new int[fs_size_];
    // to be confirmed for this part: global to local feature.
    for (auto it = this->ptr_globalFeature2LocalFeature_->begin(); it != ptr_globalFeature2LocalFeature_->end(); ++it) {
        int f_global = (*it).first;
        int f_local = this->ptr_globalFeature2LocalFeature_->find(f_global)->second;
        ptr_fs_[f_local] = f_global;
    }
    this->isFinalized_ = true;
    this->ptr_counts_ = new double[fs_size_];
    //find the networks with the maximum nodes.
}

bool LocalNetworkParam::isCacheAble() {
    return is_cache_enabled_;
}

/**
 * Extract features from a hyperedge, which is denoted by the parent node parent_k (its index), the hyperedge index
 (as there are multiple hyper edges that is rooted by parent node parent_k), and the children_nodes of the

 * @param ptr_network : the network (graph)
 * @param parent_k : the index of parent node
 * @param ptr_children_k : the pointer of a hyperedge.
 * @param children_k_index : the index of the hyperedge, which is rooted by parent node.
 * @return
 */
FeatureArray *LocalNetworkParam::Extract(Network *ptr_network, int parent_k, int *ptr_children_k,
                                         int children_k_index) {
    if (this->isCacheAble()) {
        if (!this->ptr_cache_) {
            this->ptr_cache_ = new FeatureArray ***[this->num_networks_];
            for (int networkid = 0; networkid < this->num_networks_; ++networkid) {
                ptr_cache_[networkid] = nullptr;
            }
        }
        int networkId = ptr_network->GetNetworkID();
        if (!this->ptr_cache_[networkId]) {
            this->ptr_cache_[networkId] = new FeatureArray **[ptr_network->CountNodes()];
            for (int nodeId = 0; nodeId < ptr_network->CountNodes(); ++nodeId) {
                this->ptr_cache_[networkId][nodeId] = nullptr;
            }
        }
        if (!this->ptr_cache_[networkId][parent_k]) {
            //size indicates the num of hyperedges rooted by node parent_k;
            int size = ptr_network->GetChildrens_Size(parent_k);
            this->ptr_cache_[networkId][parent_k] = new FeatureArray *[size];
            for (int hyperedge_no = 0; hyperedge_no < size; ++hyperedge_no) {
                this->ptr_cache_[networkId][parent_k][hyperedge_no] = nullptr;
            }
        }
        if (this->ptr_cache_[networkId][parent_k][children_k_index]) {
            return ptr_cache_[networkId][parent_k][children_k_index];
        }
    }
    //if the feature Array is not cached in the ptr_cache, then extract it via feature manager
    FeatureArray *ptr_fa = this->ptr_fm_->Extract(ptr_network, parent_k, ptr_children_k, children_k_index);
    //convert the global feature array to local if it works in global mode. i.e., multi-thread.
    if (!is_global_mode_) {
        ptr_fa = ptr_fa->ToLocal(this);
    }
    if (this->isCacheAble()) {
        //store the FeatureArray pointer to the cache.
        this->ptr_cache_[ptr_network->GetNetworkID()][parent_k][children_k_index] = ptr_fa;
    }
    return ptr_fa;
}

FeatureManager *LocalNetworkParam::GetFeatureManager() {
    return ptr_fm_;
}

void LocalNetworkParam::AddObj(double obj) {
    if (this->is_global_mode_) {
        this->ptr_fm_->GetGlobalParam()->AddObj(obj);
        return;
    }
    this->current_obj_ += obj;
}

void LocalNetworkParam::Reset() {
    if (this->is_global_mode_) {
        return;
    }
    this->current_obj_ = 0;
    for (int i = 0; i < fs_size_; ++i) {
        ptr_counts_[i] = 0.0;
    }
}

/**
 *
 * Add the gradient to the feature indexed by f_local.
 *
 * @param f_local
 * @param count
 *
 */

void LocalNetworkParam::AddCount(int f_local, double count) {
    if (f_local == -1) {
        std::cerr << "Error: the feature id is -1. @LocalNetworkParam::AddCount " << std::endl;
        return;
    }
    if (std::isnan(count)) {
        std::cerr << "Error: the count is NAN. @LocalNetworkParam::AddCount" << std::endl;
    }
    if (this->is_global_mode_) {
        this->ptr_fm_->GetGlobalParam()->AddCount(f_local, count);
        return;
    }
    this->ptr_counts_[f_local] += count;
}

void LocalNetworkParam::SetGlobalMode() {
    this->is_global_mode_ = true;
}

int *LocalNetworkParam::GetFeatures() {
    return ptr_fs_;
}

int LocalNetworkParam::GetFeatureSize() {
    return fs_size_;
}

double LocalNetworkParam::GetCount(int f_local) {
    if (this->is_global_mode_) {
        std::cout << "you should not do this in a global mode" << std::endl;
    }
    return this->ptr_counts_[f_local];
}

double LocalNetworkParam::GetObj() {
    return this->current_obj_;
}

int LocalNetworkParam::ToLocalFeature(int f_global) {
    if (is_global_mode_) {
        std::cout
                << "Error: The current mode is global mode, converting a global feature to a local feature is not supported. @LocalNetworkParam::ToLocalFeature"
                << std::endl;
    }
    if (-1 == f_global) {
        return -1;
    }
    if (ptr_globalFeature2LocalFeature_->find(f_global) == ptr_globalFeature2LocalFeature_->end()) {
        int size = ptr_globalFeature2LocalFeature_->size();
        ptr_globalFeature2LocalFeature_->insert(std::make_pair(f_global, size));
    }
    int local_fs_id = this->ptr_globalFeature2LocalFeature_->find(f_global)->second;
    return local_fs_id;
}

/**
 * brief: Add the input and output to a hyper-edge.
 *
 * @param ptr_network: the graphical network that based on each sentence.
 * @param netId: the neural network
 * @param parent_k: the root node of the hyper-edge
 * @param children_k_index: the kth children
 * @param ptr_input:
 * @param output:
 *
 */
void LocalNetworkParam::AddNeuralHyperEdge(int netId, Network *ptr_network,int parent_k, int children_k_index,
                                           ComType::Neural_Input *ptr_edge_input, int output) {

    if (false == BuildNeuralCache(netId, ptr_network, parent_k, children_k_index, ptr_edge_input, output)) {
        return;
    }
    std::vector<NeuralNetwork*> *ptr_nn_vec = ptr_fm_->GetGlobalParam()->GetNNParam()->GetNNVect();
    ComType::Input_Str_Vector *ptr_nn_input = (*ptr_nn_vec)[netId]->HyperEdgeInput2NNInput(ptr_edge_input);
    ComType::Neural_Input_Map *ptr_input_map = (*ptr_localNNInput2IdMap_vect_)[netId];
#ifdef DEBUG_NN
    std::cout << "add nerual sentence is: ";
    for(auto it = ptr_nn_input->begin(); it != ptr_nn_input->end(); ++it){
        std::cout << (*it) << " ";
    }
    std::cout << std::endl;
#endif
    if(ptr_input_map->find(ptr_nn_input) == ptr_input_map->end()){
        int size = ptr_localNNInput2IdMap_vect_->size();
        ptr_input_map->insert(std::make_pair(ptr_nn_input,size));
    } else{
#ifdef DEBUG_NN
    std::cout << "the input has been inserted into the has_map ptr_localNNInput2IdMap_vect_"<<std::endl;
#endif
    }
    if(NetworkConfig::USE_BATCH_TRAINING && ptr_fm_->GetGlobalParam()->GetNNParam()->IsLearningState()){
        int instId = std::abs(ptr_network->GetInstance()->GetInstanceId());
        //todo:
    }
}

/**
 *
 * Allocate the space for neural cache.
 *
 * @param ptr_network
 * @param netId: neural network
 * @param parent_k
 * @param children_k_index
 * @return
 *
 */

bool LocalNetworkParam::BuildNeuralCache(int netId, Network *ptr_network, int parent_k, int children_k_index,
                                         ComType::Neural_Input *ptr_edge_input, int output) {

    if (nullptr == ptr_neural_cache_[netId]) {
        ptr_neural_cache_[netId] = new NeuralIO ***[num_networks_];
        for (int i = 0; i < num_networks_; i++) {
            ptr_neural_cache_[netId][i] = nullptr;
        }
    }
    int networkID = ptr_network->GetNetworkID();
    std::cout << "network id is: "<<networkID<<std::endl;
    if (nullptr == ptr_neural_cache_[netId][networkID]) {
        int count_nodes = ptr_network->CountNodes();
        ptr_neural_cache_[netId][networkID] = new NeuralIO **[count_nodes];
        for (int i = 0; i < count_nodes; ++i) {
            ptr_neural_cache_[netId][networkID][i] = nullptr;
        }
    }
    if (nullptr == ptr_neural_cache_[netId][networkID][parent_k]) {
        int children_size = ptr_network->GetChildrens_Size(parent_k);
        ptr_neural_cache_[netId][networkID][parent_k] = new NeuralIO *[children_size];
        for (int i = 0; i < children_size; ++i) {
            ptr_neural_cache_[netId][networkID][parent_k][i] = nullptr;
        }
    }
    if (nullptr == ptr_neural_cache_[netId][networkID][parent_k][children_k_index]) {
        ptr_neural_cache_[netId][networkID][parent_k][children_k_index] = new NeuralIO(ptr_edge_input,output);
    } else {
        std::cout << "nn input-output pair added for this edge, add again?" << std::endl;
        return false;
    }
    return true;
}

ComType::Neural_Input_Map_Vect* LocalNetworkParam::GetLocalNNInput2Id() {
    return  ptr_localNNInput2IdMap_vect_;
}

NeuralIO* LocalNetworkParam::GetHyperEdgeIO(Network *ptr_network, int netId, int parent_k, int children_k_index) {
    if(nullptr == this->ptr_neural_cache_[netId]){
        return nullptr;
    }
    if(nullptr == this->ptr_neural_cache_[netId][ptr_network->GetNetworkID()]){
        return nullptr;
    }
    if(nullptr == this->ptr_neural_cache_[netId][ptr_network->GetNetworkID()][parent_k]){
        return nullptr;
    }
    return this->ptr_neural_cache_[netId][ptr_network->GetNetworkID()][parent_k][children_k_index];
}
