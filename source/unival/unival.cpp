/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "unival.h"

#include <algorithm>

namespace unival {
  using std::optional, std::nullopt, std::u8string, std::string_view,
      std::u8string_view, std::span, std::pair, std::lower_bound,
      std::sort, std::visit, std::decay_t, std::is_same_v,
      std::monostate;

  unival::unival(bool value) noexcept : m_value(value) { }

  unival::unival(signed char value) noexcept
      : m_value(static_cast<long long int>(value)) { }
  unival::unival(signed short value) noexcept
      : m_value(static_cast<long long int>(value)) { }
  unival::unival(signed int value) noexcept
      : m_value(static_cast<long long int>(value)) { }
  unival::unival(signed long value) noexcept
      : m_value(static_cast<long long int>(value)) { }
  unival::unival(signed long long value) noexcept
      : m_value(static_cast<long long int>(value)) { }

  unival::unival(unsigned char value) noexcept
      : m_value(static_cast<long long int>(value)) { }
  unival::unival(unsigned short value) noexcept
      : m_value(static_cast<long long int>(value)) { }
  unival::unival(unsigned int value) noexcept
      : m_value(static_cast<long long int>(value)) { }
  unival::unival(unsigned long value) noexcept
      : m_value(static_cast<long long int>(value)) { }
  unival::unival(unsigned long long value) noexcept
      : m_value(static_cast<long long int>(value)) { }

  unival::unival(float value) noexcept
      : m_value(static_cast<double>(value)) { }
  unival::unival(double value) noexcept : m_value(value) { }

  unival::unival(char const *value) noexcept
      : m_value(
            u8string { reinterpret_cast<char8_t const *>(value) }) { }
  unival::unival(char8_t const *value) noexcept
      : m_value(u8string { value }) { }

  unival::unival(std::string_view value) noexcept
      : m_value(u8string {
            reinterpret_cast<char8_t const *>(value.data()),
            value.size() }) { }

  unival::unival(std::u8string_view value) noexcept
      : m_value(u8string { value }) { }

  unival::unival(span<int8_t const> value) noexcept
      : m_value(bytes_ { value.begin(), value.end() }) { }

  unival::unival(span<unival const> value) noexcept
      : m_value(vector_ { value.begin(), value.end() }) { }

  unival::unival(span<pair<unival, unival> const> value) noexcept
      : m_value([](span<pair<unival, unival> const> value) {
          auto comp = composite_ { value.begin(), value.end() };
          sort(comp.begin(), comp.end(),
               [](auto const &left, auto const &right) {
                 return left.first.m_value < right.first.m_value;
               });
          for (auto i = comp.begin(); i != comp.end(); i++) {
            auto j = i + 1;
            while (j != comp.end() && j->first == i->first)
              j++;
            if (j - i > 1)
              comp.erase(i + 1, j);
          }
          return comp;
        }(value)) { }

  auto unival::operator==(unival const &val) const noexcept -> bool {
    return m_value == val.m_value;
  }

  auto unival::operator<(unival const &val) const noexcept -> bool {
    return m_value < val.m_value;
  }

  auto unival::operator<=(unival const &val) const noexcept -> bool {
    return m_value <= val.m_value;
  }

  auto unival::operator>(unival const &val) const noexcept -> bool {
    return m_value > val.m_value;
  }

  auto unival::operator>=(unival const &val) const noexcept -> bool {
    return m_value >= val.m_value;
  }

  auto unival::empty() const noexcept -> bool {
    return visit(
        [](auto const &value) {
          using type = decay_t<decltype(value)>;
          return is_same_v<type, monostate>;
        },
        m_value);
  }

  auto unival::error() const noexcept -> bool {
    return visit(
        [](auto const &value) {
          using type = decay_t<decltype(value)>;
          return is_same_v<type, error_>;
        },
        m_value);
  }

  auto unival::vector() const noexcept -> bool {
    return visit(
        [](auto const &value) {
          using type = decay_t<decltype(value)>;
          return is_same_v<type, vector_>;
        },
        m_value);
  }

  auto unival::composite() const noexcept -> bool {
    return visit(
        [](auto const &value) {
          using type = decay_t<decltype(value)>;
          return is_same_v<type, composite_>;
        },
        m_value);
  }

