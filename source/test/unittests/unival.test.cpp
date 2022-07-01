/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/unival.h"
#include "catch2/catch.hpp"
#include <vector>

namespace unival::test {
  using std::u8string_view, std::span, std::string, std::u8string,
      std::optional;

  TEST_CASE("unival created") { auto val = unival {}; }

  TEST_CASE("unival is empty by default") {
    auto val = unival {};
    REQUIRE(val.empty());
  }

  TEST_CASE("unival is not _error by default") {
    auto val = unival {};
    REQUIRE(!val.error());
  }

  TEST_CASE("unival is not empty when assigned to a value") {
    auto val = unival { 42 };
    REQUIRE(!val.empty());
  }

  TEST_CASE("unival is not _error when assigned to a value") {
    auto val = unival { 42 };
    REQUIRE(!val.error());
  }

  TEST_CASE("create unival from char") {
    auto val = unival { static_cast<signed char>(-42) };
    REQUIRE(val.integer() == -42);
  }

  TEST_CASE("create unival from short") {
    auto val = unival { static_cast<signed short>(-4242) };
    REQUIRE(val.integer() == -4242);
  }

  TEST_CASE("create unival from int") {
    auto val = unival { static_cast<signed int>(-424242) };
    REQUIRE(val.integer() == -424242);
  }

  TEST_CASE("create unival from long") {
    auto val = unival { static_cast<signed long>(-424242) };
    REQUIRE(val.integer() == -424242);
  }

  TEST_CASE("create unival from long long") {
    auto val = unival { static_cast<signed long long>(-4242424242) };
    REQUIRE(val.integer() == -4242424242);
  }

  TEST_CASE("create unival from unsigned char") {
    auto val = unival { static_cast<unsigned char>(42) };
    REQUIRE(val.uint() == 42);
  }

  TEST_CASE("create unival from unsigned short") {
    auto val = unival { static_cast<unsigned short>(4242) };
    REQUIRE(val.uint() == 4242);
  }

  TEST_CASE("create unival from unsigned int") {
    auto val = unival { static_cast<unsigned int>(424242) };
    REQUIRE(val.uint() == 424242);
  }

  TEST_CASE("create unival from unsigned long") {
    auto val = unival { static_cast<unsigned long>(424242) };
    REQUIRE(val.uint() == 424242);
  }

  TEST_CASE("create unival from unsigned long long") {
    auto val = unival { static_cast<unsigned long long>(4242424242) };
    REQUIRE(val.uint() == 4242424242);
  }

  TEST_CASE("create unival from unsigned long long extreme "
            "value") {
    auto val = unival { static_cast<unsigned long long>(
        18446744073709551615ull) };
    REQUIRE(val.uint() == 18446744073709551615ull);
  }

  TEST_CASE("create unival from float") {
    auto val = unival { 42.42f };
    REQUIRE(val.real() == Approx(42.42f));
  }

  TEST_CASE("create unival from double") {
    auto val = unival { 424242.42 };
    REQUIRE(val.real() == Approx(424242.42));
  }

  TEST_CASE("get uint from int") {
    auto val = unival { 42 };
    REQUIRE(val.uint() == 42);
  }

  TEST_CASE("get int from uint") {
    auto val = unival { 42u };
    REQUIRE(val.integer() == 42);
  }

  TEST_CASE("can not get float from int") {
    auto val = unival { 42 };
    REQUIRE(!val.real().has_value());
  }

  TEST_CASE("can not get float from uint") {
    auto val = unival { 42u };
    REQUIRE(!val.real().has_value());
  }

  TEST_CASE("can not get int from float") {
    auto val = unival { 42.42f };
    REQUIRE(!val.integer().has_value());
  }

  TEST_CASE("can not get uint from float") {
    auto val = unival { 42.42f };
    REQUIRE(!val.uint().has_value());
  }

  TEST_CASE("is integer when created from int") {
    auto val = unival { 42 };
    REQUIRE(val.integer());
  }

