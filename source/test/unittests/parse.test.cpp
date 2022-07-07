/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/parse.h"
#include "catch2/catch.hpp"

namespace unival::test {
  using namespace std::string_literals;
  using std::u8string, std::min, std::u8string_view, std::equal;

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

  TEST_CASE("parse float") {
    REQUIRE(parse(u8"42.42").real() == 42.42);
    REQUIRE(parse(u8"42.0042").real() == 42.0042);
    REQUIRE(parse(u8"42.").real() == 42.);
    REQUIRE(parse(u8".42").real() == .42);
  }

  TEST_CASE("parse dot or e will fail") {
    REQUIRE(parse(u8".").error());
    REQUIRE(parse(u8".e1").error());
    REQUIRE(parse(u8"1e").error());
  }

  TEST_CASE("parse float with exponent") {
    REQUIRE(parse(u8"42e42").real() == 42e42);
    REQUIRE(parse(u8"42e+2").real() == 42e+2);
    REQUIRE(parse(u8"42e-2").real() == 42e-2);
    REQUIRE(parse(u8".42e-4").real() == .42e-4);
    REQUIRE(parse(u8"4.2e-4").real() == 4.2e-4);
  }

  TEST_CASE("parse string") {
    REQUIRE(parse(u8"\"foo bar\"").string() == u8"foo bar");
  }

  TEST_CASE("parse string with spaces") {
    REQUIRE(parse(u8" \"foo bar\" ").string() == u8"foo bar");
  }

  TEST_CASE("parse string may fail") {
    REQUIRE(parse(u8"\"").error());
  }

  TEST_CASE("parse string escaped") {
    REQUIRE(parse(u8"\"\\\\\\\"\"").string() == u8"\\\"");
  }

  TEST_CASE("parse string escaped with hex") {
    char8_t s[] = { 0x00, 0x42, 0xff, 0x00 };

    REQUIRE(parse(u8R"("\x00\x42\xff")").string() ==
            u8string_view { s, 3 });
  }

  TEST_CASE("parse string escaped with oct") {
    char8_t s[] = { 0, 042, 0377, 0 };

    REQUIRE(parse(u8R"("\0\42\377")").string() ==
            u8string_view { s, 3 });
  }

  TEST_CASE("parse string escaped may fail") {
    REQUIRE(parse(u8"\"\\x00\\x4242\\xff\"").error());
    REQUIRE(parse(u8"\"\\0\\4242\\377\"").error());
  }

  TEST_CASE("parse concatenated string") {
    REQUIRE(parse(u8R"("foo" " bar")").string() == u8"foo bar");
  }

  TEST_CASE("parse empty byte array") {
    REQUIRE(parse(u8"<>") == unival { bytes {} });
  }

  TEST_CASE("parse 3-byte array") {
    REQUIRE(parse(u8"<00 1f ff>") ==
            unival { bytes { 0x00, 0x1f, -1 } });
    REQUIRE(parse(u8"<00 1F FF>") ==
            unival { bytes { 0x00, 0x1f, -1 } });
  }

  TEST_CASE("parse 3-byte array with spaces") {
    REQUIRE(parse(u8" < 00 2a ff > ") ==
            unival { bytes { 0x00, 0x2a, -1 } });
  }

  TEST_CASE("parse empty vector") {
    REQUIRE(parse(u8"[]") == unival { vector {} });
  }

  TEST_CASE("parse 3-vector") {
    REQUIRE(parse(u8"[ 1 2 3 ]") ==
            unival { vector { unival { 1 }, unival { 2 },
                              unival { 3 } } });
    REQUIRE(parse(u8"[1 2 3]") ==
            unival { vector { unival { 1 }, unival { 2 },
                              unival { 3 } } });
    REQUIRE(parse(u8"[ 1, 2, 3 ]") ==
            unival { vector { unival { 1 }, unival { 2 },
                              unival { 3 } } });
    REQUIRE(parse(u8"[ 1; 2; 3 ]") ==
            unival { vector { unival { 1 }, unival { 2 },
                              unival { 3 } } });
    REQUIRE(parse(u8"[ 1, 2, 3, ]") ==
            unival { vector { unival { 1 }, unival { 2 },
                              unival { 3 } } });
    REQUIRE(parse(u8"[ 1; 2; 3; ]") ==
            unival { vector { unival { 1 }, unival { 2 },
                              unival { 3 } } });
  }
}
