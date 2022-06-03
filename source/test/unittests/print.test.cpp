/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/print.h"

#include "catch2/catch.hpp"

#include <iostream>

namespace unival::test {
  using std::u8string, std::u8string_view;

  TEST_CASE("print int unival") {
    auto val = unival { 42 };
    auto str = u8string {};
    auto put = [&](u8string_view s) -> signed long long {
      str.append(s.begin(), s.end());
      return static_cast<signed long long>(s.size());
    };

    REQUIRE(print(val, put));
    REQUIRE(str == u8"42");
  }

  TEST_CASE("print can fail if callback fails") {
    REQUIRE(
        !print(unival { 42 }, [&](u8string_view) -> signed long long {
          return -1;
        }));
  }

  TEST_CASE("print can fail if value is invalid") {
    auto put = [&](u8string_view s) -> signed long long {
      return static_cast<signed long long>(s.size());
    };

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
    REQUIRE(!print(unival {}, [&](u8string_view) -> signed long long {
      return -1;
    }));
  }

  TEST_CASE("print boolean unival") {
    REQUIRE(to_string(unival { true }) == u8"true");
    REQUIRE(to_string(unival { false }) == u8"false");
  }

  TEST_CASE("print boolean can fail") {
    REQUIRE(!print(
        unival { true },
        [&](u8string_view) -> signed long long { return -1; }));
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
    REQUIRE(!print(
        unival { 4.2 },
        [&](u8string_view) -> signed long long { return -1; }));
  }

  TEST_CASE("print string unival") {
    REQUIRE(to_string(unival { u8"42" }) == u8"\"42\"");
  }

  TEST_CASE("print string unival with escape") {
    REQUIRE(to_string(unival { u8"\"" }) == u8"\"\\\"\"");
  }

  TEST_CASE("print string can fail") {
    REQUIRE(!print(
        unival { u8"42" },
        [&](u8string_view) -> signed long long { return -1; }));
  }
}
