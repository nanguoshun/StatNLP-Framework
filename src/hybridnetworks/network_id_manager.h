//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_NETWORKIDMANAGER_H
#define STATNLP_NETWORKIDMANAGER_H

#include "src/common/types/common.h"


class NetworkIDManager{
public:
    NetworkIDManager();
    ~NetworkIDManager();
    static void SetCapacity(std::vector<int> cap);
    static long ToHybridNodeID(std::vector<int> &vec);
    static std::vector<int> ToHybridNodeArray(long NodeId);
    static int BinarySearch(long *ptr_array, int array_size, long value);
    static std::vector<int> *ptr_capacity_vec_;
};
#endif //STATNLP_NETWORKIDMANAGER_H