  TEST_CASE("is integer when created from uint") {
    auto val = unival { 42u };
    REQUIRE(val.integer());
  }

  TEST_CASE("is not integer when created from float") {
    auto val = unival { 42.42f };
    REQUIRE(!val.integer());
  }

  TEST_CASE("is float when created from float") {
    auto val = unival { 42.42f };
    REQUIRE(val.real());
  }

  TEST_CASE("is float when created from double") {
    auto val = unival { 42.42 };
    REQUIRE(val.real());
  }

  TEST_CASE("is not float when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.real());
  }

  TEST_CASE("create unival from string") {
    auto val = unival { "42" };
    REQUIRE(val.string() == u8string_view(u8"42"));
  }

  TEST_CASE("create unival from UTF-8 string") {
    auto val = unival { u8"42" };
    REQUIRE(val.string() == u8string_view(u8"42"));
  }

  TEST_CASE("can not get string from int") {
    auto val = unival { 42 };
    REQUIRE(!val.string().has_value());
  }

  TEST_CASE("is string when created from string") {
    auto val = unival { "42" };
    REQUIRE(val.string());
  }

  TEST_CASE("is not string when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.string());
  }

  TEST_CASE("create unival from byte array") {
    auto compare_bytes = [](auto left, auto right) -> bool {
      return std::equal(left.begin(), left.end(), right.begin(),
                        right.end());
    };

    auto val = unival { bytes { 1, 2, 3 } };
    REQUIRE(
        compare_bytes(val.bytes().value(), bytes { 1, 2, 3 }));
  }

  TEST_CASE("can not get byte array from int") {
    auto val = unival { 42 };
    REQUIRE(!val.bytes().has_value());
  }

  TEST_CASE("is byte array when created from byte array") {
    auto val = unival { bytes { 1, 2, 3 } };
    REQUIRE(val.bytes());
  }

  TEST_CASE("is not byte array when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.bytes());
  }

  TEST_CASE("create byte array from span") {
    auto compare_bytes = [](auto left, auto right) -> bool {
      return std::equal(left.begin(), left.end(), right.begin(),
                        right.end());
    };

    auto vec = bytes { 1, 2, 3 };
    auto val =
        unival { span<int8_t const> { vec.begin(), vec.end() } };
    REQUIRE(compare_bytes(val.bytes().value(), vec));
  }

  TEST_CASE("create unival from vector") {
    auto vec = vector { unival { 1 }, unival { 2 }, unival { 3 } };
    auto val = unival { vec };
  }

  TEST_CASE("can get unival vector value") {
    auto vec = vector { unival { 42 } };
    auto val = unival { vec };

    REQUIRE(val.get(0).integer() == 42);
  }

  TEST_CASE("can not get unival vector out of bounds value") {
    auto vec = vector { unival { 42 } };
    auto val = unival { vec };

    REQUIRE(val.get(-1).error());
    REQUIRE(val.get(1).error());
  }

  TEST_CASE("can not get unival vector value from int") {
    auto val = unival { 42 };

    REQUIRE(val.get(0).error());
  }

  TEST_CASE("can get unival 1-vector size") {
    auto vec = vector { unival { 42 } };
    auto val = unival { vec };

    REQUIRE(val.size() == 1);
  }

  TEST_CASE("can get unival 3-vector size") {
    auto vec = vector { unival { 1 }, unival { 2 }, unival { 3 } };
    auto val = unival { vec };

    REQUIRE(val.size() == 3);
  }

  TEST_CASE("size of unival int is zero") {
    auto val = unival { 42 };

    REQUIRE(val.size() == 0);
  }

  TEST_CASE("is vector when created from vector") {
    auto vec = vector { unival { 1 }, unival { 2 }, unival { 3 } };
    auto val = unival { vec };
    REQUIRE(val.vector());
  }

