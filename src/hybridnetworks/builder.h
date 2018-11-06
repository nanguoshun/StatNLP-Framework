//
// Created by  ngs on 06/11/2018.
//

#ifndef STATNLP_BUILDER_H
#define STATNLP_BUILDER_H

#include "network.h"
#include "table_lookup_network.h"

class Builder{
public:
    inline Builder(){
        /*all of the memory will be released in main class*/
        ptr_network_vec_ = new std::vector<TableLookupNetwork *>;
        ptr_str_vec_ = new std::vector<std::vector<std::string> *>;

    }
    inline ~Builder(){
        /*
        for(auto it = ptr_network_vec_->begin(); it != ptr_network_vec_->end(); ++it){
            delete (*it);
        }
        delete ptr_network_vec_;
        for(auto it = ptr_str_vec_->begin(); it != ptr_str_vec_->end(); ++it){
            delete (*it);
        }
        delete ptr_str_vec_;
        for(auto it = ptr_inst_vec_->begin(); it != ptr_inst_vec_->end(); ++it){
            delete (*it);
        }
        delete ptr_inst_vec_;
         */

    }

    inline Network *CreateNetwork(){
        TableLookupNetwork *ptr_network = new TableLookupNetwork();
        ptr_network_vec_->push_back(ptr_network);
        return ptr_network;
    }

    inline Network *CreateNetwork(int networkid, Instance *ptr_inst, LocalNetworkParam *ptr_param){
        TableLookupNetwork *ptr_network = new TableLookupNetwork(networkid, ptr_inst, ptr_param);
        ptr_network_vec_->push_back(ptr_network);
        return ptr_network;
    }

    inline Network *CreateNetwork(int networkId, Instance *ptr_inst, TableLookupNetwork *ptr_table_network, LocalNetworkParam *ptr_param, int node_size){
        TableLookupNetwork *ptr_network = new TableLookupNetwork(networkId, ptr_inst, ptr_table_network, ptr_param, node_size);
        ptr_network_vec_->push_back(ptr_network);
        return ptr_network;
    }

    inline std::vector<std::string> * CreateStringVector(){
        std::vector<std::string> *ptr_str =  new std::vector<std::string>;
        ptr_str_vec_->push_back(ptr_str);
        return ptr_str;
    }

    inline Instance *CreateInstance(int id, int weight){
        Instance * ptr_inst = new Instance(id, weight);
        ptr_inst_vec_->push_back(ptr_inst);
        return ptr_inst;
    }

    /* currently this function is called by feature manager, and the memory will be released in featuremanager*/
    inline int *CreateIntArray(int size){
        return new int[size];
    }

    inline FeatureArray* CreateFeatureArray(int *ptr_fs, int fs_size){
        if(nullptr == ptr_fs){
            return new FeatureArray((int*) nullptr,0);
        } else{
            int *ptr_array = new int[fs_size];
            for(int i = 0; i < fs_size; ++i){
                ptr_array[i] = ptr_fs[i];
            }
            return new FeatureArray(ptr_array,fs_size);
        }
    }

    inline FeatureArray* CreateFeatureArray(int *ptr_fs, int fs_size, FeatureArray *ptr_next){
        int *ptr_array = new int[fs_size];
        for(int i = 0; i < fs_size; ++i){
            ptr_array[i] = ptr_fs[i];
        }
        return new FeatureArray(ptr_array,fs_size,ptr_next);
    }


private:
    std::vector<TableLookupNetwork *> *ptr_network_vec_;
    std::vector<std::vector<std::string> *> *ptr_str_vec_;
    std::vector<Instance *> *ptr_inst_vec_;

};

#endif //STATNLP_BUILDER_H
