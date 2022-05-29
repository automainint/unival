/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "unival.h"

namespace unival {
  template <typename type_>
  iterator<type_>::iterator(type_ const *val,
                            signed long long index) noexcept
      : m_value(val), m_index(index) { }

  template <typename type_>
  void iterator<type_>::operator++() noexcept {
    m_index++;
  }

  template <typename type_>
  auto iterator<type_>::operator*() const noexcept -> type_ {
    if (m_value->is_vector())
      return *m_value->get(m_index);
    if (m_value->is_composite())
      return std::get<unival::n_composite>(m_value->m_value)[m_index]
          .first;
    return {};
  }

  template <typename type_>
  auto
  iterator<type_>::operator==(iterator const &other) const noexcept
      -> bool {
    return m_index == other.m_index;
  }

  template class iterator<unival>;
}
