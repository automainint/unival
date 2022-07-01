target_sources(
  ${UNIVAL_LIBRARY}
    PRIVATE
      chain.cpp unival.cpp input_buffer.cpp iterator.cpp
      parse.cpp print.cpp
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/unival.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/parse.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/iterator.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/print.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/input_buffer.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/chain.h>)
