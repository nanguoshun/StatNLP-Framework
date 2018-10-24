//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_NETWORK_H
#define STATNLP_NETWORK_H

#include "hypergraph.h"
#include "../common/types/instance.h"
#include "local_network_param.h"
#include "network_id_manager.h"
#include <vector>
/**
 *  inside-outside algorithm.
 */
class Network : public HyperGraph {

public:
    Network();
    Network(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param_l);
    ~Network();
    //virtual GetVector();
    void Touch();
    void Touch(int k);
    void Train();
    inline void Inside() {
        //get the number of node of this network
        int num_node = CountNodes();
        ptr_inside_ = ptr_param_g_->GetInsideSharedArray(thread_id_); //GetInsideSharedArray();
        //get the array size of this thread. this size will be no less than num_node;
        int array_size = ptr_param_g_->GetSharedArraySize(thread_id_);
        //init the inside value of each node.
        for (int nodeId = 0; nodeId < num_node; ++nodeId) {
            ptr_inside_[nodeId] = 0;
        }
        for (int nodeId = 0; nodeId < num_node; ++nodeId) {
            this->Inside(nodeId);
#ifdef DEBUG
            std::cout << "NetworkID: "<<network_id_<< " Inside Index: "<< nodeId<< "  Value: "<<ptr_inside_[nodeId]<<std::endl;
#endif
        }
    }

    inline void Outside() {
        this->ptr_outside_ = ptr_param_g_->GetOutsideSharedArray(thread_id_);// //GetOutsideSharedArray();
        int array_size = ptr_param_g_->GetSharedArraySize(thread_id_);
        //init the outside value of each node.
        int num_node = CountNodes();
        for (int nodeId = 0; nodeId < num_node; ++nodeId) {
            ptr_outside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
        }
        for (int nodeId = num_node - 1; nodeId >= 0; --nodeId) {
            this->Outside(nodeId);
#ifdef DEBUG
            std::cout << "NetworkID: "<<network_id_<< " Outside Index: "<< nodeId<< "  Value: "<<ptr_outside_[nodeId]<<std::endl;
#endif
        }
    }

