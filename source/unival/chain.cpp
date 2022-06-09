/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "unival.h"

namespace unival {
  using std::optional, std::nullopt, std::string_view,
      std::u8string_view;

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
      if (!std::visit(overload { [&](set_ &v) {
                                  return _set(val, op.path, v.value);
                                },
                                 [&](resize_ &v) {
                                   return _resize(val, op.path,
                                                  v.size, v.def);
                                 },
                                 [&](remove_ &v) {
                                   return _remove(val, op.path);
                                 } },
                      op.action))
        return unival::_error();
    return val;
  }

  template <typename type_>
  auto chain<type_>::set(type_ const &value) noexcept
      -> chain<type_> {
    auto result = chain<type_> { std::move(*this) };
    auto path = result.m_cursor;
    result.m_cursor = path_ {};
    result.m_ops.emplace_back(op_ {
        .path = std::move(path), .action = set_ { .value = value } });
    return result;
  }

  template <typename type_>
  auto chain<type_>::on(ptrdiff_t index) noexcept -> chain<type_> {
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
  auto chain<type_>::on(string_view key) noexcept -> chain<type_> {
    return on(type_ { key });
  }

  template <typename type_>
  auto chain<type_>::on(u8string_view key) noexcept -> chain<type_> {
    return on(type_ { key });
  }

  template <typename type_>
  auto chain<type_>::on_key(ptrdiff_t key) noexcept -> chain<type_> {
    return on(type_ { key });
  }

  template <typename type_>
  auto chain<type_>::resize(ptrdiff_t size, type_ const &def) noexcept
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
  auto chain<type_>::remove() noexcept -> chain<type_> {
    auto result = chain<type_> { std::move(*this) };
    auto path = result.m_cursor;
    result.m_cursor = path_ {};
    result.m_ops.emplace_back(
        op_ { .path = std::move(path), .action = remove_ {} });
    return result;
  }

  template <typename type_>
  auto chain<type_>::_set(type_ &origin, path_span_ path,
                          type_ const &value) noexcept -> bool {
    if (path.empty()) {
      origin = type_ { value };
      return true;
    }
    if (path.size() == 1)
      return std::visit(overload { [&](signed long long index) {
                                    return origin._set(index, value);
                                  },
                                   [&](type_ const &key) {
                                     return origin._set(key, value);
                                   } },
                        path[0]);
    return std::visit(
        overload { [&](signed long long index) {
                    if (!origin._check(index))
                      return false;
                    return _set(origin._get(index),
                                { path.begin() + 1, path.end() },
                                value);
                  },
                   [&](type_ const &key) {
                     if (!origin._check(key))
                       return false;
                     return _set(origin._get(key),
                                 { path.begin() + 1, path.end() },
                                 value);
                   } },
        path[0]);
  }

  template <typename type_>
  auto chain<type_>::_resize(type_ &origin, path_span_ path,
                             ptrdiff_t size,
                             type_ const &def) noexcept -> bool {
    if (path.empty())
      return origin._resize(size, def);
    return std::visit(
        overload { [&](signed long long index) {
                    return _resize(origin._get(index),
                                   { path.begin() + 1, path.end() },
                                   size, def);
                  },
                   [&](type_ const &key) {
                     return _resize(origin._get(key),
                                    { path.begin() + 1, path.end() },
                                    size, def);
                   } },
        path[0]);
  }

  template <typename type_>
  auto chain<type_>::_remove(type_ &origin, path_span_ path) noexcept
      -> bool {
    if (path.empty())
      return false;
    if (path.size() == 1)
      return std::visit(overload { [&](signed long long index) {
                                    return origin._remove(index);
                                  },
                                   [&](type_ const &key) {
                                     return origin._remove(key);
                                   } },
                        path[0]);
    return std::visit(
        overload { [&](signed long long index) {
                    if (!origin._check(index))
                      return false;
                    return _remove(origin._get(index),
                                   { path.begin() + 1, path.end() });
                  },
                   [&](type_ const &key) {
                     if (!origin._check(key))
                       return false;
                     return _remove(origin._get(key),
                                    { path.begin() + 1, path.end() });
                   } },
        path[0]);
  }

  template class chain<unival>;
}
