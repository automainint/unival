/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/unival.h"

#include "catch2/catch.hpp"
#include <vector>

namespace unival::test {
  using std::u8string_view, std::vector, std::span, std::pair,
      std::string, std::u8string, std::optional;

  TEST_CASE("unival created") { auto val = unival {}; }

  TEST_CASE("unival is empty by default") {
    auto val = unival {};
    REQUIRE(val.is_empty());
  }

  TEST_CASE("unival created from nullopt") {
    auto val = unival { optional<unival> {} };
    REQUIRE(val.is_empty());
  }

  TEST_CASE("unival assigned to nullopt") {
    auto val = unival {};
    val = optional<unival> {};
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

  TEST_CASE("create unival from unsigned long long extreme "
            "value") {
    auto val = unival { static_cast<unsigned long long>(
        18446744073709551615ull) };
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

  TEST_CASE("create unival from optional") {
    auto foo = optional<unival> { unival { 42 } };
    auto bar = unival { foo };
    REQUIRE(bar.get_integer() == 42);
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
      return std::equal(left.begin(), left.end(), right.begin(),
                        right.end());
    };

    auto val = unival { vector<int8_t> { 1, 2, 3 } };
    REQUIRE(compare_bytes(val.get_bytes().value(),
                          vector<int8_t> { 1, 2, 3 }));
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

  TEST_CASE("create byte array from span") {
    auto compare_bytes = [](auto left, auto right) -> bool {
      return std::equal(left.begin(), left.end(), right.begin(),
                        right.end());
    };

    auto vec = vector<int8_t> { 1, 2, 3 };
    auto val =
        unival { span<int8_t const> { vec.begin(), vec.end() } };
    REQUIRE(compare_bytes(val.get_bytes().value(), vec));
  }

  TEST_CASE("create unival from vector") {
    auto vec =
        vector<unival> { unival { 1 }, unival { 2 }, unival { 3 } };
    auto val = unival { vec };
  }

  TEST_CASE("can get unival vector value") {
    auto vec = vector<unival> { unival { 42 } };
    auto val = unival { vec };

    REQUIRE(val.get(0).value().get_integer() == 42);
  }

  TEST_CASE("can not get unival vector out of bounds value") {
    auto vec = vector<unival> { unival { 42 } };
    auto val = unival { vec };

    REQUIRE(!val.get(-1).has_value());
    REQUIRE(!val.get(1).has_value());
  }

  TEST_CASE("can not get unival vector value from int") {
    auto val = unival { 42 };

    REQUIRE(!val.get(0).has_value());
  }

  TEST_CASE("can get unival 1-vector size") {
    auto vec = vector<unival> { unival { 42 } };
    auto val = unival { vec };

    REQUIRE(val.get_size() == 1);
  }

  TEST_CASE("can get unival 3-vector size") {
    auto vec =
        vector<unival> { unival { 1 }, unival { 2 }, unival { 3 } };
    auto val = unival { vec };

    REQUIRE(val.get_size() == 3);
  }

  TEST_CASE("can not get unival vector size from int") {
    auto val = unival { 42 };

    REQUIRE(!val.get_size().has_value());
  }

  TEST_CASE("is vector when created from vector") {
    auto vec =
        vector<unival> { unival { 1 }, unival { 2 }, unival { 3 } };
    auto val = unival { vec };
    REQUIRE(val.is_vector());
  }

  TEST_CASE("is not vector when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.is_vector());
  }

  TEST_CASE("create unival from true") {
    auto val = unival { true };
    REQUIRE(val.get_boolean() == true);
  }

  TEST_CASE("create unival from false") {
    auto val = unival { false };
    REQUIRE(val.get_boolean() == false);
  }

  TEST_CASE("is boolean when created from true") {
    auto val = unival { true };
    REQUIRE(val.is_boolean());
  }

  TEST_CASE("is not boolean when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.is_boolean());
  }

  TEST_CASE("can not get boolean from int") {
    auto val = unival { 42 };
    REQUIRE(!val.get_boolean().has_value());
  }

  TEST_CASE("compare unival ints") {
    auto val1 = unival { 42 };
    auto val2 = unival { 42 };
    auto val3 = unival { 43 };

    REQUIRE(val1 == val2);
    REQUIRE(val1 != val3);
    REQUIRE(val1 <= val2);
    REQUIRE(val1 < val3);
    REQUIRE(val3 >= val2);
    REQUIRE(val3 > val1);
  }

  TEST_CASE("create 1-composite unival") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } } } };
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
  }

  TEST_CASE("create 3-composite unival") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } } } };
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("create 3-composite unival unsorted") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 5 }, unival { 6 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 1 }, unival { 2 } } } };
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("compare unival different types") {
    auto a = unival {};
    auto b = unival { true };
    auto c = unival { 42 };
    auto d = unival { 3.14 };
    auto e = unival { "test" };
    auto f = unival { vector<int8_t> { 1, 2, 3 } };
    auto g = unival { vector<unival> { unival { 1 }, unival { 2 } } };
    auto h = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } } } };

    REQUIRE(a != b);
    REQUIRE(a != c);
    REQUIRE(a != d);
    REQUIRE(a != e);
    REQUIRE(a != f);
    REQUIRE(a != g);
    REQUIRE(a != h);

    REQUIRE(a < b);
    REQUIRE(b < c);
    REQUIRE(c < d);
    REQUIRE(d < e);
    REQUIRE(e < f);
    REQUIRE(f < g);
    REQUIRE(g < h);
  }

  TEST_CASE("compare unival vectors") {
    auto foo =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    auto bar =
        unival { vector<unival> { unival { 3 }, unival { 4 } } };
    auto baz = unival { vector<unival> { unival { 3 } } };

    REQUIRE(foo != bar);
    REQUIRE(foo < bar);
    REQUIRE(foo < baz);
    REQUIRE(bar > baz);
  }

  TEST_CASE("is composite when created from composite") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } } } };
    REQUIRE(val.is_composite());
  }

  TEST_CASE("is not composite when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.is_composite());
  }

  TEST_CASE("can get size of composite unival") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } } } };
    REQUIRE(val.get_size() == 3);
  }

  TEST_CASE("create 3-composite unival repeating") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } },
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } } } };
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
    REQUIRE(val.get_size() == 3);
  }

  TEST_CASE("can not get composite element from int unival") {
    auto val = unival { 42 };
    REQUIRE(!val.get(unival { 1 }).has_value());
  }

  TEST_CASE("can not get out of bounds composite unival element") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } } } };
    REQUIRE(!val.get(unival { 2 }).has_value());
    REQUIRE(!val.get(unival { 7 }).has_value());
  }

  TEST_CASE("create unival from std::string") {
    auto str = string { "foo" };
    auto val = unival { str };
    REQUIRE(val.get_string().value() == u8string { u8"foo" });
  }

  TEST_CASE("create unival from std::u8string") {
    auto str = u8string { u8"foo" };
    auto val = unival { str };
    REQUIRE(val.get_string().value() == str);
  }

  TEST_CASE("set vector unival element") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    val = val.set(0, unival { 42 });
    REQUIRE(val.get_size() == 2);
    REQUIRE(val.get(0) == unival { 42 });
    REQUIRE(val.get(1) == unival { 2 });
  }

  TEST_CASE("can not set vector element of int unival") {
    auto val = unival { 42 };
    REQUIRE(!val.set(0, unival { 1 }).has_value());
  }

  TEST_CASE("can not set out of bounds vector unival element") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    REQUIRE(!val.set(-1, unival { 1 }).has_value());
    REQUIRE(!val.set(3, unival { 1 }).has_value());
  }

  TEST_CASE("can resize vector unival") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    val = val.resize(3, unival { 42 });
    REQUIRE(val.get_size() == 3);
    REQUIRE(val.get(0) == unival { 1 });
    REQUIRE(val.get(1) == unival { 2 });
    REQUIRE(val.get(2) == unival { 42 });
  }

  TEST_CASE("can not resize vector unival to negative size") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    REQUIRE(!val.resize(-1).has_value());
  }

  TEST_CASE("can not resize int unival") {
    auto val = unival { 42 };
    REQUIRE(!val.resize(1).has_value());
  }

  TEST_CASE("set composite unival element") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } } } };
    val = val.set(unival { 1 }, unival { 42 });
    REQUIRE(val.get_size() == 3);
    REQUIRE(val.get(unival { 1 }) == unival { 42 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("set composite unival element by new keys") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 5 }, unival { 6 } } } };
    val = val.set(unival { 3 }, unival { 4 });
    val = val.set(unival { 7 }, unival { 42 });
    REQUIRE(val.get_size() == 4);
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
    REQUIRE(val.get(unival { 7 }) == unival { 42 });
  }

  TEST_CASE("can not set composite element of int unival") {
    auto val = unival { 42 };
    REQUIRE(!val.set(unival { 1 }, unival { 1 }).has_value());
  }

  TEST_CASE("edit vector unival blank") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    val = val.edit().commit();
    REQUIRE(val.get_size() == 2);
    REQUIRE(val.get(0) == unival { 1 });
    REQUIRE(val.get(1) == unival { 2 });
  }

  TEST_CASE("edit vector unival element") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    val = val.edit().set(0, unival { 42 }).commit();
    REQUIRE(val.get_size() == 2);
    REQUIRE(val.get(0) == unival { 42 });
    REQUIRE(val.get(1) == unival { 2 });
  }

  TEST_CASE("edit 2 vector unival elements") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    val = val.edit()
              .set(0, unival { 42 })
              .set(1, unival { 43 })
              .commit();
    REQUIRE(val.get_size() == 2);
    REQUIRE(val.get(0) == unival { 42 });
    REQUIRE(val.get(1) == unival { 43 });
  }

  TEST_CASE("edit composite unival blank") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } } } };
    val = val.edit().commit();
    REQUIRE(val.get_size() == 3);
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("edit composite unival element") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } } } };
    val = val.edit().set(unival { 1 }, unival { 42 }).commit();
    REQUIRE(val.get_size() == 3);
    REQUIRE(val.get(unival { 1 }) == unival { 42 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("edit 2 composite unival elements") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } },
        pair { unival { 5 }, unival { 6 } } } };
    val = val.edit()
              .set(unival { 1 }, unival { 42 })
              .set(unival { 5 }, unival { 43 })
              .commit();
    REQUIRE(val.get_size() == 3);
    REQUIRE(val.get(unival { 1 }) == unival { 42 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 43 });
  }

  TEST_CASE("edit element of composite unival element") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { vector<unival> {
                                 unival { 1 }, unival { 2 } } } },
        pair { unival { 3 }, unival { 4 } } } };
    val = val.edit().on(unival { 1 }).set(1, unival { 42 }).commit();
    REQUIRE(val.get_size() == 2);
    REQUIRE(
        val.get(unival { 1 }) ==
        unival { vector<unival> { unival { 1 }, unival { 42 } } });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
  }

  TEST_CASE("edit element of vector unival element") {
    auto val = unival { vector<unival> {
        unival { vector<pair<unival, unival>> {
            pair { unival { 1 }, unival { 2 } },
            pair { unival { 3 }, unival { 4 } } } },
        unival { 2 } } };
    val = val.edit().on(0).set(unival { 3 }, unival { 42 }).commit();
    REQUIRE(val.get_size() == 2);
    REQUIRE(val.get(0) ==
            unival { vector<pair<unival, unival>> {
                pair { unival { 1 }, unival { 2 } },
                pair { unival { 3 }, unival { 42 } } } });
    REQUIRE(val.get(1) == unival { 2 });
  }

  TEST_CASE("can not edit out of bounds element of vector unival") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    REQUIRE(!val.edit().set(2, unival { 3 }).commit().has_value());
  }

  TEST_CASE("insert elements to composite unival") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } } } };
    val = val.edit()
              .set(unival { -1 }, unival { -2 })
              .set(unival { 2 }, unival { 3 })
              .set(unival { 5 }, unival { 6 })
              .commit();
    REQUIRE(val.get_size() == 5);
    REQUIRE(val.get(unival { -1 }) == unival { -2 });
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 2 }) == unival { 3 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("edit vector unival size") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    val = val.edit().resize(3, unival { 3 }).commit();
    REQUIRE(val.get_size() == 3);
    REQUIRE(val.get(0) == unival { 1 });
    REQUIRE(val.get(1) == unival { 2 });
    REQUIRE(val.get(2) == unival { 3 });
  }

  TEST_CASE("edit size of vector unival element") {
    auto val = unival { vector<unival> {
        unival { vector<unival> { unival { 1 }, unival { 2 } } },
        unival { 2 } } };
    val = val.edit().on(0).resize(3, unival { 42 }).commit();
    REQUIRE(val ==
            unival { vector<unival> {
                unival { vector<unival> { unival { 1 }, unival { 2 },
                                          unival { 42 } } },
                unival { 2 } } });
  }

  TEST_CASE("edit size of composite unival element") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { vector<unival> {
                                 unival { 1 }, unival { 2 } } } },
        pair { unival { 3 }, unival { 4 } } } };
    val =
        val.edit().on(unival { 1 }).resize(3, unival { 42 }).commit();
    REQUIRE(val ==
            unival { vector<pair<unival, unival>> {
                pair { unival { 1 }, unival { vector<unival> {
                                         unival { 1 }, unival { 2 },
                                         unival { 42 } } } },
                pair { unival { 3 }, unival { 4 } } } });
  }

  TEST_CASE("edit two vector elements of vector unival") {
    auto val = unival { vector<unival> {
        unival { vector<unival> { unival { 1 }, unival { 2 } } },
        unival { vector<unival> { unival { 3 }, unival { 4 } } } } };
    val = val.edit()
              .on(0)
              .set(1, unival { 42 })
              .on(1)
              .set(0, unival { 43 })
              .commit();
    REQUIRE(
        val ==
        unival { vector<unival> {
            unival { vector<unival> { unival { 1 }, unival { 42 } } },
            unival {
                vector<unival> { unival { 43 }, unival { 4 } } } } });
  }

  TEST_CASE("can not edit vector element of composite unival") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } } } };
    REQUIRE(!val.edit().set(0, unival { 42 }).commit().has_value());
  }

  TEST_CASE("can not edit composite element of vector unival") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    REQUIRE(!val.edit()
                 .set(unival { 0 }, unival { 42 })
                 .commit()
                 .has_value());
  }

  TEST_CASE("can not edit and resize int unival") {
    auto val = unival { 1 };
    REQUIRE(
        !val.edit().resize(2, unival { 42 }).commit().has_value());
  }

  TEST_CASE("can not edit int unival on cursor") {
    auto val = unival { 1 };
    REQUIRE(!val.edit()
                 .on(unival { 0 })
                 .set(0, unival { 42 })
                 .commit()
                 .has_value());
    REQUIRE(
        !val.edit().on(0).set(0, unival { 42 }).commit().has_value());
  }

  TEST_CASE("can not resize composite unival") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } } } };
    REQUIRE(
        !val.edit().resize(3, unival { 42 }).commit().has_value());
  }

  TEST_CASE(
      "can not edit and resize vector unival to negative size") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    REQUIRE(
        !val.edit().resize(-1, unival { 42 }).commit().has_value());
  }

  TEST_CASE("can not edit vector unival on out of bounds cursor") {
    auto val =
        unival { vector<unival> { unival { 1 }, unival { 2 } } };
    REQUIRE(!val.edit()
                 .on(-1)
                 .set(0, unival { 42 })
                 .commit()
                 .has_value());
    REQUIRE(
        !val.edit().on(2).set(0, unival { 42 }).commit().has_value());
  }

  TEST_CASE("can not edit composite unival on out of bounds cursor") {
    auto val = unival { vector<pair<unival, unival>> {
        pair { unival { 1 }, unival { 2 } },
        pair { unival { 3 }, unival { 4 } } } };
    REQUIRE(!val.edit()
                 .on(unival { 2 })
                 .set(0, unival { 42 })
                 .commit()
                 .has_value());
    REQUIRE(!val.edit()
                 .on(unival { -1 })
                 .set(0, unival { 42 })
                 .commit()
                 .has_value());
    REQUIRE(!val.edit()
                 .on(unival { 4 })
                 .set(0, unival { 42 })
                 .commit()
                 .has_value());
  }
}
