//
// Created by  ngs on 18/10/2018.
//

#ifndef STATNLP_TREE_CRF_NETWORK_H
#define STATNLP_TREE_CRF_NETWORK_H

#include "src/hybridnetworks/table_lookup_network.h"

class TreeCRFNetwork: public TableLookupNetwork{
public:
    TreeCRFNetwork();
    ~TreeCRFNetwork();
};

#endif //STATNLP_TREE_CRF_NETWORK_H

