target_sources(
  ${UNIVAL_LIBRARY}
    PRIVATE
      chain.cpp unival.cpp iterator.cpp
    PUBLIC
      unival.h iterator.h chain.h)
