cmake_minimum_required(VERSION 3.10)
set(SHARED_STATNLP_HYPERNETWORKS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(TARGET statnlp_hypernetworks_lib)
    message("statnlp_hypernetworks_lib is already defined")
else()
    include_directories(${SHARED_STATNLP_HYPERNETWORKS_INCLUDE_DIR})
    set(LIB_HYPEREDGE_SRCS data_manager.cpp discriminative_network_model.cpp feature_array.cpp feature_manager.cpp freelistbase.h
            global_network_param.cpp hypergraph.h local_network_decoder_thread.cpp local_network_learner_thread.cpp local_network_param.cpp
            network.cpp network_compiler.cpp network_id_manager.cpp network_model.cpp table_lookup_network.cpp builder.h)
    add_library(statnlp_hypernetworks_lib STATIC ${LIB_HYPEREDGE_SRCS})
endif()

