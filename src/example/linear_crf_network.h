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
};
#endif //STATNLP_LINEAR_CRF_NETWORK_H
