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
#include <span>
#include <string>
#include <variant>
#include <vector>

namespace unival {
  class unival {
  public:
    unival() noexcept = default;
    explicit unival(std::optional<unival> opt) noexcept;
    auto operator=(std::optional<unival> opt) noexcept -> unival &;

    explicit unival(bool value) noexcept;
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
    explicit unival(char const *value) noexcept;
    explicit unival(char8_t const *value) noexcept;
    explicit unival(std::string_view value) noexcept;
    explicit unival(std::u8string_view value) noexcept;

    /*  Create byte array.
     */
    explicit unival(std::span<int8_t const> value) noexcept;

    /*  Create vector.
     */
    explicit unival(std::span<unival const> value) noexcept;

    /*  Create composite.
     */
    explicit unival(
        std::span<std::pair<unival, unival> const> value) noexcept;

    [[nodiscard]] auto operator==(unival const &val) const noexcept
        -> bool;
    [[nodiscard]] auto operator<(unival const &val) const noexcept
        -> bool;
    [[nodiscard]] auto operator<=(unival const &val) const noexcept
        -> bool;
    [[nodiscard]] auto operator>=(unival const &val) const noexcept
        -> bool;
    [[nodiscard]] auto operator>(unival const &val) const noexcept
        -> bool;

    [[nodiscard]] auto is_empty() const noexcept -> bool;
    [[nodiscard]] auto is_boolean() const noexcept -> bool;
    [[nodiscard]] auto is_integer() const noexcept -> bool;
    [[nodiscard]] auto is_float() const noexcept -> bool;
    [[nodiscard]] auto is_string() const noexcept -> bool;
    [[nodiscard]] auto is_bytes() const noexcept -> bool;
    [[nodiscard]] auto is_vector() const noexcept -> bool;
    [[nodiscard]] auto is_composite() const noexcept -> bool;

    [[nodiscard]] auto get_boolean() const noexcept
        -> std::optional<bool>;
    [[nodiscard]] auto get_integer() const noexcept
        -> std::optional<signed long long>;
    [[nodiscard]] auto get_uint() const noexcept
        -> std::optional<unsigned long long>;
    [[nodiscard]] auto get_float() const noexcept
        -> std::optional<double>;
    [[nodiscard]] auto get_string() const noexcept
        -> std::optional<std::u8string_view>;
    [[nodiscard]] auto get_bytes() const noexcept
        -> std::optional<std::span<int8_t const>>;

    /*  Get vector or composite element count.
     */
    [[nodiscard]] auto get_size() const noexcept
        -> std::optional<signed long long>;

    /*  Get vector element by index.
     */
    [[nodiscard]] auto get(signed long long index) const noexcept
        -> std::optional<unival>;

    /*  Get composite element by key.
     */
    [[nodiscard]] auto get(unival const &key) const noexcept
        -> std::optional<unival>;

    /*  Set vector element by index.
     */
    auto set(signed long long index,
             unival const &value) const noexcept
        -> std::optional<unival>;

    /*  Set composite element by key.
     */
    auto set(unival const &key, unival const &value) const noexcept
        -> std::optional<unival>;

  private:
    enum index_ : ptrdiff_t {
      n_empty,
      n_boolean,
      n_integer,
      n_float,
      n_string,
      n_bytes,
      n_vector,
      n_composite
    };

    using composite_ = std::vector<std::pair<unival, unival>>;

    std::variant<std::monostate, bool, signed long long, double,
                 std::u8string, std::vector<int8_t>,
                 std::vector<unival>, composite_>
        m_value = std::monostate {};
  };
}

#endif