    inline void UpdateGradient() {
        int num_node = this->CountNodes();
        for (int nodeId = 0; nodeId < num_node; ++nodeId) {
            this->UpdateGradient(nodeId);
        }
    }
    /**
    * Get the score summation for all features of the children_k th hyperedge ( without inside/outside)
    * @param ptr_fa: the pointer of FeatureArray.
     * @param nodeId: the parent ID
    * @param ptr_children_k: the array that contains the children nodes.
    * @param children_k: the sequence of the hyperedge.
    * @return: the score
    */
    inline double GetScore(FeatureArray *ptr_fa, int nodeId, int *ptr_children_k, int children_k){
        /**
         * calc the score summation of all features in this hyperedge. Each score is the production
         * of feature value and its weight.
         */
        double score = 0;
        //for the hand-crafted mode, for example, only use CRF or PFCFG without neural features.
        if(ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type){
            score += ptr_fa->GetScore(this->ptr_param_l_);
        }else if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){ // for the hybrid mode
            score += ptr_fa->GetScore(this->ptr_param_l_); // the hand-crafted score
            score += ptr_param_g_->GetNNParam()->GetNNScore(this,nodeId,children_k); // the nueral score
        } else if(ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type){ // for the pure neural mode
            //TODO:only add neural score
        }
        return score;
    }

    /**
     * Get the score summation for all features of the children_k th hyperedge ( without inside/outside)
     * @param nodeId: the parent ID
     * @param ptr_children_k: the array that contains the children nodes.
     * @param children_k: the sequence of the hyperedge.
     * @return: the score
     */
    inline double GetScore(int nodeId, int *ptr_children_k, int children_k){
        FeatureArray *ptr_fa = this->ptr_param_l_->Extract(this, nodeId, ptr_children_k, children_k);
        /**
         * calc the score summation of all features in this hyperedge. Each score is the production
         * of feature value and its weight.
         */
        double score = 0;
        //for the hand-crafted mode, for example, only use CRF or PFCFG without neural features.
        if(ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type){
            score += ptr_fa->GetScore(this->ptr_param_l_);
        }else if(ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type){ // for the hybrid mode
            score += ptr_fa->GetScore(this->ptr_param_l_); // the hand-crafted score
            score += ptr_param_g_->GetNNParam()->GetNNScore(this,nodeId,children_k); // the nueral score
        } else if(ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type){ // for the pure neural mode
            //TODO:only add neural score
        }
        return score;
    }

    /**
     * Calc the score for the children_k th hyperedge.
     * @param nodeId : parent node ID
     * @param ptr_children_k: the array that contains all children nodes.
     * @param children_k: the no of the hypedge;
     * @return: the score for the kth hyperedge
     */
    inline double CalcScore(int nodeId, int *ptr_children_k, int children_k){
        double score = GetScore(nodeId, ptr_children_k, children_k);
        /**
         * for the first hyperedge, no log sum is required.
         * \alpa'_j(s) = \alpha'_{j-1}(s') +  \psi_j(X,s,s')
         */
        int child_k_size = (this->GetChildren_Size(nodeId))[children_k];
        for (int child_no = 0; child_no < child_k_size; ++child_no) {
            int index = ptr_children_k[child_no];
            score += ptr_inside_[index];
        }
        return score;
    }
    /**
     * Calc the inside value for the nodeId. Noted that for the outside algorithm we calc the the outside value of its children.
     *
     * @param nodeId
     */
    inline void Inside(int nodeId) {
        //check if it is removed?
        if (this->IsRemovded(nodeId)) {
            ptr_inside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
            return;
        }
        //get the number of hyperedges rooted by nodeId
        int children_size = this->GetChildrens_Size(nodeId);
        //get the hyperedges rooted by nodeId.
        int **ptr_childrens_vec_ = this->GetChildren(nodeId);
        //if the number of hyperedges rooted by nodeId is zero, then return;
        double inside = 0.0;
        if (nullptr == ptr_childrens_vec_) {
            ptr_inside_[nodeId] = inside;
            return;
        }
        /**
         *  ****************Linear CRF***********************
         * summation of all inside values in a hyperedge
         * The probability expression of linear inside(or forward) value is:
         *  \alpha_j(s) = \sum_s' \alpha_{j-1}(s') * \psi_j(X,s,s')
         * Where \psi_j(X,s,s') denotes the score value for a hypeedge from state s' to s.
         *
         * The probability expression of linear inside(or forward) value is:
         * while in log space, the inside(or forward) value is:
         *  \alpha'_j(s) = \log \sum_s' \exp (\alpha'_{j-1}(s') + \log \psi_j(X,s,s'))
         * Actually, we can omit the log symbol before the score expression \psi_j(X,s,s').
         *
         * ****************Tree based CRF*********************
         *  A Hypedge with a parent node and multiple children is considered as a single edge both
         *  in inside and outside calculation.
         *
         */
        //for the 0th hyperedge, we don't need logsum here and hence it is calculated independently.
        int children_k = 0;
        //the pointer to all nodes of the 0th hyperedge
        int *ptr_children_k = ptr_childrens_vec_[children_k];
        //the num of nodes in the 0th hyperedge that is rootd by nodeID.
        int child_k_size = (this->GetChildren_Size(nodeId))[children_k];
        //for each node in the 0th hyperedge
        if (IsIgnored(ptr_children_k, child_k_size)) {
            inside = ComParam::DOUBLE_NEGATIVE_INFINITY;
        } else {
            inside = CalcScore(nodeId,ptr_children_k,children_k);
        }
        //For each hyper edge whose index is bigger than 1. we need log_sum here.
        for (int children_k = 1; children_k < children_size; ++children_k) {
            int *ptr_children_k = ptr_childrens_vec_[children_k];
            int child_k_size = (this->GetChildren_Size(nodeId))[children_k];
            //for each node in a hyper edge.
            if (IsIgnored(ptr_children_k, child_k_size)) {
                continue;
            } else{
                double score = CalcScore(nodeId, ptr_children_k, children_k);
                //log_sum value;
                double v1 = inside;
                double v2 = score;
                if (v1 == v2 && v2 == ComParam::DOUBLE_NEGATIVE_INFINITY) {
                    inside = ComParam::DOUBLE_NEGATIVE_INFINITY;
                } else if (v1 == v2 && v2 == ComParam::DOUBLE_NEGATIVE_INFINITY) {
                    inside = ComParam::DOUBLE_NEGATIVE_INFINITY;
                } else if (v1 > v2) {
                    inside = std::log(std::exp(score - inside) + 1) + inside;
                } else {
                    inside = std::log(std::exp(inside - score) + 1) + score;
                }
            }
        }
        ptr_inside_[nodeId] = inside;
        if (ptr_inside_[nodeId] == ComParam::DOUBLE_NEGATIVE_INFINITY) {
            this->Remove(nodeId);
        }
    }

