//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_NETWORKIDMANAGER_H
#define STATNLP_NETWORKIDMANAGER_H

#include <vector>
class NetworkIDManager{
public:
    NetworkIDManager();
    ~NetworkIDManager();
    static long ToHybridNodeID(std::vector<int> &vec);
};
#endif //STATNLP_NETWORKIDMANAGER_H
