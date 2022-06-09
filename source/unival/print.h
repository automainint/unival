/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_PRINT_H
#define UNIVAL_PRINT_H

#include "unival.h"
#include <cstddef>
#include <functional>

namespace unival {
  using fn_write_u8 = std::function<ptrdiff_t(std::u8string_view)>;

  [[nodiscard]] auto print(unival const &value,
                           fn_write_u8 const &write) noexcept -> bool;

  [[nodiscard]] auto to_string(unival const &value) noexcept
      -> std::optional<std::u8string>;
}

#endif
