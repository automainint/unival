/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/parse.h"
#include "catch2/catch.hpp"

namespace unival::test {
  using namespace std::string_literals;
  using std::u8string, std::min;

  TEST_CASE("parse empty unival") {
    REQUIRE(parse([str = u8"{}"s, i = ptrdiff_t {}](
                      ptrdiff_t size) mutable -> std::vector<char> {
              size = min<ptrdiff_t>(size, str.size() - i);
              auto buf = std::vector<char> { str.begin() + i,
                                             str.begin() + i + size };
              i += size;
              return buf;
            }).empty());
  }

  TEST_CASE("parse invalid input") {
    REQUIRE(parse([str = u8""s, i = ptrdiff_t {}](
                      ptrdiff_t size) mutable -> std::vector<char> {
              size = min<ptrdiff_t>(size, str.size() - i);
              auto buf = std::vector<char> { str.begin() + i,
                                             str.begin() + i + size };
              i += size;
              return buf;
            }).error());
  }

  TEST_CASE("parse empty unival from string") {
    REQUIRE(parse(u8"{}").empty());
  }

  TEST_CASE("parse invalid unival from string") {
    REQUIRE(parse(u8"").error());
    REQUIRE(parse(u8"{} foo").error());
  }

  TEST_CASE("parse empty unival from string with spaces") {
    REQUIRE(parse(u8" { } ").empty());
    REQUIRE(parse(u8"\n{  \t}\n").empty());
    REQUIRE(parse(u8"\r\n{  \t}\r\n").empty());
  }

  TEST_CASE("parse empty unival from string with comment") {
    REQUIRE(parse(u8"{//\n}").empty());
    REQUIRE(parse(u8"{//\r\n}").empty());
    REQUIRE(parse(u8"{/* foo bar */}").empty());
  }

  TEST_CASE("parse null") { REQUIRE(parse(u8"null").empty()); }

  TEST_CASE("parse null with spaces") {
    REQUIRE(parse(u8" null ").empty());
  }

  TEST_CASE("parse true") {
    REQUIRE(parse(u8"true").boolean() == true);
  }

  TEST_CASE("parse true with spaces") {
    REQUIRE(parse(u8" true ").boolean() == true);
  }

  TEST_CASE("parse false") {
    REQUIRE(parse(u8"false").boolean() == false);
  }

  TEST_CASE("parse false with spaces") {
    REQUIRE(parse(u8" false ").boolean() == false);
  }

  TEST_CASE("parse integer") {
    REQUIRE(parse(u8"42").integer() == 42);
  }

  TEST_CASE("parse positive integer") {
    REQUIRE(parse(u8"+42").integer() == 42);
  }

  TEST_CASE("parse negative integer") {
    REQUIRE(parse(u8"-42").integer() == -42);
  }

  TEST_CASE("parse large integer") {
    REQUIRE(parse(u8"4242424242424242").integer() ==
            4242424242424242);
  }

  TEST_CASE("parse integer with spaces") {
    REQUIRE(parse(u8" 42 ").integer() == 42);
  }

  TEST_CASE("parse bin integer") {
    REQUIRE(parse(u8"0b1010").integer() == 10);
    REQUIRE(parse(u8"0B1010").integer() == 10);
  }

  TEST_CASE("parse oct integer") {
    REQUIRE(parse(u8"0o7070").integer() == 07070);
    REQUIRE(parse(u8"0O7070").integer() == 07070);
  }

  TEST_CASE("parse hex integer") {
    REQUIRE(parse(u8"0x1a1f").integer() == 0x1a1f);
    REQUIRE(parse(u8"0X1A1F").integer() == 0x1a1f);
  }

  TEST_CASE("parse negative hex integer") {
    REQUIRE(parse(u8"-0x1a1f").integer() == -0x1a1f);
    REQUIRE(parse(u8"-0X1A1F").integer() == -0x1a1f);
  }
}
