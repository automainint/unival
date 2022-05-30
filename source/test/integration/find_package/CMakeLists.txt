cmake_minimum_required(VERSION 3.16)
set(NAME unival-integration-find-package)
project(${NAME}-root)

include(ExternalProject)

ExternalProject_Add(
  unival
  GIT_REPOSITORY  https://github.com/automainint/unival.git
  GIT_TAG         dev
  CMAKE_ARGS
  -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install)

ExternalProject_Add(
  ${NAME}
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source
  CMAKE_ARGS
  -D CMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_SOURCE_DIR}/install)

ExternalProject_Add_StepDependencies(${NAME} build unival)
