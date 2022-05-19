/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "unival.h"

namespace unival {
  template <class... types_> struct overload : types_... {
    using types_::operator()...;
  };

  template <class... types_>
  overload(types_...) -> overload<types_...>;

  template <typename type_>
  chain<type_>::chain(type_ const &value) noexcept
      : m_value(value) { }

  template <typename type_>
  auto chain<type_>::commit() noexcept -> type_ {
    auto val = unival { std::move(m_value) };
    for (auto &op : m_ops)
      _set(val, op.path, op.value);
    return val;
  }

  template <typename type_>
  auto chain<type_>::set(signed long long index,
                         type_ const &value) noexcept
      -> chain<type_> {
    auto result = chain<type_> { std::move(*this) };
    auto path = result.m_cursor;
    path.emplace_back(index);
    result.m_cursor = path_ {};
    result.m_ops.emplace_back(
        op_ { .path = std::move(path), .value = value });
    return result;
  }

  template <typename type_>
  auto chain<type_>::set(type_ const &key,
                         type_ const &value) noexcept
      -> chain<type_> {
    auto result = chain<type_> { std::move(*this) };
    auto path = result.m_cursor;
    path.emplace_back(type_ { key });
    result.m_cursor = path_ {};
    result.m_ops.emplace_back(
        op_ { .path = std::move(path), .value = value });
    return result;
  }

  template <typename type_>
  auto chain<type_>::on(signed long long index) noexcept
      -> chain<type_> {
    auto result = chain<type_> { std::move(*this) };
    result.m_cursor.emplace_back(index);
    return result;
  }

  template <typename type_>
  auto chain<type_>::on(type_ const &key) noexcept -> chain<type_> {
    auto result = chain<type_> { std::move(*this) };
    result.m_cursor.emplace_back(type_ { key });
    return result;
  }

  template <typename type_>
  void chain<type_>::_set(type_ &origin, path_span_ path,
                          type_ const &value) noexcept {
    if (path.size() == 1)
      std::visit(overload { [&](signed long long index) {
                             origin._set(index, value);
                           },
                            [&](type_ const &key) {
                              origin._set(key, value);
                            } },
                 path[0]);
    else
      std::visit(
          overload { [&](signed long long index) {
                      _set(origin._get(index),
                           { path.begin() + 1, path.end() }, value);
                    },
                     [&](type_ const &key) {
                       _set(origin._get(key),
                            { path.begin() + 1, path.end() }, value);
                     } },
          path[0]);
  }

  template class chain<unival>;
}
