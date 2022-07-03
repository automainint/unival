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
      std::span, std::array, std::string;

  constexpr static auto dec_base = 10;
  constexpr static auto dec_digits = "0123456789";
  constexpr static auto dec_values =
      array { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  constexpr static auto bin_base = 2;
  constexpr static auto bin_digits = "01";
  constexpr static auto bin_values = array { 0, 1 };

  constexpr static auto oct_base = 8;
  constexpr static auto oct_digits = "01234567";
  constexpr static auto oct_values = array { 0, 1, 2, 3, 4, 5, 6, 7 };

  constexpr static auto hex_base = 16;
  constexpr static auto hex_digits = "0123456789abcdefABCDEF";
  constexpr static auto hex_values =
      array { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
              11, 12, 13, 14, 15, 10, 11, 12, 13, 14, 15 };

  struct or_eof_tag {
    bool success_if_eof = false;
  };

  constexpr static auto or_eof = or_eof_tag { true };

  [[nodiscard]] auto
  skip_until(optional<input_buffer> buf, string_view s,
             or_eof_tag tag = or_eof_tag { false }) noexcept
      -> optional<input_buffer> {
    if (!buf)
      return nullopt;
    auto next = buf;
    while (!next->eof()) {
      if (next->read(s.size()).string() == s)
        return next;
      next = next->read(1);
    }
    if (tag.success_if_eof)
      return next;
    return nullopt;
  }

  [[nodiscard]] auto skip_comment(optional<input_buffer> buf) noexcept
      -> optional<input_buffer> {
    if (!buf)
      return nullopt;
    auto next = buf->read(2);
    if (next.string() == "//")
      return skip_until(next, "\n", or_eof);
    if (next.string() == "/*")
      return skip_until(next, "*/")->read(2);
    return buf;
  }

  [[nodiscard]] auto
  skip_whitespaces(optional<input_buffer> buf) noexcept
      -> optional<input_buffer> {
    if (!buf)
      return nullopt;
    auto next = skip_comment(buf);
    auto last = next->read(1);
    auto s = last.string();
    return s == " " || s == "\r" || s == "\n" || s == "\t"
               ? skip_whitespaces(last)
               : next;
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

  struct parse_result {
    unival value;
    input_buffer in;
  };

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

  [[nodiscard]] auto parse_digits(optional<input_buffer> buf,
                                  string_view digits) noexcept
      -> tuple<string, optional<input_buffer>> {
    auto value = string {};
    auto [c, next] = read_one(buf, digits);
    auto last = optional<input_buffer> {};
    if (!next)
      return {};
    do {
      value.append(1, c);
      last = next;
      tie(c, next) = read_one(next, digits);
    } while (next);
    return { value, last };
  }

  [[nodiscard]] auto
  convert_digits(string_view value, int base, string_view digits,
                 span<int const> digit_values) noexcept
      -> signed long long {
    auto result = int64_t {};
    for (auto c : value) {
      auto i = digits.find(c);
      result *= base;
      result += digit_values[i];
    }
    return result;
  }

  [[nodiscard]] auto
  parse_uint_internal(optional<input_buffer> buf, string_view prefix,
                      int base, string_view digits,
                      span<int const> digit_values) noexcept
      -> tuple<signed long long, optional<input_buffer>> {
    if (!buf)
      return {};
    auto c = char {};
    auto next = read_string(buf, prefix);
    auto [number, last] = parse_digits(next, digits);
    if (!last)
      return {};
    return { convert_digits(number, base, digits, digit_values),
             last };
  }

  [[nodiscard]] auto
  parse_int_internal(optional<input_buffer> buf, string_view prefix,
                     int base, string_view digits,
                     span<int const> digit_values) noexcept
      -> tuple<signed long long, optional<input_buffer>> {
    if (!buf)
      return {};
    auto [sign, next] = read_sign(buf);
    auto [value, last] =
        parse_uint_internal(next, prefix, base, digits, digit_values);
    return { sign * value, last };
  }

  [[nodiscard]] auto
  parse_integer(optional<input_buffer> buf, string_view prefix,
                int base, string_view digits,
                span<int const> digit_values) noexcept
      -> parse_result {
    if (!buf)
      return { unival::_error() };
    auto next = skip_whitespaces(buf);
    auto [value, last] =
        parse_int_internal(next, prefix, base, digits, digit_values);
    if (!last)
      return { unival::_error() };
    return { unival { value }, *last };
  }

  [[nodiscard]] auto parse_float(optional<input_buffer> buf) noexcept
      -> parse_result {
    auto pow = [](long double base, int64_t n) {
      auto x = static_cast<long double>(1);
      auto m = n > 0 ? n : -n;
      for (auto i = int64_t {}; i < m; ++i)
        x *= base;
      return n < 0 ? static_cast<long double>(1) / x : x;
    };
    if (!buf)
      return { unival::_error() };
    auto sign = 1;
    auto next = skip_whitespaces(buf);
    tie(sign, next) = read_sign(next);
    auto [n_int, next_int] = parse_uint_internal(
        next, "", dec_base, dec_digits, dec_values);
    auto next_frac = optional<input_buffer> {};
    auto next_exp = optional<input_buffer> {};
    auto s_frac = string {};
    if (next_int)
      next = next_int;
    auto next_dot = read_string(next, ".");
    if (next_dot) {
      next = next_dot;
      tie(s_frac, next_frac) = parse_digits(next, dec_digits);
      if (next_frac)
        next = next_frac;
    }
    auto n_exp = int64_t {};
    if (next_exp = read_string(next, "e"); next_exp) {
      next = next_exp;
      tie(n_exp, next) = parse_int_internal(next, "", dec_base,
                                            dec_digits, dec_values);
    }
    if (!next_int && !next_frac)
      return { unival::_error() };
    if (!next_dot && !next_exp)
      return { unival::_error() };
    if (!next)
      return { unival::_error() };
    return {
      unival { static_cast<double>(
          (static_cast<long double>(sign * n_int) +
           static_cast<long double>(convert_digits(
               s_frac, dec_base, dec_digits, dec_values)) /
               pow(dec_base, static_cast<int64_t>(s_frac.size()))) *
          pow(dec_base, n_exp)) },
      *next
    };
  }

  [[nodiscard]] auto parse(optional<input_buffer> buf) noexcept
      -> parse_result {
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
    if (auto res = parse_float(buf); !res.value.error())
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
