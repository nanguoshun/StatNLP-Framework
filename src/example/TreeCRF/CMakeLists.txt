cmake_minimum_required(VERSION 3.10)
add_executable(parsingcrf tree_crf_main.cpp binarytree.cpp evaluate.h node.cpp ptb_reader.h rule.h tree_crf_feature_manager.cpp tree_crf_instance.h
        tree_crf_network.h tree_crf_network_compiler.cpp)
target_link_libraries(parsingcrf dynet statnlp_hypernetworks_lib statnlp_neural_lib statnlp_opt_lib statnlp_type_lib)