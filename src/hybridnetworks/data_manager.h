//
// Created by  ngs on 31/07/2018.
//

#ifndef STATNLP_FILE_MANAGER_H
#define STATNLP_FILE_MANAGER_H

#include <vector>
#include "../common/types/instance.h"

template <class InstanceType>

class DataManager{
public:
    static int value;
    std::vector<Instance *> * ReadPTBData(const char* file_name, InstanceType type, bool withLabels, bool isLabeled);
    std::vector<Instance *> * ReadPosData(const char* file_name, InstanceType type, bool withLabels, bool isLabeled);
    DataManager();
    ~DataManager();

private:
    std::vector<Instance *> *ptr_instance;
};

#endif //STATNLP_FILE_MANAGER_H
