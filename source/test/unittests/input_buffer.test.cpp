/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/input_buffer.h"
#include "catch2/catch.hpp"
#include <sstream>

namespace unival::test {
  using std::istringstream;

  TEST_CASE("input buffer eof") {
    auto in = istringstream { "" };
    REQUIRE(input_buffer { in }.read(1).eof());
  }

  TEST_CASE("input buffer empty") {
    auto in = istringstream { "" };
    REQUIRE(input_buffer { in }.read(1).empty());
  }

  TEST_CASE("input buffer empty eof") {
    auto in = istringstream { "" };
    REQUIRE(input_buffer { in }.eof());
  }

  TEST_CASE("input buffer not empty") {
    auto in = istringstream { "foo" };
    REQUIRE(!input_buffer { in }.read(3).empty());
  }

  TEST_CASE("input buffer read one") {
    auto in = istringstream { "a" };
    REQUIRE(input_buffer { in }.read(1).string() == "a");
  }

  TEST_CASE("input buffer read one utf-8") {
    auto in = istringstream { "a" };
    REQUIRE(input_buffer { in }.read(1).u8string() == u8"a");
  }

  TEST_CASE("input buffer read two") {
    auto in = istringstream { "foobar" };
    auto buf = input_buffer { in }.read(3);
    REQUIRE(buf.string() == "foo");
    REQUIRE(buf.read(3).string() == "bar");
  }

  TEST_CASE("input buffer read twice") {
    auto in = istringstream { "foo" };
    auto buf = input_buffer { in };
    REQUIRE(buf.read(3).string() == "foo");
    REQUIRE(buf.read(3).string() == "foo");
  }

  TEST_CASE("input buffer read twice and eof") {
    auto in = istringstream { "foo" };
    auto buf = input_buffer { in };
    REQUIRE(buf.read(3).string() == "foo");
    REQUIRE(!buf.eof());
    REQUIRE(buf.read(3).eof());
  }
}
