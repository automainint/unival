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

  TEST_CASE("get value from end iterator") {
    auto val = unival { 1 };
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
}
