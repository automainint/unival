/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_PRINT_H
#define UNIVAL_PRINT_H

#include "unival.h"
#include <cstddef>
#include <functional>

namespace unival {
  using fn_write = std::function<bool(std::string_view)>;

  struct mode_tag {
    bool is_pretty : 1;
    bool is_json : 1;
  };

  static constexpr auto compact =
      mode_tag { .is_pretty = false, .is_json = false };
  static constexpr auto pretty =
      mode_tag { .is_pretty = true, .is_json = false };
  static constexpr auto json_compact =
      mode_tag { .is_pretty = false, .is_json = true };
  static constexpr auto json_pretty =
      mode_tag { .is_pretty = true, .is_json = true };

  [[nodiscard]] auto print(unival const &value, fn_write const &write,
                           mode_tag mode = compact) noexcept -> bool;

  [[nodiscard]] auto to_string(unival const &value,
                               mode_tag mode = compact) noexcept
      -> std::optional<std::u8string>;
}

#endif
