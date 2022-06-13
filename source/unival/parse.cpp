/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "parse.h"

namespace unival {
  using std::u8string_view, std::u8string, std::min;

  auto parse(fn_read_u8 const &read) noexcept -> unival {
    auto last = read(1);
    while (last == u8" ")
      last = read(1);
    if (last != u8"{")
      return unival::error();
    last = read(1);
    while (last == u8" ")
      last = read(1);
    if (last != u8"}")
      return unival::error();
    last = read(1);
    while (last == u8" ")
      last = read(1);
    if (!last.empty())
      return unival::error();
    return {};
  }

  auto parse(u8string_view in) noexcept -> unival {
    return parse([&, i = ptrdiff_t {}](
                     ptrdiff_t size) mutable -> u8string {
      size = min(size, static_cast<ptrdiff_t>(in.size()) - i);
      auto buf = u8string { in.begin() + i, in.begin() + i + size };
      i += size;
      return buf;
    });
  }
}
