cmake_minimum_required(VERSION 3.16)
set(NAME unival_integration_find_package)
project(${NAME} C)

find_package(unival REQUIRED)

add_executable(${NAME})
target_sources(${NAME} PRIVATE main.c)
target_link_libraries(${NAME} PRIVATE unival::unival)
install(TARGETS ${NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
