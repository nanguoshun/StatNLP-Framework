//
// Created by  ngs on 10/08/2018.
//

#ifndef STATNLP_FREELISTBASE_H
#define STATNLP_FREELISTBASE_H

#include "builder.h"

/**
 *
 * This class is in charge of memory management for the program.
 *
 */
class FreeListBase{

public:
    inline FreeListBase(){

    }
    inline ~FreeListBase(){

    }
    inline static void FreeStatic(){
        delete Builder::ptr_inst_vec_;
        delete Builder::ptr_network_vec_;
        delete Builder::ptr_str_vec_;
    }
};

#endif //STATNLP_FREELISTBASE_H
