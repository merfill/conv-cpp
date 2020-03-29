
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CONV_CPP_ROOT_SOURCE_DIR}/conv-lib)
include_directories(${Python_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

set(NAME conv)
add_library(${NAME} MODULE run.cpp)
set_property(GLOBAL APPEND PROPERTY PY_MODULES_LIST ${NAME})
set_target_properties(${NAME} PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}")

target_link_libraries (${NAME}
    conv-impl
    ${Boost_LIBRARIES}
    ${Python_LIBRARIES}
)

#file(COPY run.py DESTINATION .)
#add_test(NAME Python_Run_Check COMMAND ${Python_EXECUTABLE} run.py)

