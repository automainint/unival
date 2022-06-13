/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_PARSE_H
#define UNIVAL_PARSE_H

#include "unival.h"
#include <functional>

namespace unival {
  using fn_read_u8 =
      std::function<std::u8string(ptrdiff_t)>;

  [[nodiscard]] auto parse(fn_read_u8 const &read) noexcept -> unival;

  [[nodiscard]] auto parse(std::u8string_view in) noexcept -> unival;
}

#endif
