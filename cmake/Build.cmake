
# Do not allow in source build.
if ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")
  message(FATAL_ERROR "In-source builds are not allowed.")
endif ("${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}")

# Build type check.
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING
    "Set default build type: Release."
    FORCE)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CONV_CPP_ROOT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CONV_CPP_ROOT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CONV_CPP_ROOT_BINARY_DIR}/lib)

# Mandatory flags for GCC.
if (${CMAKE_COMPILER_IS_GNUCXX})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC -Wall -Wno-error -Wno-undef -Wl,--no-allow-shlib-undefined -D__STDC_LIMIT_MACROS=1")
endif (${CMAKE_COMPILER_IS_GNUCXX})

# Definitions for build types.
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  # Flags fog GCC.
  if (${CMAKE_COMPILER_IS_GNUCXX})
    # Add debug symbols to binary files.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb3")

    # Profiling: run cmake -DGPROF=1.
    if (GPROF)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    endif (GPROF)

    # Do not allow function inlining in debug mode.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-inline-functions -fno-inline")
  endif (${CMAKE_COMPILER_IS_GNUCXX})
elseif (${CMAKE_BUILD_TYPE} STREQUAL "Release")
  # Flags fog GCC.
  if (${CMAKE_COMPILER_IS_GNUCXX})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG=1")
  endif (${CMAKE_COMPILER_IS_GNUCXX})
endif (${CMAKE_BUILD_TYPE} STREQUAL "Debug")

# Allow cmake testing.
enable_testing()
include(CTest)

# Tests are added by using AddBoostTests.
include(AddBoostTests)
