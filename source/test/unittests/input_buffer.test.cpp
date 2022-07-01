/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/input_buffer.h"
#include "catch2/catch.hpp"

namespace unival::test {
  [[nodiscard]] auto wrap_string(char const *s) noexcept -> fn_read {
    return [s, i = ptrdiff_t {}](ptrdiff_t size) mutable {
      auto v = std::vector<char> {};
      v.reserve(size);
      while (s[i] != '\0')
        v.emplace_back(s[i++]);
      return v;
    };
  }

  TEST_CASE("input buffer eof") {
    REQUIRE(input_buffer { wrap_string("") }.read(1).eof());
  }

  TEST_CASE("input buffer empty") {
    REQUIRE(input_buffer { wrap_string("") }.read(1).empty());
  }

  TEST_CASE("input buffer empty eof") {
    REQUIRE(input_buffer { wrap_string("") }.eof());
  }

  TEST_CASE("input buffer not empty") {
    REQUIRE(!input_buffer { wrap_string("foo") }.read(3).empty());
  }

  TEST_CASE("input buffer read one") {
    REQUIRE(input_buffer { wrap_string("a") }.read(1).string() ==
            "a");
  }

  TEST_CASE("input buffer read one utf-8") {
    REQUIRE(input_buffer { wrap_string("a") }.read(1).u8string() ==
            u8"a");
  }

  TEST_CASE("input buffer read two") {
    auto buf = input_buffer { wrap_string("foobar") }.read(3);
    REQUIRE(buf.string() == "foo");
    REQUIRE(buf.read(3).string() == "bar");
  }

  TEST_CASE("input buffer read twice") {
    auto buf = input_buffer { wrap_string("foo") };
    REQUIRE(buf.read(3).string() == "foo");
    REQUIRE(buf.read(3).string() == "foo");
  }

  TEST_CASE("input buffer read twice and eof") {
    auto buf = input_buffer { wrap_string("foo") };
    REQUIRE(buf.read(3).string() == "foo");
    REQUIRE(!buf.eof());
    REQUIRE(buf.read(3).eof());
  }
}
