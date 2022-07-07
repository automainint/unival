/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "print.h"

#include <iomanip>
#include <limits>
#include <sstream>

namespace unival {
  using std::ostringstream, std::string_view, std::u8string_view,
      std::string, std::u8string, std::optional, std::nullopt,
      std::numeric_limits, std::span, std::setprecision, std::right,
      std::setw, std::setfill, std::hex, std::noshowbase;

  [[nodiscard]] auto print_impl(bool status, unival const &value,
                                fn_write const &write, mode_tag mode,
                                int indent) noexcept -> bool;

  [[nodiscard]] auto bind(bool status, auto const &f) noexcept
      -> bool {
    if (!status)
      return false;
    return f();
  }

  [[nodiscard]] auto bind_write(bool status, auto const &f,
                                std::string_view s) noexcept -> bool {
    if (!status)
      return false;
    auto res = f(s);
    return res;
  }

  [[nodiscard]] auto write_indent(bool status, fn_write const &write,
                                  int indent) -> bool {
    if (!status)
      return false;
    for (int i = 0; i < indent; i++)
      if (!write(" "))
        return false;
    return true;
  }

  [[nodiscard]] auto as_string(u8string_view const &s) noexcept
      -> string_view {
    return string_view { reinterpret_cast<char const *>(s.data()),
                         s.size() };
  }

  [[nodiscard]] auto is_id_string(u8string_view const &s) noexcept
      -> bool {
    static const auto id_chars =
        string_view { "_abcdefghijklmnopqrstuvwxyz"
                      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                      "0123456789" };
    static const auto id_first =
        string_view { "_abcdefghijklmnopqrstuvwxyz"
                      "ABCDEFGHIJKLMNOPQRSTUVWXYZ" };
    if (s.empty())
      return false;
    if (id_first.find(s[0]) == string_view::npos)
      return false;
    for (ptrdiff_t i = 1; i < s.size(); i++)
      if (id_chars.find(s[i]) == string_view::npos)
        return false;
    return true;
  }

  [[nodiscard]] auto print_empty(bool ok, fn_write const &write,
                                 mode_tag mode) noexcept -> bool {
    if (mode.is_json)
      return bind_write(ok, write, "null");
    if (mode.is_pretty)
      return bind_write(ok, write, "{ }");
    return bind_write(ok, write, "{}");
  }

  [[nodiscard]] auto print_boolean(bool ok, bool value,
                                   fn_write const &write) noexcept
      -> bool {
    if (value)
      return bind_write(ok, write, "true");
    return bind_write(ok, write, "false");
  }

  [[nodiscard]] auto print_byte(bool ok, int8_t byte,
                                fn_write const &write) noexcept {
    auto out = ostringstream {};
    out << right << setw(2) << setfill('0') << noshowbase << hex
        << static_cast<int>(static_cast<uint8_t>(byte));
    auto str = out.str();
    return bind_write(ok, write, str);
  }

  [[nodiscard]] auto print_integer(bool ok, signed long long value,
                                   fn_write const &write) noexcept
      -> bool {
    auto out = ostringstream {};
    out << value;
    auto str = out.str();
    return bind_write(ok, write, str);
  }

  [[nodiscard]] auto print_float(bool ok, double value,
                                 fn_write const &write) noexcept
      -> bool {
    auto out = ostringstream {};
    auto digits = numeric_limits<decltype(value)>::max_digits10;
    out << setprecision(digits) << value;
    auto str = out.str();
    ok = bind_write(ok, write, str);
    if (str.find('.') == string_view::npos &&
        str.find('e') == string_view::npos)
      ok = bind_write(ok, write, ".0");
    return ok;
  }

  [[nodiscard]] auto print_id_string(bool status,
                                     string_view const &value,
                                     fn_write const &write) noexcept
      -> bool {
    return bind_write(status, write, value);
  }

  [[nodiscard]] auto print_string(bool status,
                                  string_view const &value,
                                  fn_write const &write,
                                  mode_tag mode) noexcept -> bool {
    return bind(status, [&]() {
      auto ok = write("\"");
      bool is_escaped = false;
      for (auto c : value) {
        if (c < u8'\x20' || c >= u8'\x7f') {
          ok = bind_write(ok, write, "\\x");
          ok = print_byte(ok, static_cast<int8_t>(c), write);
          is_escaped = true;
        } else {
          if (c == '"' || c == '\\')
            ok = bind_write(ok, write, "\\");
          else if (is_escaped &&
                   string_view { "0123456789abcdefABCDEF" }.find(c) !=
                       string_view::npos)
            ok = bind_write(ok, write,
                            mode.is_pretty ? "\" \"" : "\"\"");
          ok = bind_write(ok, write, { &c, 1 });
          is_escaped = false;
        }
      }
      return bind_write(ok, write, "\"");
    });
  }