/**
 * Calc the outside value for all children nodes, whose hyperedge is rooted by nodeId.
 *
 * @param nodeId
 */
    inline void Outside(int nodeId) {
        if (this->IsRemovded(nodeId)) {
            ptr_outside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
            return;
        } else {
            //check whether the node is root or not.
            this->ptr_outside_[nodeId] = this->IsRoot(nodeId) ? 0.0 : ptr_outside_[nodeId];
        }

        if (this->ptr_inside_[nodeId] == ComParam::DOUBLE_NEGATIVE_INFINITY) {
            ptr_outside_[nodeId] = ComParam::DOUBLE_NEGATIVE_INFINITY;
        }
        //get the vector of hyperedges rooted by nodeId.
        int **ptr_children_vec = this->GetChildren(nodeId);
        //get the number of hyperedges rooted by nodeID
        int children_size = this->GetChildrens_Size(nodeId);
        //calculate all hyperedges rooted by nodeId.
        for (int children_k = 0; children_k < children_size; ++children_k) {
            int *ptr_children_k = ptr_children_vec[children_k];
            int child_k_size = this->GetChildren_Size(nodeId)[children_k];
            if (IsIgnored(ptr_children_k, child_k_size)) {
                continue;
            }
            //calculate the score summation of the hyeredge.
            double score = GetScore(nodeId, ptr_children_k, children_k);
            score += ptr_outside_[nodeId];
            //sum all inside value of children nodes.
            for (int child_no = 0; child_no < child_k_size; ++child_no) {
                int index = ptr_children_k[child_no];
                score += ptr_inside_[index];
            }
            if (score == ComParam::DOUBLE_NEGATIVE_INFINITY) { continue; }
            //calc the outside values of all children nodes.
            for (int child_no = 0; child_no < child_k_size; ++child_no) {
                int index = ptr_children_k[child_no];
                double v1 = ptr_outside_[index];
                /**
                 * recall that in linear CRF, there is only one child node
                 * and hence the inside value is entirely eliminated.
                 */
                double v2 = score - ptr_inside_[index];
                if (v1 > v2) {
                    //log sum computation.
                    ptr_outside_[index] = v1 + std::log(std::exp(v2 - v1) + 1);
                } else {
                    ptr_outside_[index] = v2 + std::log(std::exp(v1 - v2) + 1);
                }
            }
        }
        if (ptr_outside_[nodeId] == ComParam::DOUBLE_NEGATIVE_INFINITY) {
            this->Remove(nodeId);
        }
    }

    /**
 *
 * Calc the gradient of the node, which is indexed by nodeId.
 *
 * Noted that the gradient for all features
 *
 * @param nodeId
 *
 */
    inline void UpdateGradient(int nodeId) {
        if (this->IsRemovded(nodeId)) {
            return;
        }
        double count = 0;
        int **ptr_children_vec = this->GetChildren(nodeId);
        int children_k_size = this->GetChildrens_Size(nodeId);
        //update each hyper-edge.
        for (int children_k = 0; children_k < children_k_size; ++children_k) {
            int *ptr_children_k = ptr_children_vec[children_k];
            int child_k_size = this->GetChildren_Size(nodeId)[children_k];
            if (IsIgnored(ptr_children_k, child_k_size)) {
                continue;
            }
            FeatureArray *ptr_fa = this->ptr_param_l_->Extract(this, nodeId, ptr_children_k, children_k);
            double score = GetScore(ptr_fa, nodeId, ptr_children_k, children_k);
            score += this->ptr_outside_[nodeId];
            for (int child_no = 0; child_no < child_k_size; ++child_no) {
                int index = ptr_children_k[child_no];
                score += this->ptr_inside_[index];
            }
            double normalization = this->ptr_inside_[this->CountNodes() - 1];
            count = std::exp(score - normalization);
            count *= weight_;
            /**
             * update the gradient of all features in this hyperedge.
             * Noted that the gradient of all features are equivalent in a same hyperedge.
             *
             * Details pls refer to Equation. 61 of Classical Probabilistic Models and Conditional Random Fields. Roman Klinger. 2007
             *
             */
#ifdef DEBUG
            std::cout <<nodeId<<"th node count is: "<<count<<std::endl;
#endif
            if (ComParam::USE_HANDCRAFTED_FEATURES == NetworkConfig::Feature_Type) {
                //The count(gradient) for all features in a hyperedge equals to each other.
                ptr_fa->Update(ptr_param_l_, count);
            } else if (ComParam::USE_HYBRID_NEURAL_FEATURES == NetworkConfig::Feature_Type) { // for the hybrid mode
                ptr_fa->Update(ptr_param_l_, count);
                ptr_param_g_->GetNNParam()->UpdateNNGradientOutput(count, this, nodeId, children_k);
            } else if (ComParam::USE_PURE_NEURAL_FEATURES == NetworkConfig::Feature_Type) { // for the pure neural mode
                //TODO:only add neural score
            }
        }
    }
    double *GetInsideSharedArray();
    double *GetOutsideSharedArray();
    //Network * GetNetwork(int networkId);
    int GetNetworkID();
    Instance *GetInstance();
    double GetInside(int nodeId);
    std::vector<int> GetNodeArray(int nodeIndex);
    int tmp_count_;
    bool IsIgnored(int *ptr_children, int size);
    void Max();
    void Max(int nodeId);
    bool IsSumNode(int nodeid);
    int *GetMaxPath(int nodeid);
    int GetThreadId();
    bool GetNeuralInserted();
    void SetNeuralInserted(bool flag);
    int GetMaxChildrenSize(int nodeid);
//    static double **ptr_inside_shared_array_;
//    static double **ptr_outside_shared_array_;
    //the array size for each thread;
protected:
    int network_id_;
    int thread_id_;
    Instance *ptr_inst_;
    //the weight of an instance, usually we set it as 1 for instance un-biased training.
    double weight_;
    //local parameter
    LocalNetworkParam *ptr_param_l_;
    //global parameter
    GlobalNetworkParam *ptr_param_g_;
    double *ptr_inside_;
    double *ptr_outside_;
    /*the max sore, which is indexed by the node id of this network*/
    double *ptr_max_;
    /*node_id,*/
    int **ptr_max_children_no_;
    /*it stores the num of nodes for selected hyperedge that has the maximum score.
     * this array is indexed by the node id of the network.*/
    int *ptr_max_children_size_;
    std::mutex mtx;
    bool neural_feature_inserted_;
    int node_size_;
//    int inside_shared_array_size_;
//    int outside_shared_array_size_;
};

#endif //STATNLP_NETWORK_H
