/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_ITERATOR_H
#define UNIVAL_ITERATOR_H

#include <utility>
#include <variant>

namespace unival {
  template <typename type_> class iterator {
  public:
    iterator(type_ const *val, signed long long index) noexcept;
    void operator++() noexcept;
    [[nodiscard]] auto operator*() const noexcept -> type_;
    [[nodiscard]] auto
    operator==(iterator const &other) const noexcept -> bool;

  private:
    type_ const *m_value = nullptr;
    signed long long m_index = 0;
  };
}

#endif
