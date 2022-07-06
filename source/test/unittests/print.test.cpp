/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/print.h"
#include "catch2/catch.hpp"

namespace unival::test {
  using std::string, std::string_view, std::u8string_view;

  TEST_CASE("print int unival") {
    auto val = unival { 42 };
    auto str = string {};
    auto put = [&](string_view s) -> ptrdiff_t {
      str.append(s.begin(), s.end());
      return static_cast<ptrdiff_t>(s.size());
    };

    REQUIRE(print(val, put));
    REQUIRE(str == "42");
  }

  TEST_CASE("print can fail if callback fails") {
    REQUIRE(!print(unival { 42 },
                   [](string_view) -> bool { return false; }));
  }

  TEST_CASE("print can fail if value is invalid") {
    auto put = [](string_view) -> bool { return true; };

    REQUIRE(!print(unival { 42 }.set(0, unival { 0 }), put));
  }

  TEST_CASE("print into string") {
    REQUIRE(to_string(unival { 42 }) == u8"42");
  }

  TEST_CASE("print to string can fail") {
    REQUIRE(
        !to_string(unival { 42 }.set(0, unival { 0 })).has_value());
  }

  TEST_CASE("print empty unival") {
    REQUIRE(to_string(unival {}) == u8"{}");
  }

  TEST_CASE("print empty can fail") {
    REQUIRE(
        !print(unival {}, [](string_view) -> bool { return false; }));
  }

  TEST_CASE("print boolean unival") {
    REQUIRE(to_string(unival { true }) == u8"true");
    REQUIRE(to_string(unival { false }) == u8"false");
  }

  TEST_CASE("print boolean can fail") {
    REQUIRE(!print(unival { true },
                   [](string_view) -> bool { return false; }));
  }

  TEST_CASE("print float unival") {
    REQUIRE(to_string(unival { 42.0 }) == u8"42.0");
  }

  TEST_CASE("print big float unival 1") {
    REQUIRE(to_string(unival { 42e10 }) == u8"420000000000.0");
  }

  TEST_CASE("print big float unival 2") {
    REQUIRE(to_string(unival { 42e20 }) == u8"4.2e+21");
  }

  TEST_CASE("print big float unival 3") {
    REQUIRE(to_string(unival { 4e20 }) == u8"4e+20");
  }

  TEST_CASE("print float can fail") {
    REQUIRE(!print(unival { 4.2 },
                   [](string_view) -> bool { return false; }));
  }

  TEST_CASE("print string unival") {
    REQUIRE(to_string(unival { "42" }) == u8"\"42\"");
  }

  TEST_CASE("print string unival with escape") {
    REQUIRE(to_string(unival { "\"" }) == u8R"("\"")");
  }

  TEST_CASE("print string unival with escaped hex") {
    REQUIRE(to_string(unival { "\x04\xaf"
                               "42" }) == u8R"("\x04\xaf""42")");
    REQUIRE(to_string(unival { "\x04\xaf"
                               "af" }) == u8R"("\x04\xaf""af")");
  }

  TEST_CASE("print string can fail 1") {
    REQUIRE(!print(unival { "42" },
                   [](string_view) -> bool { return false; }));
  }

  TEST_CASE("print string can fail 2") {
    REQUIRE(
        !print(unival { "42" }, [n = 0](string_view) mutable -> bool {
          return n++ != 1;
        }));
  }

  TEST_CASE("print string can fail 3") {
    REQUIRE(!print(
        unival { "42" },
        [n = 0](string_view s) mutable -> bool { return n++ != 2; }));
  }

  TEST_CASE("print id string without parenthesis") {
    REQUIRE(to_string(unival { "foo" }) == u8"foo");
    REQUIRE(to_string(unival { "FooBar" }) == u8"FooBar");
    REQUIRE(to_string(unival { "_1foo" }) == u8"_1foo");
    REQUIRE(to_string(unival { "_1FooBar" }) == u8"_1FooBar");
  }

