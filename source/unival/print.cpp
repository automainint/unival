/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "print.h"

#include <iomanip>
#include <limits>
#include <sstream>

namespace unival {
  using std::ostringstream, std::string_view, std::u8string_view,
      std::u8string, std::optional, std::nullopt, std::numeric_limits,
      std::setprecision;

  [[nodiscard]] auto as_u8string(string_view const &s) noexcept
      -> u8string_view {
    return u8string_view {
      reinterpret_cast<char8_t const *>(s.data()), s.size()
    };
  }

  [[nodiscard]] auto print_empty(fn_write_u8 const &write) noexcept
      -> bool {
    return write(u8"{}") == 2;
  }

  [[nodiscard]] auto print_boolean(bool value,
                                   fn_write_u8 const &write) noexcept
      -> bool {
    if (value)
      return write(u8"true") == 4;
    return write(u8"false") == 5;
  }

  [[nodiscard]] auto print_integer(signed long long value,
                                   fn_write_u8 const &write) noexcept
      -> bool {
    auto out = ostringstream {};
    out << value;
    auto str = out.str();
    return write(as_u8string(str)) == str.size();
  }

  [[nodiscard]] auto print_float(double value,
                                 fn_write_u8 const &write) noexcept
      -> bool {
    auto out = ostringstream {};
    auto digits = numeric_limits<decltype(value)>::max_digits10;
    out << setprecision(digits) << value;
    auto str = out.str();
    if (write(as_u8string(str)) != str.size())
      return false;
    if (str.find('.') == string_view::npos &&
        str.find('e') == string_view::npos)
      return write(u8".0") == 2;
    return true;
  }

  [[nodiscard]] auto print_string(u8string_view const &value,
                                  fn_write_u8 const &write) noexcept
      -> bool {
    if (write(u8"\"") != 1)
      return false;
    for (auto c : value) {
      if (c == '"' || c == '\\') {
        if (write(u8"\\") != 1)
          return false;
      }
      if (write({ &c, 1 }) != 1)
        return false;
    }
    return write(u8"\"") == 1;
  }

  auto print(unival const &value, fn_write_u8 const &write) noexcept
      -> bool {
    if (value.is_empty())
      return print_empty(write);
    if (value.is_boolean())
      return print_boolean(value.get_boolean().value(), write);
    if (value.is_integer())
      return print_integer(value.get_integer().value(), write);
    if (value.is_float())
      return print_float(value.get_float().value(), write);
    if (value.is_string())
      return print_string(value.get_string().value(), write);
    return false;
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
