//
// Created by  ngs on 05/10/2018.
//

#ifndef STATNLP_LINEARNENETWORKCOMPILER_H
#define STATNLP_LINEARNENETWORKCOMPILER_H

#include "src/hybridnetworks/network_compiler.h"
#include "linear_ne_network.h"
#include "linear_ne_instance.h"
#include "entity.h"
#include <boost/algorithm/string.hpp>

class LinearNENetworkCompiler: public NetworkCompiler{
public:
    LinearNENetworkCompiler(bool useIOBE, std::vector<Entity*> *ptr_entity);
    ~LinearNENetworkCompiler();
    Network * Compile(int networkId, Instance* ptr_inst, LocalNetworkParam *ptr_param);
    Instance *Decompile(Network *ptr_network);
    void CompileUnlabeledGeneric();
    void CompileUnlabeledGenericNode(int pos,std::vector<long> &children_vec, std::vector<long>& cur_node_vec);
    void CompileUnlabeledGenericRoot(int pos,std::vector<long>& cur_node_vec);
    long ToNode(int pos, int tag_id);
    long ToNodeRoot(int size);
    long ToNodeLeaf();
    LinearNENetwork* CompileUnlabeled(int networkId, LinearNEInstance *ptr_inst, LocalNetworkParam *ptr_param);
    LinearNENetwork* CompileLabeled(int networkId, LinearNEInstance *ptr_inst, LocalNetworkParam *ptr_param);
private:
    bool is_use_IOBE_;
    LinearNEInstance *ptr_inst_;
    std::unordered_map<Entity*, int> *ptr_entity_id_map_;
    std::vector<Entity*> *ptr_entity_;
    LinearNENetwork *ptr_generic_network_;
    //contains node Id
    long *ptr_all_nodes_;
    //parent index, HyperEdge no(children), Nodes index
    int *** ptr_all_children_;
};

#endif //STATNLP_LINEARNENETWORKCOMPILER_H
