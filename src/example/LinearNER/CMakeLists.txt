cmake_minimum_required(VERSION 3.5)
add_executable(linear_ner linear_ne_main.cpp entity.cpp entity_reader.h evaluate_ner.h linear_ne_feature_manager.cpp linear_ne_instance.cpp
        linear_ne_network.cpp linear_ne_network_compiler.cpp)
target_link_libraries(linear_ner statnlp_hypernetworks_lib statnlp_neural_lib statnlp_opt_lib statnlp_type_lib dynet)