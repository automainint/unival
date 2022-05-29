/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/unival.h"

#include "catch2/catch.hpp"
#include <vector>

namespace unival::test {
  using std::vector, std::pair;

  TEST_CASE("can not iterate over empty unival") {
    auto val = unival {};
    for (auto x : val)
      REQUIRE(false);
  }

  TEST_CASE("get value from end iterator of int") {
    auto val = unival { 1 };
    REQUIRE((*val.end()).is_empty());
  }

  TEST_CASE("get value from end iterator of vector") {
    auto val = unival { vector<unival> { unival { 1 } } };
    REQUIRE((*val.end()).is_empty());
  }

  TEST_CASE("get value from end iterator of composite") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } } } };
    REQUIRE((*val.end()).is_empty());
  }

  TEST_CASE("can iterate over vector unival") {
    auto val = unival { vector<unival> { unival { 1 }, unival { 2 },
                                         unival { 3 } } };
    int i = 0;
    for (auto x : val)
      REQUIRE(x == unival { ++i });
    REQUIRE(i == 3);
  }

  TEST_CASE("can iterate over composite unival keys") {
    auto val = unival { vector<pair<unival, unival>> {
        { unival { 1 }, unival { 2 } },
        { unival { 3 }, unival { 4 } },
        { unival { 5 }, unival { 6 } } } };
    int i = -1;
    for (auto x : val)
      REQUIRE(x == unival { i += 2 });
    REQUIRE(i == 5);
  }

  TEST_CASE(
      "iterator from out of bounds index will return empty unival") {
    auto val = unival { vector<unival> { unival { 1 } } };
    auto i = iterator<unival> { val, -1 };
    auto j = iterator<unival> { val, 2 };
    REQUIRE((*i).is_empty());
    REQUIRE((*j).is_empty());
  }

  TEST_CASE("use arrow with iterator of vector unival") {
    auto val = unival { vector<unival> { unival { 1 } } };
    REQUIRE(val.begin()->get_integer() == 1);
  }
}
