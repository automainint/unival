/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/print.h"
#include "catch2/catch.hpp"

#include <iostream>

namespace unival::test {
  using std::u8string, std::u8string_view;

  TEST_CASE("Print int unival") {
    auto val = unival { 42 };
    auto str = u8string {};
    auto put = [&](u8string_view s) -> ptrdiff_t {
      str.append(s.begin(), s.end());
      return static_cast<ptrdiff_t>(s.size());
    };

    REQUIRE(print(val, put));
    REQUIRE(str == u8"42");
  }

  TEST_CASE("Print can fail if callback fails") {
    REQUIRE(!print(unival { 42 },
                   [](u8string_view) -> bool { return false; }));
  }

  TEST_CASE("Print can fail if value is invalid") {
    auto put = [](u8string_view) -> bool { return true; };

    REQUIRE(!print(unival { 42 }.set(0, unival { 0 }), put));
  }

  TEST_CASE("Print into string") {
    REQUIRE(to_string(unival { 42 }) == u8"42");
  }

  TEST_CASE("Print to string can fail") {
    REQUIRE(
        !to_string(unival { 42 }.set(0, unival { 0 })).has_value());
  }

  TEST_CASE("Print empty unival") {
    REQUIRE(to_string(unival {}) == u8"{}");
  }

  TEST_CASE("Print empty can fail") {
    REQUIRE(!print(unival {},
                   [](u8string_view) -> bool { return false; }));
  }

  TEST_CASE("Print boolean unival") {
    REQUIRE(to_string(unival { true }) == u8"true");
    REQUIRE(to_string(unival { false }) == u8"false");
  }

  TEST_CASE("Print boolean can fail") {
    REQUIRE(!print(unival { true },
                   [](u8string_view) -> bool { return false; }));
  }

  TEST_CASE("Print float unival") {
    REQUIRE(to_string(unival { 42.0 }) == u8"42.0");
  }

  TEST_CASE("Print big float unival 1") {
    REQUIRE(to_string(unival { 42e10 }) == u8"420000000000.0");
  }

  TEST_CASE("Print big float unival 2") {
    REQUIRE(to_string(unival { 42e20 }) == u8"4.2e+21");
  }

  TEST_CASE("Print big float unival 3") {
    REQUIRE(to_string(unival { 4e20 }) == u8"4e+20");
  }

  TEST_CASE("Print float can fail") {
    REQUIRE(!print(unival { 4.2 },
                   [](u8string_view) -> bool { return false; }));
  }

  TEST_CASE("Print string unival") {
    REQUIRE(to_string(unival { u8"42" }) == u8"\"42\"");
  }

  TEST_CASE("Print string unival with escape") {
    REQUIRE(to_string(unival { u8"\"" }) == u8"\"\\\"\"");
  }

  TEST_CASE("Print string can fail 1") {
    REQUIRE(!print(unival { u8"42" },
                   [](u8string_view) -> bool { return false; }));
  }

  TEST_CASE("Print string can fail 2") {
    REQUIRE(!print(
        unival { u8"42" },
        [n = 0](u8string_view) mutable -> bool { return n++ != 1; }));
  }

  TEST_CASE("Print string can fail 3") {
    REQUIRE(!print(unival { u8"42" },
                   [n = 0](u8string_view s) mutable -> bool {
                     return n++ != 2;
                   }));
  }

  TEST_CASE("Print id string without parenthesis") {
    REQUIRE(to_string(unival { u8"foo" }) == u8"foo");
    REQUIRE(to_string(unival { u8"FooBar" }) == u8"FooBar");
    REQUIRE(to_string(unival { u8"_1foo" }) == u8"_1foo");
    REQUIRE(to_string(unival { u8"_1FooBar" }) == u8"_1FooBar");
  }

  TEST_CASE("Print empty byte array") {
    REQUIRE(to_string(unival { bytes {} }) == u8"::{}");
  }

  TEST_CASE("Print byte array") {
    REQUIRE(to_string(unival { bytes { 1, 2, 3, 4, 0x1f, -8 } }) ==
            u8"::{01 02 03 04 1f f8}");
  }

  TEST_CASE("Print byte may fail") {
    REQUIRE(!print(unival { bytes { 1, 2, 3, 4, 0x1f, -8 } },
                   [](u8string_view) -> bool { return false; }));
    REQUIRE(!print(
        unival { bytes { 1, 2, 3, 4, 0x1f, -8 } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 1; }));
    REQUIRE(!print(
        unival { bytes { 1, 2, 3, 4, 0x1f, -8 } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 2; }));
    REQUIRE(!print(
        unival { bytes { 1, 2, 3, 4, 0x1f, -8 } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 3; }));
  }

  TEST_CASE("Print empty vector") {
    REQUIRE(to_string(unival { vector {} }) == u8"[]");
  }

  TEST_CASE("Print 1-vector") {
    REQUIRE(to_string(unival { vector { unival { 42 } } }) ==
            u8"[42]");
  }

  TEST_CASE("Print 3-vector") {
    REQUIRE(to_string(unival { vector {
                unival { 42 },
                unival { 43 },
                unival { 44 },
            } }) == u8"42,43,44");
  }

  TEST_CASE("Print nested vector") {
    REQUIRE(to_string(unival { vector {
                unival { 42 },
                unival { vector { unival { 43 }, unival { 44 } } },
                unival { 45 } } }) == u8"42,[43,44],45");
  }

