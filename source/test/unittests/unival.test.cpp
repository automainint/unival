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

namespace unival::test {
  TEST_CASE("unival created") { auto val = unival{}; }

  TEST_CASE("unival is empty by default") {
    auto val = unival{};
    REQUIRE(val.is_empty());
  }

  TEST_CASE("unival is not empty when assigned to a value") {
    auto val = unival{42};
    REQUIRE(!val.is_empty());
  }

  TEST_CASE("create unival from char") {
    auto val = unival{static_cast<char>(-42)};
    REQUIRE(val.get_integer() == -42);
  }

  TEST_CASE("create unival from short") {
    auto val = unival{static_cast<short>(-4242)};
    REQUIRE(val.get_integer() == -4242);
  }

  TEST_CASE("create unival from int") {
    auto val = unival{static_cast<int>(-424242)};
    REQUIRE(val.get_integer() == -424242);
  }

  TEST_CASE("create unival from long") {
    auto val = unival{static_cast<int>(-424242)};
    REQUIRE(val.get_integer() == -424242);
  }

  TEST_CASE("create unival from long long") {
    auto val = unival{static_cast<long long>(-4242424242)};
    REQUIRE(val.get_integer() == -4242424242);
  }

  TEST_CASE("create unival from unsigned char") {
    auto val = unival{static_cast<unsigned char>(42)};
    REQUIRE(val.get_uint() == 42);
  }

  TEST_CASE("create unival from unsigned short") {
    auto val = unival{static_cast<unsigned short>(4242)};
    REQUIRE(val.get_uint() == 4242);
  }

  TEST_CASE("create unival from unsigned int") {
    auto val = unival{static_cast<unsigned int>(424242)};
    REQUIRE(val.get_uint() == 424242);
  }

  TEST_CASE("create unival from unsigned long") {
    auto val = unival{static_cast<unsigned long>(424242)};
    REQUIRE(val.get_uint() == 424242);
  }

  TEST_CASE("create unival from unsigned long long") {
    auto val = unival{static_cast<unsigned long long>(4242424242)};
    REQUIRE(val.get_uint() == 4242424242);
  }

  TEST_CASE("create unival from unsigned long long extreme value") {
    auto val = unival{static_cast<unsigned long long>(18446744073709551615ull)};
    REQUIRE(val.get_uint() == 18446744073709551615ull);
  }

  TEST_CASE("create unival from float") {
    auto val = unival{42.42f};
    REQUIRE(val.get_float() == Approx(42.42f));
  }
  
  TEST_CASE("create unival from double") {
    auto val = unival{424242.42};
    REQUIRE(val.get_float() == Approx(424242.42));
  }
}