  TEST_CASE("print empty byte array") {
    REQUIRE(to_string(unival { bytes {} }) == u8"::{}");
  }

  TEST_CASE("print byte array") {
    REQUIRE(to_string(unival { bytes { 1, 2, 3, 4, 0x1f, -8 } }) ==
            u8"::{01 02 03 04 1f f8}");
  }

  TEST_CASE("print byte may fail") {
    for (int fail_index = 0; fail_index < 10; fail_index++) {
      REQUIRE(!print(unival { bytes { 1, 2, 3, 4, 0x1f, -8 } },
                     [&, i = 0](string_view) mutable -> bool {
                       return i++ != fail_index;
                     }));
    }
  }

  TEST_CASE("print empty vector") {
    REQUIRE(to_string(unival { vector {} }) == u8"[]");
  }

  TEST_CASE("print 1-vector") {
    REQUIRE(to_string(unival { vector { unival { 42 } } }) ==
            u8"[42]");
  }

  TEST_CASE("print 3-vector") {
    REQUIRE(to_string(unival { vector {
                unival { 42 },
                unival { 43 },
                unival { 44 },
            } }) == u8"[42,43,44]");
  }

  TEST_CASE("print nested vector") {
    REQUIRE(to_string(unival { vector {
                unival { 42 },
                unival { vector { unival { 43 }, unival { 44 } } },
                unival { 45 } } }) == u8"[42,[43,44],45]");
  }

  TEST_CASE("print vector may fail") {
    for (int fail_index = 0; fail_index < 3; fail_index++) {
      REQUIRE(!print(unival { vector { unival { 42 } } },
                     [&, i = 0](string_view) mutable -> bool {
                       return i++ != fail_index;
                     }));
    }

    for (int fail_index = 0; fail_index < 6; fail_index++) {
      REQUIRE(!print(unival { vector { unival { 42 }, unival { 43 },
                                       unival { 44 } } },
                     [&, i = 0](string_view) mutable -> bool {
                       return i++ != fail_index;
                     }));
    }
  }

  TEST_CASE("print empty composite") {
    REQUIRE(to_string(unival { composite {

            } }) == u8"{}");
  }

  TEST_CASE("print 1-composite") {
    REQUIRE(to_string(unival { composite {
                { unival { 42 }, unival { 43 } },
            } }) == u8"{42:43}");
  }

  TEST_CASE("print 3-composite") {
    REQUIRE(to_string(unival {
                composite { { unival { 42 }, unival { 43 } },
                            { unival { 44 }, unival { 45 } },
                            { unival { 46 }, unival { 47 } } } }) ==
            u8"{42:43 44:45 46:47}");
  }

  TEST_CASE("print composite may fail") {
    for (int fail_index = 0; fail_index < 3; fail_index++) {
      REQUIRE(!print(
          unival { composite { { unival { 42 }, unival { 43 } } } },
          [&, i = 0](string_view) mutable -> bool {
            return i++ != fail_index;
          }));
    }
    for (int fail_index = 0; fail_index < 6; fail_index++) {
      REQUIRE(!print(
          unival { composite { { unival { 42 }, unival { 43 } },
                               { unival { 44 }, unival { 45 } },
                               { unival { 46 }, unival { 47 } } } },
          [&, i = 0](string_view) mutable -> bool {
            return i++ != fail_index;
          }));
    }
  }

  TEST_CASE("pretty print empty") {
    REQUIRE(to_string(unival {}, pretty) == u8"{ }");
  }

  TEST_CASE("pretty print string unival with escaped hex") {
    REQUIRE(to_string(unival { "\x04\xaf"
                               "42" },
                      pretty) == u8R"("\x04\xaf" "42")");
    REQUIRE(to_string(unival { "\x04\xaf"
                               "af" },
                      pretty) == u8R"("\x04\xaf" "af")");
  }

