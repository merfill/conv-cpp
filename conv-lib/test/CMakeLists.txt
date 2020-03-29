
include_directories(${CONV_CPP_ROOT_SOURCE_DIR}/conv-lib)

set(UNIT_TEST_MODULE conv-lib-test)

set(UNIT_TEST_SOURCES
    test.cpp
)

add_executable(${UNIT_TEST_MODULE} ${UNIT_TEST_SOURCES})

target_link_libraries(${UNIT_TEST_MODULE}
    conv-impl
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_boost_tests("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${UNIT_TEST_MODULE}" "--catch_system_errors=no" ${UNIT_TEST_SOURCES})
