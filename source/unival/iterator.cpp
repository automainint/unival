/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "unival.h"

namespace unival {
  template <typename type_>
  iterator<type_>::iterator(type_ const &val,
                            signed long long index) noexcept
      : m_value(&val), m_index(index < 0 ? _size(val) : index) { }

  template <typename type_>
  void iterator<type_>::operator++() noexcept {
    m_index++;
  }

  template <typename type_>
  auto iterator<type_>::operator->() const noexcept -> type_ const * {
    return std::get<unival::n_vector>(m_value->m_value).data() +
           m_index;
  }

  template <typename type_>
  auto iterator<type_>::operator*() const noexcept -> type_ {
    if (m_index >= _size(*m_value))
      return {};
    if (m_value->is_vector())
      return *m_value->get(m_index);
    return std::get<unival::n_composite>(m_value->m_value)[m_index]
        .first;
  }

  template <typename type_>
  auto
  iterator<type_>::operator==(iterator const &other) const noexcept
      -> bool {
    return m_index == other.m_index;
  }

  template <typename type_>
  auto iterator<type_>::_size(type_ const &val) noexcept
      -> signed long long {
    auto size = val.get_size();
    return size.has_value() ? *size : 0;
  }

  template class iterator<unival>;
}
