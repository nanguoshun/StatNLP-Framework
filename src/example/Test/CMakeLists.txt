cmake_minimum_required(VERSION 3.5)
add_executable(test test.cpp test_instance.h test_intance.cpp test_compiler.h test_featuremgr.h test_network.h test_network.cpp)
target_link_libraries(test statnlp_hypernetworks_lib statnlp_neural_lib statnlp_opt_lib statnlp_type_lib dynet)