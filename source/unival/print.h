/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_PRINT_H
#define UNIVAL_PRINT_H

#include "unival.h"
#include <cstddef>
#include <functional>

namespace unival {
  using fn_write_u8 = std::function<bool(std::u8string_view)>;

  struct mode_tag {
    bool is_pretty;
  };

  static constexpr auto pretty = mode_tag { true };
  static constexpr auto compact = mode_tag { false };

  [[nodiscard]] auto print(unival const &value,
                           fn_write_u8 const &write,
                           mode_tag mode = compact) noexcept -> bool;

  [[nodiscard]] auto to_string(unival const &value,
                               mode_tag mode = compact) noexcept
      -> std::optional<std::u8string>;
}

#endif
