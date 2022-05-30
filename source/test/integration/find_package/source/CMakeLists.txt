cmake_minimum_required(VERSION 3.16)
set(NAME unival-integration-find-package)
project(${NAME})

find_package(unival)

add_executable(${NAME})
target_sources(${NAME} main.cpp)
target_link_libraries(${NAME} PRIVATE unival::unival)
