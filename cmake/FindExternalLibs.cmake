
# ************************ Boost **********************

# Compose boost libraries as static.
set(Boost_USE_STATIC_LIBS ON)

# Use the multithreaded version.
set(Boost_USE_MULTITHREADED ON)

# Does not replace the standard library.
set(Boost_USE_STATIC_RUNTIME OFF)

message(STATUS "Searching for Boost...")
find_package(Boost 1.67.0 EXACT COMPONENTS
  unit_test_framework
  python
  numpy
)

if (Boost_FOUND)
  message(STATUS "Boost library version: " ${Boost_LIB_VERSION})
  message(STATUS "Boost include dirs: " ${Boost_INCLUDE_DIRS})
  message(STATUS "Boost library dirs: " ${Boost_LIBRARY_DIRS})
  message(STATUS "Boost libraries: " ${Boost_LIBRARIES})
else ()
  message(FATAL_ERROR "Some boost libraries not found")
endif ()

# ********************* Python *************************

#set(Python_USE_STATIC_LIBS TRUE)

# Search python
message(STATUS "Searching for Python...")
find_package(Python 3 COMPONENTS Interpreter Development)
if (Python_FOUND)
  message(STATUS "  Python interpreter found: " ${Python_EXECUTABLE})
  message(STATUS "  Python version: " ${Python_VERSION})
  message(STATUS "  Python include dirs: " ${Python_INCLUDE_DIRS})
  message(STATUS "  Python library dirs: " ${Python_LIBRARY_DIRS})
  message(STATUS "  Python libraries: " ${Python_LIBRARIES})
else ()
  message(FATAL_ERROR "Python not found, install python package")
endif ()

# ********************** CUDA *************************
message(STATUS "Searching for CUDA Toolkit...")
find_package(CUDA REQUIRED)
if (CUDA_FOUND)
  message(STATUS "  Found CUDA version: ${CUDA_VERSION}")
  message(STATUS "  Found CUDA toolkit dir: ${CUDA_TOOLKIT_ROOT_DIR}")
  message(STATUS "  Found CUDA include dirs: ${CUDA_INCLUDE_DIRS}")
else ()
  message(FATAL_ERROR "CUDA Toolkit not found. Install CUDA")
endif ()
