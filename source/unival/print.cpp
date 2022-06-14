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
                                bool is_nested_vector, mode_tag mode,
                                int indent) noexcept -> bool;

  [[nodiscard]] auto write_indent(fn_write_u8 const &write,
                                  int indent) -> bool {
    for (int i = 0; i < indent; i++)
      if (!write(u8" "))
        return false;
    return true;
  }

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

  [[nodiscard]] auto print_empty(fn_write_u8 const &write,
                                 mode_tag mode) noexcept -> bool {
    if (mode.is_pretty)
      return write(u8"{ }");
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
      if (c < u8'\x20' || c >= u8'\x7f') {
        if (!write(u8"\\x"))
          return false;
        if (!print_byte(static_cast<int8_t>(c), write))
          return false;
        continue;
      }
      if (c == '"' || c == '\\') {
        if (!write(u8"\\"))
          return false;
      }
      if (!write({ &c, 1 }))
        return false;
    }
    return write(u8"\"");
  }

  [[nodiscard]] auto print_byte_array(span<int8_t const> value,
                                      fn_write_u8 const &write,
                                      mode_tag mode,
                                      int indent) noexcept -> bool {
    if (value.empty())
      return write(mode.is_pretty ? u8":: { }" : u8"::{}");
    if (!write(mode.is_pretty ? u8":: {" : u8"::{"))
      return false;
    for (ptrdiff_t i = 0, k = 0; i < value.size(); i++) {
      auto const b = value[i];
      if (k == 0 && mode.is_pretty &&
          (!write(u8"\n") || !write_indent(write, indent + 2)))
        return false;
      if (k > 0 && !write(u8" "))
        return false;
      if (!print_byte(b, write))
        return false;
      k++;
      if (mode.is_pretty)
        k = k % 16;
    }
    if (mode.is_pretty)
      if (!write(u8"\n") || !write_indent(write, indent))
        return false;
    return write(u8"}");
  }

  [[nodiscard]] auto print_vector(unival const &value,
                                  fn_write_u8 const &write,
                                  bool is_nested_vector,
                                  mode_tag mode, int indent) noexcept
      -> bool {
    if (mode.is_pretty && value.get_size() == 0)
      return write(u8"[ ]");
    if (!write(u8"["))
      return false;
    if (mode.is_pretty && !write(u8"\n"))
      return false;
    for (ptrdiff_t i = 0; i < value.get_size(); i++) {
      if (mode.is_pretty && !write_indent(write, indent + 2))
        return false;
      if (!print_impl(value.get(i), write, true, mode, indent + 2))
        return false;
      if (i + 1 < value.get_size() && !write(u8","))
        return false;
      if (mode.is_pretty && !write(u8"\n"))
        return false;
    }
    if (mode.is_pretty && !write_indent(write, indent))
      return false;
    return write(u8"]");
  }

  [[nodiscard]] auto print_composite(unival const &value,
                                     fn_write_u8 const &write,
                                     mode_tag mode,
                                     int indent) noexcept -> bool {
    if (mode.is_pretty && value.get_size() == 0)
      return write(u8"{ }");
    if (!write(u8"{"))
      return false;
    if (mode.is_pretty && !write(u8"\n"))
      return false;
    bool first = true;
    auto i = ptrdiff_t {};
    for (auto const &key : value) {
      if (mode.is_pretty) {
        if (!write_indent(write, indent + 2))
          return false;
      } else if (!first) {
        if (!mode.is_json && !write(u8" "))
          return false;
      } else
        first = false;
      if (!print_impl(key, write, false, mode, indent + 2))
        return false;
      if (!write(u8":"))
        return false;
      if (mode.is_pretty && !write(u8" "))
        return false;
      if (!print_impl(value.get(key), write, false, mode, indent + 2))
        return false;
      if (mode.is_json) {
        if (++i != value.get_size() && !write(u8","))
          return false;
        if (mode.is_pretty && !write(u8"\n"))
          return false;
      } else if (mode.is_pretty && !write(u8";\n"))
        return false;
    }
    if (mode.is_pretty && !write_indent(write, indent))
      return false;
    return write(u8"}");
  }

  auto print_impl(unival const &value, fn_write_u8 const &write,
                  bool is_nested_vector, mode_tag mode,
                  int indent) noexcept -> bool {
    if (value.is_empty())
      return print_empty(write, mode);
    if (value.is_boolean())
      return print_boolean(value.get_boolean().value(), write);
    if (value.is_integer())
      return print_integer(value.get_integer().value(), write);
    if (value.is_float())
      return print_float(value.get_float().value(), write);
    if (value.is_string()) {
      auto str = value.get_string().value();
      if (!mode.is_json && is_id_string(str))
        return print_id_string(str, write);
      return print_string(str, write);
    }
    if (value.is_bytes())
      return print_byte_array(value.get_bytes().value(), write, mode,
                              indent);
    if (value.is_vector())
      return print_vector(value, write, is_nested_vector, mode,
                          indent);
    if (value.is_composite())
      return print_composite(value, write, mode, indent);
    return false;
  }

  auto print(unival const &value, fn_write_u8 const &write,
             mode_tag mode) noexcept -> bool {
    return print_impl(value, write, false, mode, 0);
  }

  auto to_string(unival const &value, mode_tag mode) noexcept
      -> optional<u8string> {
    auto result = u8string {};

    if (!print(
            value,
            [&](u8string_view s) -> ptrdiff_t {
              result.append(s.begin(), s.end());
              return static_cast<ptrdiff_t>(s.size());
            },
            mode))
      return nullopt;

    return result;
  }
}
