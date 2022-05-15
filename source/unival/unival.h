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
#include <optional>
#include <variant>

namespace unival {
  class unival {
  public:
    unival() noexcept = default;
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
    [[nodiscard]] auto is_integer() const noexcept -> bool;
    [[nodiscard]] auto is_float() const noexcept -> bool;
    
    [[nodiscard]] auto get_integer() const noexcept
        -> std::optional<signed long long>;
    [[nodiscard]] auto get_uint() const noexcept
        -> std::optional<unsigned long long>;
    [[nodiscard]] auto get_float() const noexcept -> std::optional<double>;
    
  private:
    enum index_ : ptrdiff_t { n_empty, n_integer, n_float };

    std::variant<std::monostate, signed long long, double> m_value =
        std::monostate{};
  };
}

#endif
