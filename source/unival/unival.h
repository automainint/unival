/*  Copyright (c) 2022 Mitya Selivanov
 *
 *  This file is part of the Laplace project.
 *
 *  Laplace is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 *  the MIT License for more details.
 */

#ifndef UNIVAL_UNIVAL_H
#define UNIVAL_UNIVAL_H

#include <cstdint>

namespace unival {
  class unival {
  public:
    unival() noexcept;
    explicit unival(signed char value) noexcept;
    explicit unival(signed short value) noexcept;
    explicit unival(signed int value) noexcept;
    explicit unival(signed long value) noexcept;
    explicit unival(signed long long value) noexcept;
    explicit unival(unsigned char value) noexcept;
    explicit unival(unsigned short value) noexcept;
    explicit unival(unsigned int value) noexcept;
    explicit unival(unsigned long value) noexcept;
    explicit unival(unsigned long long value) noexcept;
    explicit unival(float value) noexcept;
    explicit unival(double value) noexcept;

    [[nodiscard]] auto is_empty() const noexcept -> bool;
    [[nodiscard]] auto get_integer() const noexcept -> int64_t;
    [[nodiscard]] auto get_uint() const noexcept -> uint64_t;
    [[nodiscard]] auto get_float() const noexcept -> double;

  private:
    bool m_is_empty = true;
    int64_t m_value = {};
    double m_float = {};
  };
}

#endif
