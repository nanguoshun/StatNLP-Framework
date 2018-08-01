//
// Created by  ngs on 01/08/2018.
//

#include "linear_crf_nework_compiler.h"


LinearCRFNetworkCompiler::LinearCRFNetworkCompiler(std::list<std::string> &labels) {
    this->labels_ = labels;
    int i = 0;
    for(std::string label: labels){
        labels_id_map_.insert(std::make_pair(label,i));
        ++i;
    }
    ptr_network_ = new LinearCRFNetwork();
    this->CompileUnlabeledGeneric();
}

LinearCRFNetworkCompiler::~LinearCRFNetworkCompiler() {
    //first release the node resource

    //the delete the network
    delete ptr_network_;
}

LinearCRFNetwork* LinearCRFNetworkCompiler::Compile(int networkId, Instance inst, LocalNetworkParam param) {
    LinearCRFNetwork *ptr;
    return ptr;
}

Instance* LinearCRFNetworkCompiler::Decompile(Network network) {

}

long LinearCRFNetworkCompiler::ToNode(int pos, int tag_id) {

}

long LinearCRFNetworkCompiler::ToNodeRoot(int size) {

}

long LinearCRFNetworkCompiler::ToNodeLeaf() {

}

void LinearCRFNetworkCompiler::CompileUnlabeledGeneric() {

}