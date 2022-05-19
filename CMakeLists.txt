cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
  set(CMAKE_BUILD_PARALLEL_LEVEL 3)
endif()

set(UNIVAL_PROJECT unival)
set(UNIVAL_TEST unival-test)
set(UNIVAL_UNITTESTS unival-unittests)

project(
  ${UNIVAL_PROJECT}
  VERSION 0.1.1
  DESCRIPTION "Universal data container"
  LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.9)

FetchContent_MakeAvailable(Catch2)

add_executable(${UNIVAL_TEST})
target_compile_features(${UNIVAL_TEST} PRIVATE cxx_std_20)
target_link_libraries(${UNIVAL_TEST} PRIVATE Catch2::Catch2)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(
    ${UNIVAL_TEST} PRIVATE
    -fsanitize=undefined,address --coverage -O0 -g)
  target_link_options(
    ${UNIVAL_TEST} PRIVATE
    -fsanitize=undefined,address --coverage)
endif()

if(MSVC)
  target_compile_options(${UNIVAL_TEST} INTERFACE /Od)
endif()

add_subdirectory(source)

enable_testing()

add_test(
  NAME ${UNIVAL_UNITTESTS}
  COMMAND ${UNIVAL_TEST})

set_tests_properties(
  ${UNIVAL_UNITTESTS}
  PROPERTIES
  TIMEOUT "15")
