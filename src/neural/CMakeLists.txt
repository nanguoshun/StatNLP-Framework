cmake_minimum_required(VERSION 3.10)
set(SHARED_STATNLP_NEURAL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/dynet)

if(TARGET statnlp_neural_lib)
    message("statnlp_neural_lib is already defined")
else()
    include_directories(${SHARED_STATNLP_NEURAL_INCLUDE_DIR})
    set(LIB_NEUAL_SRCS cnn_factory.cpp cnn_network.cpp dynet_interface.cpp global_neural_network_param.cpp lstm_factory.cpp
            lstm_network.cpp neural_factory.cpp neural_input_output.cpp neural_network.cpp vanilla_rnn.cpp)
    add_library(statnlp_neural_lib STATIC ${LIB_NEUAL_SRCS})
endif()

