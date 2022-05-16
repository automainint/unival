/*  Copyright (c) 2022 Mitya Selivanov
 *
 *  This file is part of the Laplace project.
 *
 *  Laplace is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 *  the MIT License for more details.
 */

#include "../../unival/unival.h"

#include "catch2/catch.hpp"
#include <vector>

namespace unival::test {
  using std::u8string_view, std::vector;

  TEST_CASE("unival created") { auto val = unival {}; }

  TEST_CASE("unival is empty by default") {
    auto val = unival {};
    REQUIRE(val.is_empty());
  }

  TEST_CASE("unival is not empty when assigned to a value") {
    auto val = unival { 42 };
    REQUIRE(!val.is_empty());
  }

  TEST_CASE("create unival from char") {
    auto val = unival { static_cast<signed char>(-42) };
    REQUIRE(val.get_integer() == -42);
  }

  TEST_CASE("create unival from short") {
    auto val = unival { static_cast<signed short>(-4242) };
    REQUIRE(val.get_integer() == -4242);
  }

  TEST_CASE("create unival from int") {
    auto val = unival { static_cast<signed int>(-424242) };
    REQUIRE(val.get_integer() == -424242);
  }

  TEST_CASE("create unival from long") {
    auto val = unival { static_cast<signed long>(-424242) };
    REQUIRE(val.get_integer() == -424242);
  }

  TEST_CASE("create unival from long long") {
    auto val = unival { static_cast<signed long long>(-4242424242) };
    REQUIRE(val.get_integer() == -4242424242);
  }

  TEST_CASE("create unival from unsigned char") {
    auto val = unival { static_cast<unsigned char>(42) };
    REQUIRE(val.get_uint() == 42);
  }

  TEST_CASE("create unival from unsigned short") {
    auto val = unival { static_cast<unsigned short>(4242) };
    REQUIRE(val.get_uint() == 4242);
  }

  TEST_CASE("create unival from unsigned int") {
    auto val = unival { static_cast<unsigned int>(424242) };
    REQUIRE(val.get_uint() == 424242);
  }

  TEST_CASE("create unival from unsigned long") {
    auto val = unival { static_cast<unsigned long>(424242) };
    REQUIRE(val.get_uint() == 424242);
  }

  TEST_CASE("create unival from unsigned long long") {
    auto val = unival { static_cast<unsigned long long>(4242424242) };
    REQUIRE(val.get_uint() == 4242424242);
  }

  TEST_CASE("create unival from unsigned long long extreme value") {
    auto val =
        unival { static_cast<unsigned long long>(18446744073709551615ull) };
    REQUIRE(val.get_uint() == 18446744073709551615ull);
  }

  TEST_CASE("create unival from float") {
    auto val = unival { 42.42f };
    REQUIRE(val.get_float() == Approx(42.42f));
  }

  TEST_CASE("create unival from double") {
    auto val = unival { 424242.42 };
    REQUIRE(val.get_float() == Approx(424242.42));
  }

  TEST_CASE("get uint from int") {
    auto val = unival { 42 };
    REQUIRE(val.get_uint() == 42);
  }

  TEST_CASE("get int from uint") {
    auto val = unival { 42u };
    REQUIRE(val.get_integer() == 42);
  }

  TEST_CASE("can not get float from int") {
    auto val = unival { 42 };
    REQUIRE(!val.get_float().has_value());
  }

  TEST_CASE("can not get float from uint") {
    auto val = unival { 42u };
    REQUIRE(!val.get_float().has_value());
  }

  TEST_CASE("can not get int from float") {
    auto val = unival { 42.42f };
    REQUIRE(!val.get_integer().has_value());
  }

  TEST_CASE("can not get uint from float") {
    auto val = unival { 42.42f };
    REQUIRE(!val.get_uint().has_value());
  }

  TEST_CASE("is integer when created from int") {
    auto val = unival { 42 };
    REQUIRE(val.is_integer());
  }

  TEST_CASE("is integer when created from uint") {
    auto val = unival { 42u };
    REQUIRE(val.is_integer());
  }

  TEST_CASE("is not integer when created from float") {
    auto val = unival { 42.42f };
    REQUIRE(!val.is_integer());
  }

  TEST_CASE("is float when created from float") {
    auto val = unival { 42.42f };
    REQUIRE(val.is_float());
  }

  TEST_CASE("is float when created from double") {
    auto val = unival { 42.42 };
    REQUIRE(val.is_float());
  }

  TEST_CASE("is not float when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.is_float());
  }

  TEST_CASE("create unival from string") {
    auto val = unival { "42" };
    REQUIRE(val.get_string() == u8string_view(u8"42"));
  }

  TEST_CASE("create unival from UTF-8 string") {
    auto val = unival { u8"42" };
    REQUIRE(val.get_string() == u8string_view(u8"42"));
  }

  TEST_CASE("can not get string from int") {
    auto val = unival { 42 };
    REQUIRE(!val.get_string().has_value());
  }

  TEST_CASE("is string when created from string") {
    auto val = unival { "42" };
    REQUIRE(val.is_string());
  }

  TEST_CASE("is not string when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.is_string());
  }

  TEST_CASE("create unival from byte array") {
    auto compare_bytes = [](auto left, auto right) -> bool {
      return std::equal(left.begin(), left.end(), right.begin(), right.end());
    };

    auto val = unival { vector<int8_t> { 1, 2, 3 } };
    REQUIRE(compare_bytes(val.get_bytes().value(), vector<int8_t> { 1, 2, 3 }));
  }

  TEST_CASE("can not get byte array from int") {
    auto val = unival { 42 };
    REQUIRE(!val.get_bytes().has_value());
  }

  TEST_CASE("is byte array when created from byte array") {
    auto val = unival { vector<int8_t> { 1, 2, 3 } };
    REQUIRE(val.is_bytes());
  }

  TEST_CASE("is not byte array when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.is_bytes());
  }
}
