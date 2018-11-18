cmake_minimum_required(VERSION 3.5)
add_executable(linear_crf linear_crf_main.cpp linear_crf_feature_manager.cpp linear_crf_instance.cpp linear_crf_lstm.cpp linear_crf_network.cpp
        linear_crf_network_compiler.cpp data_reader.h)
target_link_libraries(linear_crf statnlp_hypernetworks_lib statnlp_neural_lib statnlp_opt_lib statnlp_type_lib dynet)