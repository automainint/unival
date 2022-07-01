/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "input_buffer.h"

namespace unival {
  using std::istream, std::make_shared, std::shared_ptr, std::min;

  input_buffer::input_buffer(istream &in) noexcept
      : m_buffered(make_shared<buffered>(buffered { &in })) { }

  auto input_buffer::eof() const noexcept -> bool {
    if (m_offset == m_buffered->data.size() || m_buffered->in->eof())
      return true;

    buffered_read(1);
    return m_buffered->in->eof();
  }

  auto input_buffer::empty() const noexcept -> bool {
    return m_data.empty();
  }

  auto input_buffer::string() const noexcept -> std::string {
    return { m_data.data(), m_data.size() };
  }

  auto input_buffer::u8string() const noexcept -> std::u8string {
    return { reinterpret_cast<char8_t const *>(m_data.data()),
             m_data.size() };
  }

  auto input_buffer::read(ptrdiff_t size) const noexcept
      -> input_buffer {
    auto buf = input_buffer { m_buffered };

    buffered_read(size);

    auto const bytes_available = min(
        static_cast<ptrdiff_t>(m_buffered->data.size()) - m_offset,
        size);

    buf.m_data.insert(
        buf.m_data.end(), m_buffered->data.begin() + m_offset,
        m_buffered->data.begin() + m_offset + bytes_available);
    buf.m_offset = m_offset + bytes_available;

    return buf;
  }

  void input_buffer::buffered_read(ptrdiff_t size) const noexcept {
    auto const read_offset =
        static_cast<ptrdiff_t>(m_buffered->data.size());
    auto const bytes_buffered = min(read_offset - m_offset, size);
    auto const bytes_read = size - bytes_buffered;

    m_buffered->data.resize(read_offset + bytes_read);
    m_buffered->in->read(m_buffered->data.data() + read_offset,
                         bytes_read);
    m_buffered->data.resize(read_offset + m_buffered->in->gcount());
  }

  input_buffer::input_buffer(shared_ptr<buffered> const &buf) noexcept
      : m_buffered(buf) { }
}
