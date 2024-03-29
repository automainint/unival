cmake_minimum_required(VERSION 3.16)

option(UNIVAL_ENABLE_TESTING "Enable testing" ON)

set(UNIVAL_PROJECT unival)
set(UNIVAL_LIBRARY unival)
set(UNIVAL_TEST unival_test)
set(UNIVAL_TARGETS unival-targets)

if(NOT DEFINED CMAKE_BUILD_PARALLEL_LEVEL)
  set(CMAKE_BUILD_PARALLEL_LEVEL 4)
endif()

project(
  ${UNIVAL_PROJECT}
  VERSION 0.1.1
  DESCRIPTION "Universal data container"
  LANGUAGES C)

include(FetchContent)

find_package(kit QUIET)

if(NOT kit_FOUND)
  FetchContent_Declare(
    kit
    GIT_REPOSITORY https://github.com/automainint/kit.git
    GIT_TAG dev)
  set(KIT_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(kit)
endif()

add_library(${UNIVAL_LIBRARY} STATIC)
add_library(${UNIVAL_PROJECT}::${UNIVAL_LIBRARY} ALIAS ${UNIVAL_LIBRARY})
target_include_directories(
  ${UNIVAL_LIBRARY} INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/source>)
target_compile_features(${UNIVAL_LIBRARY} PUBLIC c_std_11)
target_link_libraries(${UNIVAL_LIBRARY} PUBLIC kit::kit)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(${UNIVAL_LIBRARY} PUBLIC -pedantic -Wall -Werror)
endif()

enable_testing()

if(UNIVAL_ENABLE_TESTING)
  find_package(Threads REQUIRED)

  add_executable(${UNIVAL_TEST})
  add_executable(${UNIVAL_PROJECT}::${UNIVAL_TEST} ALIAS ${UNIVAL_TEST})
  target_compile_features(${UNIVAL_TEST} PRIVATE c_std_11)
  target_link_libraries(${UNIVAL_TEST} PRIVATE ${UNIVAL_LIBRARY} kit::kit_test)

  if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
      ${UNIVAL_LIBRARY} PUBLIC
      -fsanitize=undefined,address
      --coverage -O0 -g)
    target_link_options(
      ${UNIVAL_LIBRARY} PUBLIC
      -fsanitize=undefined,address
      --coverage)
  endif()

  add_test(
    NAME ${UNIVAL_LIBRARY}_unittests
    COMMAND ${UNIVAL_TEST} --no-carriage-return)

  set_tests_properties(
    ${UNIVAL_LIBRARY}_unittests
    PROPERTIES
    TIMEOUT "120")
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
  TARGETS ${UNIVAL_LIBRARY} ${UNIVAL_TEST_LIBRARY}
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
  ${CMAKE_CURRENT_SOURCE_DIR}/config.cmake.in
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
