target_sources(
  ${UNIVAL_LIBRARY}
    PRIVATE
      unival.c
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/unival.h>)
