//
// Created by  ngs on 25/11/2018.
//

#include <src/hybridnetworks/network_compiler.h>

#ifndef STATNLP_TEST_COMPILER_H
#define STATNLP_TEST_COMPILER_H

#endif //STATNLP_TEST_COMPILER_H

class TestCompiler:public NetworkCompiler{
public:
    inline Network *Compile(int networkId, Instance *ptr_inst, LocalNetworkParam *ptr_param) override {
        if(ptr_inst->IS_Labeled()){
            CompileLabeledNetwork();
        } else{
            CompileUnlabeledNetwork();
        }
    }

    inline Instance *Decompile(Network *ptr_network) override {

    }

    inline void CompileLabeledNetwork(){

    }


    inline void CompileUnlabeledNetwork(){

    }

    inline long ToNode(int pos, int tag_id) {
        int arr[] = {pos, tag_id, ComType::NODE_TYPES::NODE};
        std::vector<int> vec(std::begin(arr), std::end(arr));
        return NetworkIDManager::ToHybridNodeID(vec);
    }

    inline long ToNodeLeaf() {
        int arr[] = {0, 0, ComType::NODE_TYPES::LEAF};
        std::vector<int> vec(std::begin(arr), std::end(arr));
        return NetworkIDManager::ToHybridNodeID(vec);
    }

    long ToNodeRoot(int size) {
        int arr[] = {size-1, 2, ComType::NODE_TYPES::ROOT};
        std::vector<int> vec(std::begin(arr), std::end(arr));
        return NetworkIDManager::ToHybridNodeID(vec);
    }

};