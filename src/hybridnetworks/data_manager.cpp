//
// Created by  ngs on 31/07/2018.
//

#include <fstream>
#include "data_manager.h"



template <class InstanceType>
DataManager<InstanceType>::DataManager() {
    ptr_instance = new std::vector<Instance *>;
}

template <class InstanceType>
DataManager<InstanceType>::~DataManager() {

    for(auto it = ptr_instance->begin(); it!=ptr_instance->end(); ++it){
        delete (*it);
    }
    delete ptr_instance;
}

template <class InstanceType>
std::vector<Instance *> * DataManager<InstanceType>::ReadPTBData(const char* file_name, InstanceType type, bool withLabels, bool isLabeled) {
    std::ifstream ifs(file_name);

    return ptr_instance;
}

template <class InstanceType>
std::vector<Instance *> * DataManager<InstanceType>::ReadPosData(const char* file_name, InstanceType type, bool withLabels, bool isLabeled) {
    std::ifstream ifs(file_name);
    std::string str;
    while (std::getline(ifs,str)){

    }
    return ptr_instance;
}