  [[nodiscard]] auto print_byte_array(bool status,
                                      span<int8_t const> value,
                                      fn_write const &write,
                                      mode_tag mode,
                                      int indent) noexcept -> bool {
    return bind(status, [&]() {
      if (value.empty())
        return write(mode.is_pretty ? "< >" : "<>");
      auto ok = write("<");
      for (ptrdiff_t i = 0, k = 0; i < value.size(); i++) {
        auto const b = value[i];
        if (k == 0 && mode.is_pretty) {
          ok = bind_write(ok, write, "\n");
          ok = write_indent(ok, write, indent + 2);
        }
        if (k > 0)
          ok = bind_write(ok, write, " ");
        ok = print_byte(ok, b, write);
        k++;
        if (mode.is_pretty)
          k = k % 16;
      }
      if (mode.is_pretty) {
        ok = bind_write(ok, write, "\n");
        ok = write_indent(ok, write, indent);
      }
      return bind_write(ok, write, ">");
    });
  }

  [[nodiscard]] auto print_vector(bool status, unival const &value,
                                  fn_write const &write,
                                  mode_tag mode, int indent) noexcept
      -> bool {
    return bind(status, [&]() {
      if (mode.is_pretty && value.size() == 0)
        return write("[ ]");
      auto ok = write("[");
      if (mode.is_pretty)
        ok = bind_write(ok, write, "\n");
      for (ptrdiff_t i = 0; i < value.size(); i++) {
        if (mode.is_pretty)
          ok = write_indent(ok, write, indent + 2);
        ok = print_impl(ok, value.get(i), write, mode, indent + 2);
        if (i + 1 < value.size())
          ok = bind_write(ok, write, ",");
        if (mode.is_pretty)
          ok = bind_write(ok, write, "\n");
      }
      if (mode.is_pretty)
        ok = write_indent(ok, write, indent);
      return bind_write(ok, write, "]");
    });
  }

  [[nodiscard]] auto print_composite(bool status, unival const &value,
                                     fn_write const &write,
                                     mode_tag mode,
                                     int indent) noexcept -> bool {
    return bind(status, [&]() {
      if (mode.is_pretty && value.size() == 0)
        return write("{ }");
      auto ok = write("{");
      if (mode.is_pretty)
        ok = bind_write(ok, write, "\n");
      auto i = ptrdiff_t {};
      for (auto const &key : value) {
        if (mode.is_pretty)
          ok = write_indent(ok, write, indent + 2);
        ok = print_impl(ok, key, write, mode, indent + 2);
        ok = bind_write(ok, write, ":");
        if (mode.is_pretty)
          ok = bind_write(ok, write, " ");
        ok = print_impl(ok, value.get(key), write, mode, indent + 2);
        if (++i != value.size()) {
          if (mode.is_json)
            ok = bind_write(ok, write, ",");
          else
            ok = bind_write(ok, write, ";");
        } else if (mode.is_pretty && !mode.is_json)
          ok = bind_write(ok, write, ";");
        if (mode.is_pretty)
          ok = bind_write(ok, write, "\n");
      }
      if (mode.is_pretty)
        ok = write_indent(ok, write, indent);
      ok = bind_write(ok, write, "}");
      return ok;
    });
  }

  auto print_impl(bool status, unival const &value,
                  fn_write const &write, mode_tag mode,
                  int indent) noexcept -> bool {
    return bind(status, [&]() {
      if (value.empty())
        return print_empty(true, write, mode);
      if (value.boolean())
        return print_boolean(true, value.boolean().value(), write);
      if (value.integer())
        return print_integer(true, value.integer().value(), write);
      if (value.real())
        return print_float(true, value.real().value(), write);
      if (value.string()) {
        auto str = value.string().value();
        if (!mode.is_json && is_id_string(str))
          return print_id_string(true, as_string(str), write);
        return print_string(true, as_string(str), write, mode);
      }
      if (value.bytes())
        return print_byte_array(true, value.bytes().value(), write,
                                mode, indent);
      if (value.vector())
        return print_vector(true, value, write, mode, indent);
      if (value.composite())
        return print_composite(true, value, write, mode, indent);
      return false;
    });
  }

  auto print(unival const &value, fn_write const &write,
             mode_tag mode) noexcept -> bool {
    return print_impl(true, value, write, mode, 0);
  }

  auto to_string(unival const &value, mode_tag mode) noexcept
      -> optional<u8string> {
    auto result = u8string {};

    if (!print(
            value,
            [&](string_view s) -> ptrdiff_t {
              result.append(s.begin(), s.end());
              return static_cast<ptrdiff_t>(s.size());
            },
            mode))
      return nullopt;

    return result;
  }
}