  TEST_CASE("is not vector when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.vector());
  }

  TEST_CASE("create unival from true") {
    auto val = unival { true };
    REQUIRE(val.boolean() == true);
  }

  TEST_CASE("create unival from false") {
    auto val = unival { false };
    REQUIRE(val.boolean() == false);
  }

  TEST_CASE("is boolean when created from true") {
    auto val = unival { true };
    REQUIRE(val.boolean());
  }

  TEST_CASE("is not boolean when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.boolean());
  }

  TEST_CASE("can not get boolean from int") {
    auto val = unival { 42 };
    REQUIRE(!val.boolean().has_value());
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
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } } } };
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
  }

  TEST_CASE("create 3-composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } } } };
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("create 3-composite unival unsorted") {
    auto val =
        unival { composite { { unival { 5 }, unival { 6 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 1 }, unival { 2 } } } };
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
    auto f = unival { bytes { 1, 2, 3 } };
    auto g = unival { vector { unival { 1 }, unival { 2 } } };
    auto h = unival { composite { { unival { 1 }, unival { 2 } },
                                  { unival { 3 }, unival { 4 } } } };

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
    auto foo = unival { vector { unival { 1 }, unival { 2 } } };
    auto bar = unival { vector { unival { 3 }, unival { 4 } } };
    auto baz = unival { vector { unival { 3 } } };

    REQUIRE(foo != bar);
    REQUIRE(foo < bar);
    REQUIRE(foo < baz);
    REQUIRE(bar > baz);
  }

  TEST_CASE("compare _error univals") {
    auto foo = unival { 1 }.get(0);
    auto bar = unival { 2 }.get(0);
    auto baz = unival { 42 };

    REQUIRE(foo == bar);
    REQUIRE(foo != baz);
  }

  TEST_CASE("is composite when created from composite") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } } } };
    REQUIRE(val.composite());
  }

  TEST_CASE("is not composite when created from int") {
    auto val = unival { 42 };
    REQUIRE(!val.composite());
  }

  TEST_CASE("can get size of composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } } } };
    REQUIRE(val.size() == 3);
  }

  TEST_CASE("create 3-composite unival repeating") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } },
                             { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } } } };
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
    REQUIRE(val.size() == 3);
  }

  TEST_CASE("can not get composite element from int unival") {
    auto val = unival { 42 };
    REQUIRE(val.get(unival { 1 }).error());
  }

  TEST_CASE("can not get out of bounds composite unival element") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } } } };
    REQUIRE(val.get(unival { 2 }).error());
    REQUIRE(val.get(unival { 7 }).error());
  }

  TEST_CASE("create unival from std::string") {
    auto str = string { "foo" };
    auto val = unival { str };
    REQUIRE(val.string().value() == u8string { u8"foo" });
  }

  TEST_CASE("create unival from std::u8string") {
    auto str = u8string { u8"foo" };
    auto val = unival { str };
    REQUIRE(val.string().value() == str);
  }

  TEST_CASE("set vector unival element") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.set(0, unival { 42 });
    REQUIRE(val.size() == 2);
    REQUIRE(val.get(0) == unival { 42 });
    REQUIRE(val.get(1) == unival { 2 });
  }

  TEST_CASE("can not set vector element of int unival") {
    auto val = unival { 42 };
    REQUIRE(val.set(0, unival { 1 }).error());
  }

  TEST_CASE("can not set out of bounds vector unival element") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(val.set(-1, unival { 1 }).error());
    REQUIRE(val.set(3, unival { 1 }).error());
  }

  TEST_CASE("can resize vector unival") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.resize(3, unival { 42 });
    REQUIRE(val.size() == 3);
    REQUIRE(val.get(0) == unival { 1 });
    REQUIRE(val.get(1) == unival { 2 });
    REQUIRE(val.get(2) == unival { 42 });
  }

  TEST_CASE("can not resize vector unival to negative size") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(val.resize(-1).error());
  }

  TEST_CASE("can not resize int unival") {
    auto val = unival { 42 };
    REQUIRE(val.resize(1).error());
  }

  TEST_CASE("set composite unival element") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } } } };
    val = val.set(unival { 1 }, unival { 42 });
    REQUIRE(val.size() == 3);
    REQUIRE(val.get(unival { 1 }) == unival { 42 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("set composite unival element by new keys") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 5 }, unival { 6 } } } };
    val = val.set(unival { 3 }, unival { 4 });
    val = val.set(unival { 7 }, unival { 42 });
    REQUIRE(val.size() == 4);
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
    REQUIRE(val.get(unival { 7 }) == unival { 42 });
  }

  TEST_CASE("can not set composite element of int unival") {
    auto val = unival { 42 };
    REQUIRE(val.set(unival { 1 }, unival { 1 }).error());
  }

  TEST_CASE("can not remove vector element of int unival") {
    auto val = unival { 1 };
    REQUIRE(val.remove(0).error());
  }

  TEST_CASE("can remove element of vector unival") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.remove(0);
    REQUIRE(val.size() == 1);
    REQUIRE(val.get(0) == unival { 2 });
  }

  TEST_CASE("can not remove out of bounds element of vector unival") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(val.remove(2).error());
    REQUIRE(val.remove(-1).error());
  }

  TEST_CASE("can not remove composite element of int unival") {
    auto val = unival { 1 };
    REQUIRE(val.remove(unival { 0 }).error());
  }

  TEST_CASE("can remove element of composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    val = val.remove(unival { 1 });
    REQUIRE(val.size() == 1);
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
  }

  TEST_CASE(
      "can not remove out of bounds element of composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    REQUIRE(val.remove(unival { 2 }).error());
    REQUIRE(val.remove(unival { -1 }).error());
    REQUIRE(val.remove(unival { 4 }).error());
  }

  TEST_CASE("can use empty unival as vector") {
    auto val = unival {};
    val = val.resize(3, unival { 1 });
    REQUIRE(val == unival { vector { unival { 1 }, unival { 1 },
                                     unival { 1 } } });
  }

  TEST_CASE("can use empty unival as composite") {
    auto val = unival {};
    val = val.set(unival { 0 }, unival { 1 });
    REQUIRE(val ==
            unival { composite { { unival { 0 }, unival { 1 } } } });
  }

  TEST_CASE("access operator by index") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(val[0] == unival { 1 });
    REQUIRE(val[1] == unival { 2 });
  }

  TEST_CASE("access operator by key") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    REQUIRE(val[unival { 1 }] == unival { 2 });
    REQUIRE(val[unival { 3 }] == unival { 4 });
  }

  TEST_CASE("access operator by ASCII string key") {
    auto val =
        unival { composite { { unival { "foo" }, unival { 2 } },
                             { unival { "bar" }, unival { 4 } } } };
    REQUIRE(val["foo"] == unival { 2 });
    REQUIRE(val["bar"] == unival { 4 });
  }

  TEST_CASE("access operator by UTF-8 string key") {
    auto val =
        unival { composite { { unival { u8"foo" }, unival { 2 } },
                             { unival { u8"bar" }, unival { 4 } } } };
    REQUIRE(val[u8"foo"] == unival { 2 });
    REQUIRE(val[u8"bar"] == unival { 4 });
  }

  TEST_CASE("get by int key") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    REQUIRE(val.get_by_key(1) == unival { 2 });
    REQUIRE(val.get_by_key(3) == unival { 4 });
  }

  TEST_CASE("set by int key") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    val = val.set_by_key(1, unival { 5 });
    REQUIRE(val.get_by_key(1) == unival { 5 });
  }

  TEST_CASE("remove by int key") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    val = val.remove_by_key(1);
    REQUIRE(val.size() == 1);
    REQUIRE(val.get_by_key(3) == unival { 4 });
  }
}
