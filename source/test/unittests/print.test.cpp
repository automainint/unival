/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/print.h"

#include "catch2/catch.hpp"

#include <iostream>

namespace unival::test {
  using std::u8string, std::u8string_view, std::vector;

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
    REQUIRE(to_string(unival { vector<int8_t> {} }) == u8"::{}");
  }

  TEST_CASE("Print byte array") {
    REQUIRE(to_string(
                unival { vector<int8_t> { 1, 2, 3, 4, 0x1f, -8 } }) ==
            u8"::{01 02 03 04 1f f8}");
  }

  TEST_CASE("Print byte may fail") {
    REQUIRE(!print(unival { vector<int8_t> { 1, 2, 3, 4, 0x1f, -8 } },
                   [](u8string_view) -> bool { return false; }));
    REQUIRE(!print(
        unival { vector<int8_t> { 1, 2, 3, 4, 0x1f, -8 } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 1; }));
    REQUIRE(!print(
        unival { vector<int8_t> { 1, 2, 3, 4, 0x1f, -8 } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 2; }));
    REQUIRE(!print(
        unival { vector<int8_t> { 1, 2, 3, 4, 0x1f, -8 } },
        [i = 0](u8string_view) mutable -> bool { return i++ != 3; }));
  }

  TEST_CASE("Print empty vector") {
    REQUIRE(to_string(unival { vector<unival> {} }) == u8"[]");
  }

  TEST_CASE("Print 1-vector") {
    REQUIRE(to_string(unival { vector<unival> { unival { 42 } } }) ==
            u8"[42]");
  }

  TEST_CASE("Print 3-vector") {
    REQUIRE(to_string(unival { vector<unival> {
                unival { 42 },
                unival { 43 },
                unival { 44 },
            } }) == u8"42,43,44");
  }

  TEST_CASE("Print nested vector") {
    REQUIRE(to_string(unival { vector<unival> {
                unival { 42 },
                unival {
                    vector<unival> { unival { 43 }, unival { 44 } } },
                unival { 45 } } }) == u8"42,[43,44],45");
  }
}