  auto unival::boolean() const noexcept -> optional<bool> {
    return visit(
        [](auto const &value) -> optional<bool> {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, bool>)
            return value;
          return nullopt;
        },
        m_value);
  }

  auto unival::integer() const noexcept
      -> optional<signed long long> {
    return visit(
        [](auto const &value) -> optional<signed long long> {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, signed long long>)
            return value;
          return nullopt;
        },
        m_value);
  }

  auto unival::uint() const noexcept -> optional<unsigned long long> {
    return visit(
        [](auto const &value) -> optional<unsigned long long> {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, signed long long>)
            return static_cast<unsigned long long>(value);
          return nullopt;
        },
        m_value);
  }

  auto unival::real() const noexcept -> optional<double> {
    return visit(
        [](auto const &value) -> optional<double> {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, double>)
            return value;
          return nullopt;
        },
        m_value);
  }

  auto unival::string() const noexcept -> optional<u8string_view> {
    return visit(
        [](auto const &value) -> optional<u8string_view> {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, u8string>)
            return u8string_view { value };
          return nullopt;
        },
        m_value);
  }

  auto unival::bytes() const noexcept
      -> optional<span<int8_t const>> {
    return visit(
        [](auto const &value) -> optional<span<int8_t const>> {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, bytes_>)
            return span<int8_t const> { value.begin(), value.end() };
          return nullopt;
        },
        m_value);
  }

  auto unival::size() const noexcept -> ptrdiff_t {
    return visit(
        [](auto const &value) -> ptrdiff_t {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, vector_>)
            return static_cast<ptrdiff_t>(value.size());
          if constexpr (is_same_v<type, composite_>)
            return static_cast<ptrdiff_t>(value.size());
          return 0;
        },
        m_value);
  }

  auto unival::resize(ptrdiff_t size) const -> unival {
    return resize(size, unival {});
  }

  auto unival::resize(ptrdiff_t size, unival const &def) const
      -> unival {
    return visit(
        [&](auto const &vec) -> unival {
          using type = decay_t<decltype(vec)>;
          if constexpr (is_same_v<type, vector_>) {
            if (size < 0)
              return _error();
            auto v = vector_ { vec };
            v.resize(size, def);
            return unival { v };
          }
          if constexpr (is_same_v<type, monostate>)
            return unival { vector_ { static_cast<size_t>(size),
                                      def } };
          return _error();
        },
        m_value);
  }

  auto unival::get(ptrdiff_t index) const noexcept -> unival const & {
    return visit(
        [&](auto const &value) -> unival const & {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, vector_>) {
            if (index < 0 || index >= value.size())
              return *_error_ptr();
            return value[index];
          }
          if constexpr (is_same_v<type, composite_>) {
            if (index < 0 || index >= value.size())
              return *_error_ptr();
            return value[index].first;
          }
          return *_error_ptr();
        },
        m_value);
  }

  auto unival::get(unival const &key) const noexcept
      -> unival const & {
    return visit(
        [&](auto const &comp) -> unival const & {
          using type = decay_t<decltype(comp)>;
          if constexpr (is_same_v<type, composite_>) {
            auto i =
                lower_bound(comp.begin(), comp.end(), key,
                            [](auto const &value, auto const &key) {
                              return value.first < key;
                            });
            if (i == comp.end() || i->first != key)
              return *_error_ptr();
            return i->second;
          }
          return *_error_ptr();
        },
        m_value);
  }

  auto unival::get_by_key(ptrdiff_t key) const noexcept
      -> unival const & {
    return get(unival { key });
  }

  auto unival::set(ptrdiff_t index,
                   unival const &value) const noexcept -> unival {
    auto val = unival { *this };
    if (!val._set(index, value))
      return _error();
    return val;
  }

  auto unival::set(unival const &key,
                   unival const &value) const noexcept -> unival {
    auto val = unival { *this };
    if (!val._set(key, value))
      return _error();
    return val;
  }

  auto unival::set_by_key(ptrdiff_t key,
                          unival const &value) const noexcept
      -> unival {
    return set(unival { key }, value);
  }

  auto unival::remove(ptrdiff_t index) const noexcept -> unival {
    auto val = unival { *this };
    if (!val._remove(index))
      return _error();
    return val;
  }

  auto unival::remove(unival const &key) const noexcept -> unival {
    auto val = unival { *this };
    if (!val._remove(key))
      return _error();
    return val;
  }

  auto unival::remove_by_key(ptrdiff_t key) const noexcept -> unival {
    return remove(unival { key });
  }

  auto unival::edit() const noexcept -> chain<unival> {
    return chain<unival> { *this };
  }

  auto unival::begin() const noexcept -> iterator<unival> {
    return iterator<unival> { *this, 0 };
  }

  auto unival::end() const noexcept -> iterator<unival> {
    return iterator<unival> { *this, size() };
  }

  auto unival::operator[](ptrdiff_t index) const noexcept
      -> unival const & {
    return get(index);
  }

  auto unival::operator[](unival const &key) const noexcept
      -> unival const & {
    return get(key);
  }

  auto unival::operator[](string_view key) const noexcept
      -> unival const & {
    return get(unival { key });
  }

  auto unival::operator[](u8string_view key) const noexcept
      -> unival const & {
    return get(unival { key });
  }

  auto unival::_error() noexcept -> unival {
    auto val = unival {};
    val.m_value = error_ {};
    return val;
  }

  auto unival::_error_ptr() noexcept -> unival const * {
    static auto const val = _error();
    return &val;
  }

  auto unival::_check(ptrdiff_t index) const noexcept -> bool {
    return visit(
        [&](auto const &value) -> bool {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, vector_>)
            return index >= 0 && index < value.size();
          return false;
        },
        m_value);
  }

  auto unival::_check(unival const &key) const noexcept -> bool {
    return visit(
        [&](auto const &comp) -> bool {
          using type = decay_t<decltype(comp)>;
          if constexpr (is_same_v<type, composite_>) {
            auto i =
                lower_bound(comp.begin(), comp.end(), key,
                            [](auto const &value, auto const &key) {
                              return value.first < key;
                            });
            return i != comp.end() && i->first == key;
          }
          return false;
        },
        m_value);
  }

  auto unival::_get(ptrdiff_t index) noexcept -> unival & {
    return visit(
        [&](auto &value) -> unival & {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, vector_>)
            return value[index];
          /*  unreachable */
          return *static_cast<unival *>(nullptr);
        },
        m_value);
  }

  auto unival::_get(unival const &key) noexcept -> unival & {
    return visit(
        [&](auto &value) -> unival & {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, composite_>) {
            auto i =
                lower_bound(value.begin(), value.end(), key,
                            [](auto const &value, auto const &key) {
                              return value.first < key;
                            });
            return i->second;
          }
          /*  unreachable */
          return *static_cast<unival *>(nullptr);
        },
        m_value);
  }

  auto unival::_set(ptrdiff_t index, unival const &value) noexcept
      -> bool {
    return visit(
        [&](auto &vec) -> bool {
          using type = decay_t<decltype(vec)>;
          if constexpr (is_same_v<type, vector_>) {
            if (index < 0 || index >= vec.size())
              return false;
            vec[index] = value;
            return true;
          }
          return false;
        },
        m_value);
  }

  auto unival::_set(unival const &key, unival const &value) noexcept
      -> bool {
    return visit(
        [&](auto &comp) -> bool {
          using type = decay_t<decltype(comp)>;
          if constexpr (is_same_v<type, composite_>) {
            auto i =
                lower_bound(comp.begin(), comp.end(), key,
                            [](auto const &value, auto const &key) {
                              return value.first < key;
                            });
            if (i != comp.end() && i->first == key)
              i->second = value;
            else
              comp.emplace(i, key, value);
            return true;
          }
          if constexpr (is_same_v<type, monostate>) {
            m_value = composite_ { { key, value } };
            return true;
          }
          return false;
        },
        m_value);
  }

  auto unival::_resize(ptrdiff_t size, unival const &def) -> bool {
    return visit(
        [&](auto &value) -> bool {
          using type = decay_t<decltype(value)>;
          if constexpr (is_same_v<type, vector_>) {
            if (size < 0)
              return false;
            value.resize(size, def);
            return true;
          }
          if constexpr (is_same_v<type, monostate>) {
            if (size < 0)
              return false;
            m_value = vector_ { static_cast<size_t>(size), def };
            return true;
          }
          return false;
        },
        m_value);
  }

  auto unival::_remove(ptrdiff_t index) noexcept -> bool {
    return visit(
        [&](auto &vec) -> bool {
          using type = decay_t<decltype(vec)>;
          if constexpr (is_same_v<type, vector_>) {
            if (index < 0 || index >= vec.size())
              return false;
            vec.erase(vec.begin() + index);
            return true;
          }
          return false;
        },
        m_value);
  }

  auto unival::_remove(unival const &key) noexcept -> bool {
    return visit(
        [&](auto &comp) -> bool {
          using type = decay_t<decltype(comp)>;
          if constexpr (is_same_v<type, composite_>) {
            auto i =
                lower_bound(comp.begin(), comp.end(), key,
                            [](auto const &value, auto const &key) {
                              return value.first < key;
                            });
            if (i == comp.end() || i->first != key)
              return false;
            comp.erase(i);
            return true;
          }
          return false;
        },
        m_value);
  }
}
