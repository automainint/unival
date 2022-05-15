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
  using std::optional, std::nullopt, std::get;

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

  auto unival::is_empty() const noexcept -> bool {
    return m_value.index() == n_empty;
  }

  auto unival::is_integer() const noexcept -> bool {
    return m_value.index() == n_integer;
  }

  auto unival::is_float() const noexcept -> bool {
    return m_value.index() == n_float;
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
}
