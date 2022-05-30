cmake_minimum_required(VERSION 3.16)

option(UNIVAL_ENABLE_TESTING "Enable testing" ON)

set(UNIVAL_PROJECT unival)
set(UNIVAL_LIBRARY unival)
set(UNIVAL_TEST unival-test)
set(UNIVAL_UNITTESTS unival-unittests)
set(UNIVAL_TARGETS unival-targets)

if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
  set(CMAKE_BUILD_PARALLEL_LEVEL 3)
endif()

project(
  ${UNIVAL_PROJECT}
  VERSION 0.1.1
  DESCRIPTION "Universal data container"
  LANGUAGES CXX)

add_library(${UNIVAL_LIBRARY} STATIC)
add_library(${UNIVAL_PROJECT}::${UNIVAL_LIBRARY} ALIAS ${UNIVAL_LIBRARY})
target_include_directories(
  ${UNIVAL_LIBRARY} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)
target_compile_features(${UNIVAL_LIBRARY} PUBLIC cxx_std_20)

enable_testing()

if(UNIVAL_ENABLE_TESTING)
  include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.9)

  FetchContent_MakeAvailable(Catch2)

  add_executable(${UNIVAL_TEST})

  target_link_libraries(
    ${UNIVAL_TEST}
    PRIVATE
    ${UNIVAL_LIBRARY} Catch2::Catch2)

  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
      ${UNIVAL_TEST} PRIVATE
      #-fsanitize=undefined,address
      --coverage -O0 -g)
    target_link_options(
      ${UNIVAL_TEST} PRIVATE
      #-fsanitize=undefined,address
      --coverage)
  endif()

  if(MSVC)
    target_compile_options(${UNIVAL_TEST} PRIVATE /Od)
  endif()

  add_test(
    NAME ${UNIVAL_UNITTESTS}
    COMMAND ${UNIVAL_TEST})

  set_tests_properties(
    ${UNIVAL_UNITTESTS}
    PROPERTIES
    TIMEOUT "15")
endif()

add_subdirectory(source)

include(GNUInstallDirs)

file(
  GLOB_RECURSE headers_
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/source/unival
  ${CMAKE_CURRENT_SOURCE_DIR}/source/unival/*.h)

foreach(path_ ${headers_})
  get_filename_component(dir_ "${path_}" DIRECTORY)

  install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/source/unival/${path_}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/unival/${dir_})
endforeach()

unset(headers_)
unset(path_)
unset(dir_)

install(
  TARGETS ${UNIVAL_LIBRARY}
  EXPORT ${UNIVAL_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT ${UNIVAL_TARGETS}
  FILE ${UNIVAL_TARGETS}.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

export(
  EXPORT ${UNIVAL_TARGETS}
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/${UNIVAL_TARGETS}.cmake"
  NAMESPACE ${PROJECT_NAME}::)

include(CMakePackageConfigHelpers)

string(TOLOWER ${PROJECT_NAME} project_lower_)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config-version.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion)

install(
  FILES
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/${project_lower_}-config-version.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

unset(project_lower_)