  TEST_CASE("pretty print byte array empty") {
    REQUIRE(to_string(unival { bytes {} }, pretty) == u8":: { }");
  }

  TEST_CASE("pretty print byte array") {
    REQUIRE(to_string(unival { bytes { 1, 2, 3, 4, 0x1f, -8 } },
                      pretty) == u8":: {\n"
                                 "  01 02 03 04 1f f8\n"
                                 "}");
  }

  TEST_CASE("pretty print big byte array") {
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

  TEST_CASE("pretty print empty vector") {
    REQUIRE(to_string(unival { vector {} }, pretty) == u8"[ ]");
  }

  TEST_CASE("pretty print 1-vector") {
    REQUIRE(to_string(unival { vector { unival { 42 } } }, pretty) ==
            u8"[\n"
            "  42\n"
            "]");
  }

  TEST_CASE("pretty print 3-vector") {
    REQUIRE(to_string(unival { vector { unival { 42 }, unival { 43 },
                                        unival { 44 } } },
                      pretty) == u8"[\n"
                                 "  42,\n"
                                 "  43,\n"
                                 "  44\n"
                                 "]");
  }

  TEST_CASE("pretty print empty composite") {
    REQUIRE(to_string(unival { composite {} }, pretty) == u8"{ }");
  }

  TEST_CASE("pretty print 1-composite") {
    REQUIRE(to_string(unival { composite {
                          { unival { 42 }, unival { 43 } },
                      } },
                      pretty) == u8"{\n"
                                 "  42: 43;\n"
                                 "}");
  }

  TEST_CASE("pretty print 3-composite") {
    REQUIRE(to_string(unival { composite {
                          { unival { 42 }, unival { 43 } },
                          { unival { 44 }, unival { 45 } },
                          { unival { 46 }, unival { 47 } } } },
                      pretty) == u8"{\n"
                                 "  42: 43;\n"
                                 "  44: 45;\n"
                                 "  46: 47;\n"
                                 "}");
  }

  TEST_CASE("compact print nested") {
    REQUIRE(to_string(unival { composite {
                { unival { 42 }, unival { 43 } },
                { unival { 44 }, unival { bytes { 1, 2, 3 } } },
                { unival { 45 },
                  unival { vector { unival { 1 }, unival { 2 },
                                    unival { 3 } } } },
                { unival { 46 }, unival { composite { {
                                     unival { 1 },
                                     unival { 2 },
                                 } } } } } }) ==
            u8"{42:43 44:::{01 02 03} 45:[1,2,3] 46:{1:2}}");
  }

  TEST_CASE("pretty print nested") {
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
                             "  42: 43;\n"
                             "  44: :: {\n"
                             "    01 02 03\n"
                             "  };\n"
                             "  45: [\n"
                             "    1,\n"
                             "    2,\n"
                             "    3\n"
                             "  ];\n"
                             "  46: {\n"
                             "    1: 2;\n"
                             "  };\n"
                             "}");
  }

  TEST_CASE("json print empty") {
    REQUIRE(to_string(unival {}, json_compact) == u8"null");
    REQUIRE(to_string(unival {}, json_pretty) == u8"null");
  }

  TEST_CASE("json print nested") {
    REQUIRE(
        to_string(unival { composite {
                      { unival { "42" }, unival { 43 } },
                      { unival { "foo" },
                        unival { vector { unival { 1 }, unival { 2 },
                                          unival { 3 } } } },
                      { unival { "bar" }, unival { composite { {
                                              unival { "1" },
                                              unival { 2 },
                                          } } } } } },
                  json_compact) ==
        u8"{\"42\":43,\"bar\":{\"1\":2},\"foo\":[1,2,3]}");
  }

