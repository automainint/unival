cmake_minimum_required(VERSION 3.16)
set(NAME unival-integration-fetch-content)
project(${NAME} C)

include(FetchContent)
FetchContent_Declare(
  unival
  GIT_REPOSITORY https://github.com/automainint/unival.git
  GIT_TAG dev)
set(UNIVAL_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(unival)

add_executable(${NAME})
target_sources(${NAME} PRIVATE main.c)
target_link_libraries(${NAME} PRIVATE unival::unival)

enable_testing()
add_test(NAME ${NAME}-run COMMAND ${NAME})
set_tests_properties(${NAME}-run PROPERTIES TIMEOUT "15")
