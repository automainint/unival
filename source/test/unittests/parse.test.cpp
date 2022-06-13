/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/parse.h"
#include "catch2/catch.hpp"

#include <iostream>

namespace unival::test {
  using namespace std::string_literals;
  using std::u8string, std::min;

  TEST_CASE("Parse empty unival") {
    REQUIRE(parse([str = u8"{}"s, i = ptrdiff_t {}](
                      ptrdiff_t size) mutable -> std::u8string {
              size = min<ptrdiff_t>(size, str.size() - i);
              auto buf = std::u8string { str.begin() + i,
                                         str.begin() + i + size };
              i += size;
              return buf;
            }).is_empty());
  }

  TEST_CASE("Parse invalid input") {
    REQUIRE(parse([str = u8""s, i = ptrdiff_t {}](
                      ptrdiff_t size) mutable -> std::u8string {
              size = min<ptrdiff_t>(size, str.size() - i);
              auto buf = std::u8string { str.begin() + i,
                                         str.begin() + i + size };
              i += size;
              return buf;
            }).is_error());
  }

  TEST_CASE("Parse empty unival from string") {
    REQUIRE(parse(u8"{}").is_empty());
  }

  TEST_CASE("Parse invalid unival from string") {
    REQUIRE(parse(u8"").is_error());
    REQUIRE(parse(u8"{} foo").is_error());
  }

  TEST_CASE("Parse empty unival from string with spaces") {
    REQUIRE(parse(u8" { } ").is_empty());
  }
}
