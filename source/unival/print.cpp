/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "print.h"

#include <iomanip>
#include <limits>
#include <sstream>

namespace unival {
  using std::ostringstream, std::string_view, std::u8string_view,
      std::u8string, std::optional, std::nullopt, std::numeric_limits,
      std::span, std::setprecision, std::right, std::setw,
      std::setfill, std::hex, std::noshowbase;

  [[nodiscard]] auto print_impl(unival const &value,
                                fn_write_u8 const &write,
                                bool is_nested_vector) noexcept
      -> bool;

  [[nodiscard]] auto as_u8string(string_view const &s) noexcept
      -> u8string_view {
    return u8string_view {
      reinterpret_cast<char8_t const *>(s.data()), s.size()
    };
  }

  [[nodiscard]] auto is_id_string(u8string_view const &s) noexcept
      -> bool {
    static const auto id_chars =
        u8string_view { u8"_abcdefghijklmnopqrstuvwxyz"
                        u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        u8"0123456789" };
    static const auto id_first =
        u8string_view { u8"_abcdefghijklmnopqrstuvwxyz"
                        u8"ABCDEFGHIJKLMNOPQRSTUVWXYZ" };

    if (s.empty())
      return false;

    if (id_first.find(s[0]) == u8string_view::npos)
      return false;

    for (ptrdiff_t i = 1; i < s.size(); i++)
      if (id_chars.find(s[i]) == u8string_view::npos)
        return false;

    return true;
  }

  [[nodiscard]] auto print_empty(fn_write_u8 const &write) noexcept
      -> bool {
    return write(u8"{}");
  }

  [[nodiscard]] auto print_boolean(bool value,
                                   fn_write_u8 const &write) noexcept
      -> bool {
    if (value)
      return write(u8"true");
    return write(u8"false");
  }

  [[nodiscard]] auto print_byte(int8_t byte,
                                fn_write_u8 const &write) noexcept {
    auto out = ostringstream {};
    out << right << setw(2) << setfill('0') << noshowbase << hex
        << static_cast<int>(static_cast<uint8_t>(byte));
    auto str = out.str();
    return write(as_u8string(str));
  }

  [[nodiscard]] auto print_integer(signed long long value,
                                   fn_write_u8 const &write) noexcept
      -> bool {
    auto out = ostringstream {};
    out << value;
    auto str = out.str();
    return write(as_u8string(str));
  }

  [[nodiscard]] auto print_float(double value,
                                 fn_write_u8 const &write) noexcept
      -> bool {
    auto out = ostringstream {};
    auto digits = numeric_limits<decltype(value)>::max_digits10;
    out << setprecision(digits) << value;
    auto str = out.str();
    if (!write(as_u8string(str)))
      return false;
    if (str.find('.') == string_view::npos &&
        str.find('e') == string_view::npos)
      return write(u8".0");
    return true;
  }

  [[nodiscard]] auto
  print_id_string(u8string_view const &value,
                  fn_write_u8 const &write) noexcept -> bool {
    return write(value);
  }

  [[nodiscard]] auto print_string(u8string_view const &value,
                                  fn_write_u8 const &write) noexcept
      -> bool {
    if (!write(u8"\""))
      return false;
    for (auto c : value) {
      if (c == '"' || c == '\\') {
        if (!write(u8"\\"))
          return false;
      }
      if (!write({ &c, 1 }))
        return false;
    }
    return write(u8"\"");
  }

  [[nodiscard]] auto
  print_byte_array(span<int8_t const> value,
                   fn_write_u8 const &write) noexcept -> bool {
    if (!write(u8"::{"))
      return false;
    for (ptrdiff_t i = 0; i < value.size(); i++) {
      auto const b = value[i];
      if (i > 0 && !write(u8" "))
        return false;
      if (!print_byte(b, write))
        return false;
    }
    return write(u8"}");
  }

  [[nodiscard]] auto print_vector(unival const &value,
                                  fn_write_u8 const &write,
                                  bool is_nested_vector) noexcept
      -> bool {
    bool need_brackets = is_nested_vector || value.get_size() < 2;
    if (need_brackets && !write(u8"["))
      return false;
    for (ptrdiff_t i = 0; i < value.get_size(); i++) {
      if (i > 0 && !write(u8","))
        return false;
      if (!print_impl(value.get(i), write, true))
        return false;
    }
    return !need_brackets || write(u8"]");
  }

  auto print_impl(unival const &value, fn_write_u8 const &write,
                  bool is_nested_vector) noexcept -> bool {
    if (value.is_empty())
      return print_empty(write);
    if (value.is_boolean())
      return print_boolean(value.get_boolean().value(), write);
    if (value.is_integer())
      return print_integer(value.get_integer().value(), write);
    if (value.is_float())
      return print_float(value.get_float().value(), write);
    if (value.is_string()) {
      auto str = value.get_string().value();
      if (is_id_string(str))
        return print_id_string(str, write);
      return print_string(str, write);
    }
    if (value.is_bytes())
      return print_byte_array(value.get_bytes().value(), write);
    if (value.is_vector())
      return print_vector(value, write, is_nested_vector);
    return false;
  }

  auto print(unival const &value, fn_write_u8 const &write) noexcept
      -> bool {
    return print_impl(value, write, false);
  }

  auto to_string(unival const &value) noexcept -> optional<u8string> {
    auto result = u8string {};

    if (!print(value, [&](u8string_view s) -> ptrdiff_t {
          result.append(s.begin(), s.end());
          return static_cast<ptrdiff_t>(s.size());
        }))
      return nullopt;

    return result;
  }
}
