cmake_minimum_required(VERSION 3.10)
project(statNLP)

set(CMAKE_CXX_STANDARD 11)

add_executable(statNLP main.cpp src/hybridnetworks/hypergraph.h src/hybridnetworks/network_config.h
        src/hybridnetworks/network_config.cpp src/hybridnetworks/global_network_param.h src/hybridnetworks/global_network_param.cpp
        src/hybridnetworks/feature_manager.h src/hybridnetworks/feature_manager.cpp src/hybridnetworks/local_network_param.h
        src/hybridnetworks/local_network_param.cpp src/example/linear_crf_instance.h src/common/types/instance.h
        src/common/types/base_instance.h src/common/types/base_instance.h src/common/types/base_instance.cpp
        src/common/types/instance.cpp src/example/linear_crf_instance.cpp src/hybridnetworks/network_model.h src/hybridnetworks/network_model.cpp src/hybridnetworks/data_manager.h src/hybridnetworks/data_manager.cpp src/example/linear_crf_nework_compiler.h src/example/linear_crf_network_compiler.cpp src/hybridnetworks/network_compiler.h src/hybridnetworks/network_compiler.cpp src/hybridnetworks/network.h src/hybridnetworks/network.cpp src/example/linear_crf_network.h src/example/linear_crf_network.cpp src/hybridnetworks/table_lookup_network.h src/hybridnetworks/table_lookup_network.cpp src/hybridnetworks/common.h src/hybridnetworks/network_id_manager.h src/hybridnetworks/network_id_manager.cpp src/example/linear_crf_feature_manager.h src/example/linear_crf_feature_manager.cpp src/hybridnetworks/feature_array.h src/hybridnetworks/feature_array.cpp src/hybridnetworks/discriminative_network_model.h src/hybridnetworks/discriminative_network_model.cpp src/hybridnetworks/local_network_learner_thread.h src/hybridnetworks/local_network_learner_thread.cpp)