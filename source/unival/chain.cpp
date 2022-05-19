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
      std::visit(
          overload { [&](set_ &v) { _set(val, op.path, v.value); },
                     [&](resize_ &v) {
                       _resize(val, op.path, v.size, v.def);
                     } },
          op.action);
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
    result.m_ops.emplace_back(op_ {
        .path = std::move(path), .action = set_ { .value = value } });
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
    result.m_ops.emplace_back(op_ {
        .path = std::move(path), .action = set_ { .value = value } });
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
  auto chain<type_>::resize(signed long long size,
                            type_ const &def) noexcept
      -> chain<type_> {
    auto result = chain<type_> { std::move(*this) };
    auto path = result.m_cursor;
    result.m_cursor = path_ {};
    result.m_ops.emplace_back(
        op_ { .path = std::move(path),
              .action = resize_ { .size = size, .def = def } });
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

  template <typename type_>
  void chain<type_>::_resize(type_ &origin, path_span_ path,
                             signed long long size,
                             type_ const &def) noexcept {
    if (path.empty())
      origin._resize(size, def);
    else
      std::visit(overload { [&](signed long long index) {
                             _resize(origin._get(index),
                                     { path.begin() + 1, path.end() },
                                     size, def);
                           },
                            [&](type_ const &key) {
                              _resize(
                                  origin._get(key),
                                  { path.begin() + 1, path.end() },
                                  size, def);
                            } },
                 path[0]);
  }

  template class chain<unival>;
}
