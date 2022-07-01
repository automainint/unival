/*  Copyright (c) 2022 Mitya Selivanov
 */

#ifndef UNIVAL_READ_H
#define UNIVAL_READ_H

#include <cstddef>
#include <functional>
#include <vector>

namespace unival {
  using fn_read = std::function<std::vector<char>(ptrdiff_t)>;
}

#endif
