/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "parse.h"

#include "input_buffer.h"
#include <array>
#include <cstdint>
#include <optional>
#include <span>
#include <tuple>

namespace unival {
  using std::optional, std::nullopt, std::u8string_view,
      std::string_view, std::u8string, std::min, std::tie, std::tuple,
      std::span, std::array;

  struct parse_result {
    unival value;
    input_buffer in;
  };

  [[nodiscard]] auto
  skip_whitespaces(optional<input_buffer> buf) noexcept
      -> optional<input_buffer> {
    if (!buf)
      return nullopt;
    auto next = buf->read(1);
    auto s = next.string();
    return s == " " || s == "\n" || s == "\t" ? skip_whitespaces(next)
                                              : buf;
  }

  [[nodiscard]] auto skip_string(optional<input_buffer> buf,
                                 string_view s) noexcept
      -> optional<input_buffer> {
    if (!buf)
      return nullopt;
    auto next = buf->read(1);
    return next.string() == s ? next : buf;
  }

  [[nodiscard]] auto read_string(optional<input_buffer> buf,
                                 string_view s) noexcept
      -> optional<input_buffer> {
    if (!buf)
      return nullopt;
    auto next = buf->read(s.size());
    if (next.string() == s)
      return next;
    return nullopt;
  }

  [[nodiscard]] auto read_sign(optional<input_buffer> buf) noexcept
      -> tuple<int, optional<input_buffer>> {
    if (auto next = read_string(buf, "+"); next)
      return { 1, next };
    if (auto next = read_string(buf, "-"); next)
      return { -1, next };
    return { 1, buf };
  }

  [[nodiscard]] auto read_one(optional<input_buffer> buf,
                              string_view chars) noexcept
      -> tuple<char, optional<input_buffer>> {
    if (!buf)
      return { '\0', nullopt };
    auto next = buf->read(1);
    auto s = next.string();
    if (s.empty())
      return { '\0', nullopt };
    if (chars.find(s) != string_view::npos)
      return { s[0], next };
    return { '\0', nullopt };
  }

  [[nodiscard]] auto parse_empty(optional<input_buffer> buf) noexcept
      -> parse_result {
    if (!buf)
      return { unival::_error() };
    auto next = skip_whitespaces(buf);
    next = read_string(next, "{");
    next = skip_whitespaces(next);
    next = read_string(next, "}");
    if (!next)
      return { unival::_error() };
    return { unival {}, *next };
  }

  [[nodiscard]] auto parse_null(optional<input_buffer> buf) noexcept
      -> parse_result {
    if (!buf)
      return { unival::_error() };
    auto next = skip_whitespaces(buf);
    next = read_string(next, "null");
    if (!next)
      return { unival::_error() };
    return { unival {}, *next };
  }

  [[nodiscard]] auto parse_true(optional<input_buffer> buf) noexcept
      -> parse_result {
    if (!buf)
      return { unival::_error() };
    auto next = skip_whitespaces(buf);
    next = read_string(next, "true");
    if (!next)
      return { unival::_error() };
    return { unival { true }, *next };
  }

  [[nodiscard]] auto parse_false(optional<input_buffer> buf) noexcept
      -> parse_result {
    if (!buf)
      return { unival::_error() };
    auto next = skip_whitespaces(buf);
    next = read_string(next, "false");
    if (!next)
      return { unival::_error() };
    return { unival { false }, *next };
  }

  [[nodiscard]] auto
  parse_integer(optional<input_buffer> buf, string_view prefix,
                int base, string_view digits,
                span<int const> digit_values) noexcept
      -> parse_result {
    auto digit_value = [&](char c) {
      return digit_values[digits.find(c)];
    };
    if (!buf)
      return { unival::_error() };
    auto next = skip_whitespaces(buf);
    auto sign = 1;
    auto value = int64_t {};
    auto c = char {};
    tie(sign, next) = read_sign(next);
    next = read_string(next, prefix);
    auto last = next;
    tie(c, next) = read_one(next, digits);
    if (!next)
      return { unival::_error() };
    do {
      value = value * base + digit_value(c);
      last = next;
      tie(c, next) = read_one(next, digits);
    } while (next);
    return { unival { sign * value }, *last };
  }

  [[nodiscard]] auto parse(optional<input_buffer> buf) noexcept
      -> parse_result {
    constexpr auto dec_base = 10;
    constexpr auto dec_digits = "0123456789";
    constexpr auto dec_values =
        array { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    constexpr auto bin_base = 2;
    constexpr auto bin_digits = "01";
    constexpr auto bin_values = array { 0, 1 };

    constexpr auto oct_base = 8;
    constexpr auto oct_digits = "01234567";
    constexpr auto oct_values = array { 0, 1, 2, 3, 4, 5, 6, 7 };

    constexpr auto hex_base = 16;
    constexpr auto hex_digits = "0123456789abcdefABCDEF";
    constexpr auto hex_values =
        array { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                11, 12, 13, 14, 15, 10, 11, 12, 13, 14, 15 };

    if (!buf)
      return { unival::_error() };
    if (auto res = parse_empty(buf); !res.value.error())
      return res;
    if (auto res = parse_null(buf); !res.value.error())
      return res;
    if (auto res = parse_true(buf); !res.value.error())
      return res;
    if (auto res = parse_false(buf); !res.value.error())
      return res;
    if (auto res = parse_integer(buf, "0b", bin_base, bin_digits,
                                 bin_values);
        !res.value.error())
      return res;
    if (auto res = parse_integer(buf, "0B", bin_base, bin_digits,
                                 bin_values);
        !res.value.error())
      return res;
    if (auto res = parse_integer(buf, "0o", oct_base, oct_digits,
                                 oct_values);
        !res.value.error())
      return res;
    if (auto res = parse_integer(buf, "0O", oct_base, oct_digits,
                                 oct_values);
        !res.value.error())
      return res;
    if (auto res = parse_integer(buf, "0x", hex_base, hex_digits,
                                 hex_values);
        !res.value.error())
      return res;
    if (auto res = parse_integer(buf, "0X", hex_base, hex_digits,
                                 hex_values);
        !res.value.error())
      return res;
    if (auto res =
            parse_integer(buf, "", dec_base, dec_digits, dec_values);
        !res.value.error())
      return res;
    return { unival::_error() };
  }

  auto parse(fn_read const &read) noexcept -> unival {
    auto result = parse(input_buffer { read });
    if (skip_whitespaces(result.in)->eof())
      return result.value;
    return unival::_error();
  }

  auto parse(u8string_view in) noexcept -> unival {
    return parse([&, i = ptrdiff_t {}](
                     ptrdiff_t size) mutable -> std::vector<char> {
      size = min(size, static_cast<ptrdiff_t>(in.size()) - i);
      auto buf =
          std::vector<char> { in.begin() + i, in.begin() + i + size };
      i += size;
      return buf;
    });
  }
}
