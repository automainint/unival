/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "unival.h"

#include <algorithm>

namespace unival {
  using std::optional, std::nullopt, std::u8string, std::string_view,
      std::u8string_view, std::vector, std::span, std::pair,
      std::lower_bound, std::sort;

  unival::unival(optional<unival> opt) noexcept {
    if (opt.has_value())
      m_value = opt->m_value;
  }

  auto unival::operator=(optional<unival> opt) noexcept -> unival & {
    if (opt.has_value())
      // gcc-11 bug
      // m_value = opt->m_value;
      *this = *opt;
    else
      m_value = std::monostate {};
    return *this;
  }

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
      : m_value(vector<int8_t> { value.begin(), value.end() }) { }

  unival::unival(span<unival const> value) noexcept
      : m_value(vector<unival> { value.begin(), value.end() }) { }

  unival::unival(span<pair<unival, unival> const> value) noexcept
      : m_value(composite_ { value.begin(), value.end() }) {
    auto &comp = std::get<n_composite>(m_value);
    sort(comp.begin(), comp.end(),
         [](auto const &left, auto const &right) {
           return left.first.m_value < right.first.m_value;
         });
    for (auto i = comp.begin(); i != comp.end(); i++) {
      auto j = i;
      while (j != comp.end() && j->first == i->first)
        j++;
      if (j - i > 1)
        comp.erase(i + 1, j);
    }
  }

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

  auto unival::is_empty() const noexcept -> bool {
    return m_value.index() == n_empty;
  }

  auto unival::is_boolean() const noexcept -> bool {
    return m_value.index() == n_boolean;
  }

  auto unival::is_integer() const noexcept -> bool {
    return m_value.index() == n_integer;
  }

  auto unival::is_float() const noexcept -> bool {
    return m_value.index() == n_float;
  }

  auto unival::is_string() const noexcept -> bool {
    return m_value.index() == n_string;
  }

  auto unival::is_bytes() const noexcept -> bool {
    return m_value.index() == n_bytes;
  }

  auto unival::is_vector() const noexcept -> bool {
    return m_value.index() == n_vector;
  }

  auto unival::is_composite() const noexcept -> bool {
    return m_value.index() == n_composite;
  }

  auto unival::get_boolean() const noexcept -> optional<bool> {
    if (!is_boolean())
      return nullopt;
    return std::get<n_boolean>(m_value);
  }

  auto unival::get_integer() const noexcept
      -> optional<signed long long> {
    if (!is_integer())
      return nullopt;
    return std::get<n_integer>(m_value);
  }

  auto unival::get_uint() const noexcept
      -> optional<unsigned long long> {
    if (!is_integer())
      return nullopt;
    return static_cast<unsigned long long>(
        std::get<n_integer>(m_value));
  }

  auto unival::get_float() const noexcept -> optional<double> {
    if (!is_float())
      return nullopt;
    return std::get<n_float>(m_value);
  }

  auto unival::get_string() const noexcept
      -> optional<u8string_view> {
    if (!is_string())
      return nullopt;
    return std::get<n_string>(m_value);
  }

  auto unival::get_bytes() const noexcept
      -> optional<span<int8_t const>> {
    if (!is_bytes())
      return nullopt;
    auto const &v = std::get<n_bytes>(m_value);
    return span<int8_t const> { v.begin(), v.end() };
  }

  auto unival::get_size() const noexcept
      -> optional<signed long long> {
    if (is_vector())
      return std::get<n_vector>(m_value).size();
    if (is_composite())
      return std::get<n_composite>(m_value).size();
    return nullopt;
  }

  auto unival::resize(signed long long size,
                      unival const &def) const noexcept
      -> std::optional<unival> {
    if (!is_vector() || size < 0)
      return nullopt;
    auto val = unival { *this };
    std::get<n_vector>(val.m_value).resize(size, def);
    return val;
  }

  auto unival::get(signed long long index) const noexcept
      -> optional<unival> {
    if (!is_vector())
      return nullopt;
    auto const &v = std::get<n_vector>(m_value);
    if (index < 0 || index >= v.size())
      return nullopt;
    return v[index];
  }

  auto unival::get(unival const &key) const noexcept
      -> optional<unival> {
    if (!is_composite())
      return nullopt;
    auto const &comp = std::get<n_composite>(m_value);
    auto i = lower_bound(comp.begin(), comp.end(), key,
                         [](auto const &value, auto const &key) {
                           return value.first < key;
                         });
    if (i == comp.end() || i->first != key)
      return nullopt;
    return i->second;
  }

  auto unival::set(signed long long index,
                   unival const &value) const noexcept
      -> optional<unival> {
    auto val = unival { *this };
    if (!val._set(index, value))
      return nullopt;
    return val;
  }

  auto unival::set(unival const &key,
                   unival const &value) const noexcept
      -> optional<unival> {
    auto val = unival { *this };
    if (!val._set(key, value))
      return nullopt;
    return val;
  }

  auto unival::edit() const noexcept -> chain<unival> {
    return chain<unival> { *this };
  }

  auto unival::_get(signed long long index) noexcept -> unival & {
    auto &v = std::get<n_vector>(m_value);
    return v[index];
  }

  auto unival::_get(unival const &key) noexcept -> unival & {
    auto &comp = std::get<n_composite>(m_value);
    auto i = lower_bound(comp.begin(), comp.end(), key,
                         [](auto const &value, auto const &key) {
                           return value.first < key;
                         });
    return i->second;
  }

  auto unival::_set(signed long long index,
                    unival const &value) noexcept -> bool {
    if (!is_vector())
      return false;
    if (index < 0)
      return false;
    auto &v = std::get<n_vector>(m_value);
    if (index >= v.size())
      v.resize(index + 1);
    v[index] = value;
    return true;
  }

  auto unival::_set(unival const &key, unival const &value) noexcept
      -> bool {
    if (!is_composite())
      return false;
    auto &comp = std::get<n_composite>(m_value);
    auto i = lower_bound(comp.begin(), comp.end(), key,
                         [](auto const &value, auto const &key) {
                           return value.first < key;
                         });
    if (i != comp.end() && i->first == key)
      i->second = value;
    else
      comp.emplace(i, key, value);
    return true;
  }
}
