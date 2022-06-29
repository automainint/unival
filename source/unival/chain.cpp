/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "unival.h"

namespace unival {
  using std::optional, std::string_view, std::u8string_view,
      std::decay_t, std::visit, std::is_same_v;

  template <typename type_>
  chain<type_>::chain(type_ const &value) noexcept
      : m_value(value) { }

  template <typename type_>
  auto chain<type_>::commit() noexcept -> type_ {
    auto val = unival { std::move(m_value) };
    for (auto &op : m_ops)
      if (!visit(
              [&](auto &v) {
                using type = decay_t<decltype(v)>;
                if constexpr (is_same_v<type, set_>)
                  return _set(val, op.path, v.value);
                if constexpr (is_same_v<type, resize_>)
                  return _resize(val, op.path, v.size, v.def);
                if constexpr (is_same_v<type, remove_>)
                  return _remove(val, op.path);
                return false;
              },
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
      return visit(
          [&](auto const &v) {
            using type = decay_t<decltype(v)>;
            if constexpr (is_same_v<type, ptrdiff_t>)
              return origin._set(v, value);
            if constexpr (is_same_v<type, type_>)
              return origin._set(v, value);
            return false;
          },
          path[0]);
    return std::visit(
        [&](auto const &v) {
          using type = decay_t<decltype(v)>;
          if constexpr (is_same_v<type, ptrdiff_t>) {
            if (!origin._check(v))
              return false;
            return _set(origin._get(v),
                        { path.begin() + 1, path.end() }, value);
          }
          if constexpr (is_same_v<type, type_>) {
            if (!origin._check(v))
              return false;
            return _set(origin._get(v),
                        { path.begin() + 1, path.end() }, value);
          }
          return false;
        },
        path[0]);
  }

  template <typename type_>
  auto chain<type_>::_resize(type_ &origin, path_span_ path,
                             ptrdiff_t size,
                             type_ const &def) noexcept -> bool {
    if (path.empty())
      return origin._resize(size, def);
    return visit(
        [&](auto const &v) {
          using type = decay_t<decltype(v)>;
          if constexpr (is_same_v<type, ptrdiff_t>)
            return _resize(origin._get(v),
                           { path.begin() + 1, path.end() }, size,
                           def);
          if constexpr (is_same_v<type, type_>)
            return _resize(origin._get(v),
                           { path.begin() + 1, path.end() }, size,
                           def);
          return false;
        },
        path[0]);
  }

  template <typename type_>
  auto chain<type_>::_remove(type_ &origin, path_span_ path) noexcept
      -> bool {
    if (path.empty())
      return false;
    if (path.size() == 1)
      return visit(
          [&](auto const &v) {
            using type = decay_t<decltype(v)>;
            if constexpr (is_same_v<type, ptrdiff_t>)
              return origin._remove(v);
            if constexpr (is_same_v<type, type_>)
              return origin._remove(v);
            return false;
          },
          path[0]);
    return visit(
        [&](auto const &v) {
          using type = decay_t<decltype(v)>;
          if constexpr (is_same_v<type, ptrdiff_t>) {
            if (!origin._check(v))
              return false;
            return _remove(origin._get(v),
                           { path.begin() + 1, path.end() });
          }
          if constexpr (is_same_v<type, type_>) {
            if (!origin._check(v))
              return false;
            return _remove(origin._get(v),
                           { path.begin() + 1, path.end() });
          }
          return false;
        },
        path[0]);
  }

  template class chain<unival>;
}
