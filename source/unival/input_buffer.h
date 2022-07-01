/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_INPUT_BUFFER_H
#define UNIVAL_INPUT_BUFFER_H

#include "read.h"
#include <memory>
#include <string>

namespace unival {
  class input_buffer {
  public:
    input_buffer() noexcept = default;
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

    std::shared_ptr<buffered> m_buffered =
        std::make_shared<buffered>(buffered {
            [](ptrdiff_t) -> std::vector<char> { return {}; } });
  };
}

#endif
