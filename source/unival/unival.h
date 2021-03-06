/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_UNIVAL_H
#define UNIVAL_UNIVAL_H

#include "chain.h"
#include "iterator.h"

#include <string>
#include <functional>

namespace unival {
  class unival {
  public:
    friend class chain<unival>;
    friend class iterator<unival>;

    unival() noexcept = default;
    ~unival() noexcept = default;
    unival(unival const &) noexcept = default;
    unival(unival &&) noexcept = default;
    auto operator=(unival const &) noexcept -> unival & = default;
    auto operator=(unival &&) noexcept -> unival & = default;

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

    [[nodiscard]] auto empty() const noexcept -> bool;
    [[nodiscard]] auto error() const noexcept -> bool;
    [[nodiscard]] auto vector() const noexcept -> bool;
    [[nodiscard]] auto composite() const noexcept -> bool;

    [[nodiscard]] auto boolean() const noexcept
        -> std::optional<bool>;
    [[nodiscard]] auto integer() const noexcept
        -> std::optional<signed long long>;
    [[nodiscard]] auto uint() const noexcept
        -> std::optional<unsigned long long>;
    [[nodiscard]] auto real() const noexcept -> std::optional<double>;
    [[nodiscard]] auto string() const noexcept
        -> std::optional<std::u8string_view>;
    [[nodiscard]] auto bytes() const noexcept
        -> std::optional<std::span<int8_t const>>;

    /*  Get vector or composite element count.
     */
    [[nodiscard]] auto size() const noexcept -> ptrdiff_t;

    /*  Resize vector.
     */
    [[nodiscard]] auto resize(ptrdiff_t size) const 
        -> unival;

    /*  Resize vector with default element value.
     */
    [[nodiscard]] auto resize(ptrdiff_t size,
                              unival const &def) const 
        -> unival;

    /*  Get vector element by index.
     */
    [[nodiscard]] auto get(ptrdiff_t index) const noexcept
        -> unival const &;

    /*  Get composite element by key.
     */
    [[nodiscard]] auto get(unival const &key) const noexcept
        -> unival const &;

    /*  Get composite element by integer key.
     */
    [[nodiscard]] auto get_by_key(ptrdiff_t key) const noexcept
        -> unival const &;

    /*  Set vector element by index.
     */
    [[nodiscard]] auto set(ptrdiff_t index,
                           unival const &value) const noexcept
        -> unival;

    /*  Set composite element by key.
     */
    [[nodiscard]] auto set(unival const &key,
                           unival const &value) const noexcept
        -> unival;

    /*  Set composite element by integer key.
     */
    [[nodiscard]] auto set_by_key(ptrdiff_t key,
                                  unival const &value) const noexcept
        -> unival;

    /*  Remove vector element by index.
     */
    [[nodiscard]] auto remove(ptrdiff_t index) const noexcept
        -> unival;

    /*  Remove composite element by key.
     */
    [[nodiscard]] auto remove(unival const &key) const noexcept
        -> unival;

    /*  Remove composite element by integer key.
     */
    [[nodiscard]] auto remove_by_key(ptrdiff_t key) const noexcept
        -> unival;

    /*  Start edit chain.
     *
     *  Available chain methods:
     *    on(index) - move cursor to vector element by index;
     *    on(key) - move cursor to composite element by key;
     *    set(value) - set value on current cursor;
     *    remove() - remove value on current cursor;
     *    resize(size, def) - resize vector on current cursor;
     *    commit() - apply changes.
     *
     *  Usage:
     *  ```
     *  new_val = val.edit()
     *    .set(0, elem1)
     *    .set(1, elem2)
     *    .commit();
     *  ```
     */
    [[nodiscard]] auto edit() const noexcept -> chain<unival>;

    [[nodiscard]] auto begin() const noexcept -> iterator<unival>;
    [[nodiscard]] auto end() const noexcept -> iterator<unival>;

    [[nodiscard]] auto operator[](ptrdiff_t index) const noexcept
        -> unival const &;

    [[nodiscard]] auto operator[](unival const &key) const noexcept
        -> unival const &;

    [[nodiscard]] auto operator[](std::string_view key) const noexcept
        -> unival const &;

    [[nodiscard]] auto
    operator[](std::u8string_view key) const noexcept
        -> unival const &;

    [[nodiscard]] static auto _error() noexcept -> unival;

  private:
    [[nodiscard]] static auto _error_ptr() noexcept -> unival const *;

    [[nodiscard]] auto _check(ptrdiff_t index) const noexcept -> bool;

    [[nodiscard]] auto _check(unival const &key) const noexcept
        -> bool;

    [[nodiscard]] auto _get(ptrdiff_t index) noexcept -> unival &;

    [[nodiscard]] auto _get(unival const &key) noexcept -> unival &;

    [[nodiscard]] auto _set(ptrdiff_t index,
                            unival const &value) noexcept -> bool;

    [[nodiscard]] auto _set(unival const &key,
                            unival const &value) noexcept -> bool;

    [[nodiscard]] auto _resize(ptrdiff_t size,
                               unival const &def)  -> bool;

    [[nodiscard]] auto _remove(ptrdiff_t index) noexcept -> bool;

    [[nodiscard]] auto _remove(unival const &key) noexcept -> bool;

    struct error_ {
      auto operator<=>(error_ const &) const noexcept = default;
    };

    using bytes_ = std::vector<int8_t>;
    using vector_ = std::vector<unival>;
    using composite_ = std::vector<std::pair<unival, unival>>;

    using value_type_ =
        std::variant<std::monostate, error_, bool, signed long long,
                     double, std::u8string, bytes_, vector_,
                     composite_>;

    value_type_ m_value;
  };

  using bytes = std::vector<int8_t>;
  using vector = std::vector<unival>;
  using composite = std::vector<std::pair<unival, unival>>;
}

#endif
