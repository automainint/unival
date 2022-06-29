/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "unival.h"

namespace unival {
  using std::visit, std::decay_t, std::is_same_v;

  template <typename type_>
  iterator<type_>::iterator(type_ const &val,
                            ptrdiff_t index) noexcept
      : m_value(&val), m_index(index < 0 ? val.size() : index) { }

  template <typename type_>
  void iterator<type_>::operator++() noexcept {
    m_index++;
  }

  template <typename type_>
  auto iterator<type_>::operator->() const noexcept -> type_ const * {
    return _get_ptr();
  }

  template <typename type_>
  auto iterator<type_>::operator*() const noexcept -> type_ const & {
    return *_get_ptr();
  }

  template <typename type_>
  auto
  iterator<type_>::operator==(iterator const &other) const noexcept
      -> bool {
    return m_index == other.m_index;
  }

  template <typename type_>
  auto iterator<type_>::_get_ptr() const noexcept -> type_ const * {
    return visit(
        [&](auto const &v) -> type_ const * {
          using type = decay_t<decltype(v)>;
          if constexpr (is_same_v<type, typename type_::vector_>) {
            if (m_index >= v.size())
              return type_::_error_ptr();
            return &v[m_index];
          }
          if constexpr (is_same_v<type, typename type_::composite_>) {
            if (m_index >= v.size())
              return type_::_error_ptr();
            return &v[m_index].first;
          }
          return type_::_error_ptr();
        },
        m_value->m_value);
  }

  template class iterator<unival>;
}
