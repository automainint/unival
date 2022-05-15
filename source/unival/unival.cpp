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
  unival::unival() noexcept {}

  unival::unival(signed char value) noexcept
      : m_is_empty(false), m_value(value) {}
  unival::unival(signed short value) noexcept
      : m_is_empty(false), m_value(value) {}
  unival::unival(signed int value) noexcept
      : m_is_empty(false), m_value(value) {}
  unival::unival(signed long value) noexcept
      : m_is_empty(false), m_value(value) {}
  unival::unival(signed long long value) noexcept
      : m_is_empty(false), m_value(value) {}

  unival::unival(unsigned char value) noexcept
      : m_is_empty(false), m_value(value) {}
  unival::unival(unsigned short value) noexcept
      : m_is_empty(false), m_value(value) {}
  unival::unival(unsigned int value) noexcept
      : m_is_empty(false), m_value(value) {}
  unival::unival(unsigned long value) noexcept
      : m_is_empty(false), m_value(static_cast<int64_t>(value)) {}
  unival::unival(unsigned long long value) noexcept
      : m_is_empty(false), m_value(static_cast<int64_t>(value)) {}
  unival::unival(float value) noexcept : m_is_empty(false), m_float(value) {}
  unival::unival(double value) noexcept : m_is_empty(false), m_float(value) {}

  auto unival::is_empty() const noexcept -> bool { return m_is_empty; }

  auto unival::get_integer() const noexcept -> int64_t { return m_value; }

  auto unival::get_uint() const noexcept -> uint64_t {
    return static_cast<uint64_t>(m_value);
  }

  auto unival::get_float() const noexcept -> double { return m_float; }
}
