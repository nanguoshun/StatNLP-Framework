cmake_minimum_required(VERSION 3.10)
project(statNLP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# compiler optimization, -O3 is the highest level.
set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I /usr/local/include -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O0")

# include the dynet
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dynet)

# set the directory of eign3
#set(EIGEN3_INCLUDE_DIR "/Users/ngs/Documents/cplusproject/eigen" )
set(EIGEN3_INCLUDE_DIR "dynet/eigen" )

IF( NOT EIGEN3_INCLUDE_DIR )
    MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()
include_directories ( "${EIGEN3_INCLUDE_DIR}" )

# look for Boost
set(Boost_REALPATH ON)
find_package(Boost COMPONENTS program_options iostreams serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

#find_package(BLAS REQUIRED)

# add subdirectory and the library
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dynet/dynet)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/neural statnlp_neural_lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/hybridnetworks statnlp_hypernetworks_lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/common/opt statnlp_opt_lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/common/types statnlp_type_lib)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/example/LinearCRF)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/example/LinearNER)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/example/ParsingCRF)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/example/LatentNER)