  TEST_CASE("json pretty print nested") {
    REQUIRE(
        to_string(unival { composite {
                      { unival { "42" }, unival { 43 } },
                      { unival { "foo" },
                        unival { vector { unival { 1 }, unival { 2 },
                                          unival { 3 } } } },
                      { unival { "bar" }, unival { composite { {
                                              unival { "1" },
                                              unival { 2 },
                                          } } } } } },
                  json_pretty) == u8"{\n"
                                  "  \"42\": 43,\n"
                                  "  \"bar\": {\n"
                                  "    \"1\": 2\n"
                                  "  },\n"
                                  "  \"foo\": [\n"
                                  "    1,\n"
                                  "    2,\n"
                                  "    3\n"
                                  "  ]\n"
                                  "}");
  }

  TEST_CASE("print complex unival may fail") {
    for (int fail_index = 0; fail_index < 60; fail_index++) {
      REQUIRE(!print(
          unival { composite {
              { unival { 42 }, unival { "\"" } },
              { unival { 44 }, unival { bytes { 1, 2, 3 } } },
              { unival { 45 },
                unival { vector { unival { 1 }, unival { 2 },
                                  unival { 3 }, unival { 4 } } } },
              { unival { 46 },
                unival {
                    composite { { unival { 1 }, unival { 2 } } } } },
              { unival { 47 }, unival { 1 } },
              { unival { 48 }, unival { 1 } },
              { unival { 49 },
                unival { bytes { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                 12 } } } } },
          [&, i = 0](string_view) mutable -> bool {
            return i++ != fail_index;
          }));

      REQUIRE(!print(
          unival { composite {
              { unival { 42 }, unival { bytes { int8_t { 1 } } } },
              { unival { 43 },
                unival {
                    composite { { unival { 1 }, unival { 2 } } } } },
              { unival { 44 }, unival { vector { unival { 1 } } } },
              { unival { 45 }, unival { 1 } },
              { unival { 46 }, unival { 1 } },
              { unival { 47 },
                unival { bytes { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                 12 } } } } },
          [&, i = 0](string_view) mutable -> bool {
            return i++ != fail_index;
          },
          pretty));

      REQUIRE(!print(
          unival { composite {
              { unival { "42" }, unival { 43 } },
              { unival { "44" },
                unival { vector { unival { 1 }, unival { 2 },
                                  unival { 3 }, unival { 4 } } } },
              { unival { "45" },
                unival { composite {
                    { unival { "1" }, unival { 2 } } } } },
              { unival { "46" }, unival { 1 } },
              { unival { "47" }, unival { 1 } },
              { unival { "48" }, unival { 1 } },
              { unival { "49" },
                unival { bytes { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                 12 } } } } },
          [&, i = 0](string_view) mutable -> bool {
            return i++ != fail_index;
          },
          json_compact));

      REQUIRE(!print(
          unival { composite {
              { unival { "42" }, unival { 43 } },
              { unival { "44" },
                unival { vector { unival { 1 }, unival { 2 },
                                  unival { 3 } } } },
              { unival { "45" },
                unival { composite {
                    { unival { "1" }, unival { 2 } } } } },
              { unival { "46" }, unival { 1 } },
              { unival { "47" }, unival { 1 } },
              { unival { "48" }, unival { 1 } },
              { unival { "49" },
                unival { bytes { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
                                 12 } } } } },
          [&, i = 0](string_view) mutable -> bool {
            return i++ != fail_index;
          },
          json_pretty));
    }
  }

  TEST_CASE("print non-ASCII characters") {
    char8_t s[] = { 0x1f, 0x7f, 0xe2, 0x98, 0x83, 0 };
    REQUIRE(to_string(unival { u8string_view { s, 5 } }) ==
            u8"\"\\x1f\\x7f\\xe2\\x98\\x83\"");
  }

  TEST_CASE("print non-ASCII characters may fail") {
    for (int fail_index = 0; fail_index < 5; fail_index++) {
      REQUIRE(!print(unival { "\x1f\x7f\xe2\x98\x83" },
                     [&, i = 0](string_view) mutable -> bool {
                       return i++ != fail_index;
                     }));
    }
  }
}
