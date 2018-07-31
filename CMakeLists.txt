cmake_minimum_required(VERSION 3.10)
project(statNLP)

set(CMAKE_CXX_STANDARD 11)

add_executable(statNLP main.cpp src/hybridnetworks/hypergraph.h src/hybridnetworks/network_config.h
        src/hybridnetworks/network_config.cpp src/hybridnetworks/global_network_param.h src/hybridnetworks/global_network_param.cpp
        src/hybridnetworks/feature_manager.h src/hybridnetworks/feature_manager.cpp src/hybridnetworks/local_network_param.h
        src/hybridnetworks/local_network_param.cpp src/example/linear_crf_instance.h src/common/types/instance.h
        src/common/types/base_instance.h src/common/types/base_instance.h src/common/types/base_instance.cpp
        src/common/types/instance.cpp src/example/linear_crf_instance.cpp)