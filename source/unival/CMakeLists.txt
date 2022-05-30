target_sources(
  ${UNIVAL_LIBRARY}
    PRIVATE
      chain.cpp unival.cpp iterator.cpp
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/unival.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/iterator.h>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/chain.h>)
