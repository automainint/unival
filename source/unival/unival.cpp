/*  Copyright (c) 2022 Mitya Selivanov
 *
 *  This file is part of the Laplace project.
 *
 *  Laplace is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 *  the MIT License for more details.
 */

#include "unival.h"

namespace unival {
  using std::optional, std::nullopt, std::get, std::u8string, std::string_view,
      std::u8string_view, std::vector, std::span;

  static auto as_u8string(string_view s) {
    return u8string(s.begin(), s.end());
  }

  unival::unival(signed char value) noexcept
      : m_value(static_cast<long long int>(value)) {}
  unival::unival(signed short value) noexcept
      : m_value(static_cast<long long int>(value)) {}
  unival::unival(signed int value) noexcept
      : m_value(static_cast<long long int>(value)) {}
  unival::unival(signed long value) noexcept
      : m_value(static_cast<long long int>(value)) {}
  unival::unival(signed long long value) noexcept
      : m_value(static_cast<long long int>(value)) {}

  unival::unival(unsigned char value) noexcept
      : m_value(static_cast<long long int>(value)) {}
  unival::unival(unsigned short value) noexcept
      : m_value(static_cast<long long int>(value)) {}
  unival::unival(unsigned int value) noexcept
      : m_value(static_cast<long long int>(value)) {}
  unival::unival(unsigned long value) noexcept
      : m_value(static_cast<long long int>(value)) {}
  unival::unival(unsigned long long value) noexcept
      : m_value(static_cast<long long int>(value)) {}

  unival::unival(float value) noexcept : m_value(static_cast<double>(value)) {}
  unival::unival(double value) noexcept : m_value(value) {}

  unival::unival(string_view value) noexcept : m_value(as_u8string(value)) {}
  unival::unival(u8string_view value) noexcept : m_value(u8string { value }) {}

  unival::unival(vector<int8_t> const &value) noexcept : m_value(value) {}

  auto unival::is_empty() const noexcept -> bool {
    return m_value.index() == n_empty;
  }

  auto unival::is_integer() const noexcept -> bool {
    return m_value.index() == n_integer;
  }

  auto unival::is_float() const noexcept -> bool {
    return m_value.index() == n_float;
  }

  auto unival::is_string() const noexcept -> bool {
    return m_value.index() == n_string;
  }

  auto unival::is_bytes() const noexcept -> bool {
    return m_value.index() == n_bytes;
  }

  auto unival::get_integer() const noexcept -> optional<signed long long> {
    if (m_value.index() != n_integer)
      return nullopt;
    return get<n_integer>(m_value);
  }

  auto unival::get_uint() const noexcept -> optional<unsigned long long> {
    if (m_value.index() != n_integer)
      return nullopt;
    return static_cast<unsigned long long>(get<n_integer>(m_value));
  }

  auto unival::get_float() const noexcept -> optional<double> {
    if (m_value.index() != n_float)
      return nullopt;
    return get<n_float>(m_value);
  }

  auto unival::get_string() const noexcept -> optional<u8string_view> {
    if (m_value.index() != n_string)
      return nullopt;
    return get<n_string>(m_value);
  }

  auto unival::get_bytes() const noexcept -> optional<span<int8_t const>> {
    if (m_value.index() != n_bytes)
      return nullopt;
    auto const &v = get<n_bytes>(m_value);
    return span<int8_t const> { v.begin(), v.end() };
  }
}
