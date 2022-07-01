/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_PARSE_H
#define UNIVAL_PARSE_H

#include "read.h"
#include "unival.h"

namespace unival {
  [[nodiscard]] auto parse(fn_read const &read) noexcept -> unival;

  [[nodiscard]] auto parse(std::u8string_view in) noexcept -> unival;
}

#endif
