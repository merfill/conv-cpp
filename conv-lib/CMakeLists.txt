
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


set(NAME conv-impl)

add_library(${NAME} STATIC
  conv.cu
)

target_compile_features(conv-impl PUBLIC cxx_std_11)
set_target_properties(conv-impl PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_target_properties(conv-impl PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

add_subdirectory(test)

