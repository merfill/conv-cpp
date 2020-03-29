
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(conv-cpp CXX CUDA)

set(CONV_CPP_ROOT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CONV_CPP_ROOT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# The path to the directory where cmake modules are.
set(CMAKE_MODULE_PATH ${CONV_CPP_ROOT_SOURCE_DIR}/cmake)

# Connection assembly configuration.
include(Build)

# External libraries search.
include(FindExternalLibs)

# Define unit test data directory path.
set(CONV_CPP_UNIT_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unit_test_data")
add_definitions(-DCONV_CPP_UNIT_TEST_DIR="${CONV_CPP_UNIT_TEST_DIR}")

# Putting it in the directories from the list below:
add_subdirectory(conv-lib)
add_subdirectory(python)
