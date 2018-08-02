//
// Created by  ngs on 01/08/2018.
//

#ifndef STATNLP_LINEAR_CRF_NETWORK_H
#define STATNLP_LINEAR_CRF_NETWORK_H

#include "../hybridnetworks/table_lookup_network.h"

class LinearCRFNetwork: public TableLookupNetwork{
public:
    LinearCRFNetwork();
    ~LinearCRFNetwork();

    int CountNodes() override;

    long GetNode(int k) override;

    std::vector<int> *GetNodeArray(int k) override;

    std::vector<std::vector<int>> *GetChildren(int k) override;

    bool IsRemovded(int k) override;

    void Remove(int k) override;

    bool IsRoot(int k) override;

    bool IsLeaf(int k) override;

    bool IsContain(long node) override;

private:
};
#endif //STATNLP_LINEAR_CRF_NETWORK_H
