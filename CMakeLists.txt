cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
  set(CMAKE_BUILD_PARALLEL_LEVEL 3)
endif()

set(UNIVAL_PROJECT unival)
set(UNIVAL_TEST unival-test)
set(UNIVAL_UNITTESTS unival-unittests)

project(${UNIVAL_PROJECT}
        VERSION 0.1.1
        DESCRIPTION "Universal data container"
        LANGUAGES CXX)

add_executable(${UNIVAL_TEST})
add_subdirectory(source)

target_compile_features(${UNIVAL_TEST} PRIVATE cxx_std_20)
target_compile_options(${UNIVAL_TEST} PRIVATE -O0 -g --coverage)
target_link_options(${UNIVAL_TEST} PRIVATE --coverage)

enable_testing()

add_test(NAME ${UNIVAL_UNITTESTS}
         COMMAND ${UNIVAL_TEST})

set_tests_properties(${UNIVAL_UNITTESTS}
                     PROPERTIES
                     TIMEOUT "15")