  TEST_CASE("Print vector may fail") {
    REQUIRE(!print(unival { vector { unival { 42 } } },
                   [](u8string_view) -> bool { return false; }));
    REQUIRE(!print(
        unival { vector { unival { 42 } } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 1; }));
    REQUIRE(!print(
        unival { vector { unival { 42 } } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 2; }));
    REQUIRE(!print(
        unival {
            vector { unival { 42 }, unival { 43 }, unival { 44 } } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 3; }));
  }

  TEST_CASE("Print empty composite") {
    REQUIRE(to_string(unival { composite {

            } }) == u8"{}");
  }

  TEST_CASE("Print 1-composite") {
    REQUIRE(to_string(unival { composite {
                { unival { 42 }, unival { 43 } },
            } }) == u8"{42:43}");
  }

  TEST_CASE("Print 3-composite") {
    REQUIRE(to_string(unival {
                composite { { unival { 42 }, unival { 43 } },
                            { unival { 44 }, unival { 45 } },
                            { unival { 46 }, unival { 47 } } } }) ==
            u8"{42:43 44:45 46:47}");
  }

  TEST_CASE("Print composite may fail") {
    REQUIRE(!print(
        unival { composite { { unival { 42 }, unival { 43 } } } },
        [](u8string_view) -> bool { return false; }));
    REQUIRE(!print(
        unival { composite { { unival { 42 }, unival { 43 } } } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 1; }));
    REQUIRE(!print(
        unival { composite { { unival { 42 }, unival { 43 } } } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 2; }));
    REQUIRE(!print(
        unival { composite { { unival { 42 }, unival { 43 } },
                             { unival { 44 }, unival { 45 } },
                             { unival { 46 }, unival { 47 } } } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 3; }));
    REQUIRE(!print(
        unival { composite { { unival { 42 }, unival { 43 } },
                             { unival { 44 }, unival { 45 } },
                             { unival { 46 }, unival { 47 } } } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 4; }));
    REQUIRE(!print(
        unival { composite { { unival { 42 }, unival { 43 } },
                             { unival { 44 }, unival { 45 } },
                             { unival { 46 }, unival { 47 } } } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 5; }));
  }

  TEST_CASE("Pretty print empty") {
    REQUIRE(to_string(unival {}, pretty) == u8"{ }");
  }

  TEST_CASE("Pretty print byte array empty") {
    REQUIRE(to_string(unival { bytes {} }, pretty) == u8":: { }");
  }

  TEST_CASE("Pretty print byte array") {
    REQUIRE(to_string(unival { bytes { 1, 2, 3, 4, 0x1f, -8 } },
                      pretty) == u8":: {\n"
                                 "  01 02 03 04 1f f8\n"
                                 "}");
  }

  TEST_CASE("Pretty print big byte array") {
    REQUIRE(to_string(unival { bytes {
                          0,    1,    2,    3,    4,    5,    6,
                          7,    8,    9,    0xa,  0xb,  0xc,  0xd,
                          0xe,  0xf,  0x10, 0x11, 0x12, 0x13, 0x14,
                          0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
                          0x1c, 0x1d, 0x1e, 0x1f } },
                      pretty) ==
            u8":: {\n"
            "  00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f\n"
            "  10 11 12 13 14 15 16 17 18 19 1a 1b 1c 1d 1e 1f\n"
            "}");
  }

  TEST_CASE("Pretty print empty vector") {
    REQUIRE(to_string(unival { vector {} }, pretty) == u8"[ ]");
  }

  TEST_CASE("Pretty print 1-vector") {
    REQUIRE(to_string(unival { vector { unival { 42 } } }, pretty) ==
            u8"[\n"
            u8"  42\n"
            u8"]");
  }

  TEST_CASE("Pretty print 3-vector") {
    REQUIRE(to_string(unival { vector { unival { 42 }, unival { 43 },
                                        unival { 44 } } },
                      pretty) == u8"[\n"
                                 u8"  42,\n"
                                 u8"  43,\n"
                                 u8"  44\n"
                                 u8"]");
  }

  TEST_CASE("Pretty print empty composite") {
    REQUIRE(to_string(unival { composite {} }, pretty) == u8"{ }");
  }

  TEST_CASE("Pretty print 1-composite") {
    REQUIRE(to_string(unival { composite {
                          { unival { 42 }, unival { 43 } },
                      } },
                      pretty) == u8"{\n"
                                 u8"  42: 43;\n"
                                 u8"}");
  }

  TEST_CASE("Pretty print 3-composite") {
    REQUIRE(to_string(unival { composite {
                          { unival { 42 }, unival { 43 } },
                          { unival { 44 }, unival { 45 } },
                          { unival { 46 }, unival { 47 } } } },
                      pretty) == u8"{\n"
                                 u8"  42: 43;\n"
                                 u8"  44: 45;\n"
                                 u8"  46: 47;\n"
                                 u8"}");
  }

  TEST_CASE("Pretty print nested") {
    REQUIRE(
        to_string(unival { composite {
                      { unival { 42 }, unival { 43 } },
                      { unival { 44 }, unival { bytes { 1, 2, 3 } } },
                      { unival { 45 },
                        unival { vector { unival { 1 }, unival { 2 },
                                          unival { 3 } } } },
                      { unival { 46 }, unival { composite { {
                                           unival { 1 },
                                           unival { 2 },
                                       } } } } } },
                  pretty) == u8"{\n"
                             u8"  42: 43;\n"
                             u8"  44: :: {\n"
                             u8"    01 02 03\n"
                             u8"  };\n"
                             u8"  45: [\n"
                             u8"    1,\n"
                             u8"    2,\n"
                             u8"    3\n"
                             u8"  ];\n"
                             u8"  46: {\n"
                             u8"    1: 2;\n"
                             u8"  };\n"
                             u8"}");
  }
}
