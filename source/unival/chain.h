/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_CHAIN_H
#define UNIVAL_CHAIN_H

#include <span>
#include <variant>
#include <vector>

namespace unival {
  template <typename type_> class chain {
  public:
    explicit chain(type_ const &value) noexcept;

    auto commit() noexcept -> type_;

    auto set(signed long long index, type_ const &value) noexcept
        -> chain<type_>;

    auto set(type_ const &key, type_ const &value) noexcept
        -> chain<type_>;

    auto on(signed long long index) noexcept -> chain<type_>;

    auto on(type_ const &key) noexcept -> chain<type_>;

  private:
    using path_ = std::vector<std::variant<signed long long, type_>>;

    using path_span_ =
        std::span<std::variant<signed long long, type_> const>;

    static void _set(type_ &origin, path_span_ path,
                     type_ const &value) noexcept;

    struct op_ {
      path_ path;
      type_ value;
    };

    type_ m_value;
    path_ m_cursor;
    std::vector<op_> m_ops;
  };
}

#endif
