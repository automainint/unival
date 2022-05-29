target_sources(
  ${UNIVAL_TEST}
    PRIVATE
      chain.cpp unival.cpp iterator.cpp
    PUBLIC
      unival.h iterator.h chain.h)
