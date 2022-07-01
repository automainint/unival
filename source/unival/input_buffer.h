/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_INPUT_BUFFER_H
#define UNIVAL_INPUT_BUFFER_H

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace unival {
  using fn_read = std::function<std::vector<char>(ptrdiff_t)>;

  class input_buffer {
  public:
    explicit input_buffer(fn_read read) noexcept;

    [[nodiscard]] auto eof() const noexcept -> bool;
    [[nodiscard]] auto empty() const noexcept -> bool;

    [[nodiscard]] auto string() const noexcept -> std::string;
    [[nodiscard]] auto u8string() const noexcept -> std::u8string;

    [[nodiscard]] auto read(ptrdiff_t size) const noexcept
        -> input_buffer;

  private:
    void buffered_read(ptrdiff_t size) const noexcept;

    struct buffered {
      fn_read read;
      std::vector<char> data;
    };

    explicit input_buffer(
        std::shared_ptr<buffered> const &buf) noexcept;

    std::vector<char> m_data;
    ptrdiff_t m_offset = 0;
    std::shared_ptr<buffered> m_buffered;
  };
}

#endif
