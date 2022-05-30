/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_CHAIN_H
#define UNIVAL_CHAIN_H

#include <optional>
#include <span>
#include <variant>
#include <vector>

namespace unival {
  template <typename type_> class chain {
  public:
    explicit chain(type_ const &value) noexcept;

    /*  Apply changes.
     */
    auto commit() noexcept -> type_;

    /*  Set value on the current cursor position and reset the cursor.
     */
    auto set(type_ const &value) noexcept -> chain<type_>;

    /*  Move cursor to vector element by index relative to the current
     *  cursor position.
     */
    auto on(signed long long index) noexcept -> chain<type_>;

    /*  Move cursor to composite element by key relative to the
     *  current cursor position.
     */
    auto on(type_ const &key) noexcept -> chain<type_>;

    /*  Resize vector on the current cursor position and reset the
     *  cursor.
     */
    auto resize(signed long long size,
                type_ const &def = type_ {}) noexcept -> chain<type_>;

    /*  Remove element on the current cursor position and reset the
     *  cursor.
     */
    auto remove() noexcept -> chain<type_>;

  private:
    using path_ = std::vector<std::variant<signed long long, type_>>;

    using path_span_ =
        std::span<std::variant<signed long long, type_> const>;

    static auto _set(type_ &origin, path_span_ path,
                     type_ const &value) noexcept -> bool;

    static auto _resize(type_ &origin, path_span_ path,
                        signed long long size,
                        type_ const &def) noexcept -> bool;

    static auto _remove(type_ &origin, path_span_ path) noexcept
        -> bool;

    struct set_ {
      type_ value;
    };

    struct resize_ {
      signed long long size = 0;
      type_ def;
    };

    struct remove_ { };

    using action_ = std::variant<set_, resize_, remove_>;

    struct op_ {
      path_ path;
      action_ action;
    };

    type_ m_value;
    path_ m_cursor;
    std::vector<op_> m_ops;
  };
}

#endif
