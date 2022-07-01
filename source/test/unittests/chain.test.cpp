/*  Copyright (c) 2022 Mitya Selivanov
 */

#include "../../unival/unival.h"
#include "catch2/catch.hpp"
#include <vector>

namespace unival::test {
  using std::optional;

  TEST_CASE("edit unival on root cursor") {
    auto val = unival { 42 };
    REQUIRE(val.edit().set(unival { 42.f }).commit() ==
            unival { 42.f });
  }

  TEST_CASE("edit vector unival blank") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.edit().commit();
    REQUIRE(val.size() == 2);
    REQUIRE(val.get(0) == unival { 1 });
    REQUIRE(val.get(1) == unival { 2 });
  }

  TEST_CASE("edit vector unival element") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.edit().on(0).set(unival { 42 }).commit();
    REQUIRE(val.size() == 2);
    REQUIRE(val.get(0) == unival { 42 });
    REQUIRE(val.get(1) == unival { 2 });
  }

  TEST_CASE("edit 2 vector unival elements") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.edit()
              .on(0)
              .set(unival { 42 })
              .on(1)
              .set(unival { 43 })
              .commit();
    REQUIRE(val.size() == 2);
    REQUIRE(val.get(0) == unival { 42 });
    REQUIRE(val.get(1) == unival { 43 });
  }

  TEST_CASE("edit composite unival blank") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } } } };
    val = val.edit().commit();
    REQUIRE(val.size() == 3);
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("edit composite unival element") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } } } };
    val = val.edit().on(unival { 1 }).set(unival { 42 }).commit();
    REQUIRE(val.size() == 3);
    REQUIRE(val.get(unival { 1 }) == unival { 42 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("edit 2 composite unival elements") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } },
                             { unival { 5 }, unival { 6 } } } };
    val = val.edit()
              .on(unival { 1 })
              .set(unival { 42 })
              .on(unival { 5 })
              .set(unival { 43 })
              .commit();
    REQUIRE(val.size() == 3);
    REQUIRE(val.get(unival { 1 }) == unival { 42 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 43 });
  }

  TEST_CASE("edit element of composite unival element") {
    auto val = unival { composite {
        { unival { 1 },
          unival { vector { unival { 1 }, unival { 2 } } } },
        { unival { 3 }, unival { 4 } } } };
    val =
        val.edit().on(unival { 1 }).on(1).set(unival { 42 }).commit();
    REQUIRE(val.size() == 2);
    REQUIRE(val.get(unival { 1 }) ==
            unival { vector { unival { 1 }, unival { 42 } } });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
  }

  TEST_CASE("edit element of vector unival element") {
    auto val = unival { vector {
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } },
        unival { 2 } } };
    val =
        val.edit().on(0).on(unival { 3 }).set(unival { 42 }).commit();
    REQUIRE(val.size() == 2);
    REQUIRE(val.get(0) ==
            unival { composite { { unival { 1 }, unival { 2 } },
                                 { unival { 3 }, unival { 42 } } } });
    REQUIRE(val.get(1) == unival { 2 });
  }

  TEST_CASE("can not edit out of bounds element of vector unival") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(val.edit().on(2).set(unival { 3 }).commit().error());
  }

  TEST_CASE("insert elements to composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    val = val.edit()
              .on(unival { -1 })
              .set(unival { -2 })
              .on(unival { 2 })
              .set(unival { 3 })
              .on(unival { 5 })
              .set(unival { 6 })
              .commit();
    REQUIRE(val.size() == 5);
    REQUIRE(val.get(unival { -1 }) == unival { -2 });
    REQUIRE(val.get(unival { 1 }) == unival { 2 });
    REQUIRE(val.get(unival { 2 }) == unival { 3 });
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
    REQUIRE(val.get(unival { 5 }) == unival { 6 });
  }

  TEST_CASE("edit vector unival size") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.edit().resize(3, unival { 3 }).commit();
    REQUIRE(val.size() == 3);
    REQUIRE(val.get(0) == unival { 1 });
    REQUIRE(val.get(1) == unival { 2 });
    REQUIRE(val.get(2) == unival { 3 });
  }

  TEST_CASE("edit size of vector unival element") {
    auto val = unival { vector {
        unival { vector { unival { 1 }, unival { 2 } } },
        unival { 2 } } };
    val = val.edit().on(0).resize(3, unival { 42 }).commit();
    REQUIRE(val == unival { vector {
                       unival { vector { unival { 1 }, unival { 2 },
                                         unival { 42 } } },
                       unival { 2 } } });
  }

  TEST_CASE("edit size of composite unival element") {
    auto val = unival { composite {
        { unival { 1 },
          unival { vector { unival { 1 }, unival { 2 } } } },
        { unival { 3 }, unival { 4 } } } };
    val =
        val.edit().on(unival { 1 }).resize(3, unival { 42 }).commit();
    REQUIRE(val == unival { composite {
                       { unival { 1 },
                         unival { vector { unival { 1 }, unival { 2 },
                                           unival { 42 } } } },
                       { unival { 3 }, unival { 4 } } } });
  }

  TEST_CASE("edit two vector elements of vector unival") {
    auto val = unival { vector {
        unival { vector { unival { 1 }, unival { 2 } } },
        unival { vector { unival { 3 }, unival { 4 } } } } };
    val = val.edit()
              .on(0)
              .on(1)
              .set(unival { 42 })
              .on(1)
              .on(0)
              .set(unival { 43 })
              .commit();
    REQUIRE(
        val ==
        unival { vector {
            unival { vector { unival { 1 }, unival { 42 } } },
            unival { vector { unival { 43 }, unival { 4 } } } } });
  }

  TEST_CASE("can not edit vector element of composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    REQUIRE(val.edit().on(0).set(unival { 42 }).commit().error());
    REQUIRE(
        val.edit().on(0).on(0).set(unival { 42 }).commit().error());
  }

  TEST_CASE("can not edit composite element of vector unival") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(val.edit()
                .on(unival { 0 })
                .set(unival { 42 })
                .commit()
                .error());
    REQUIRE(val.edit()
                .on(unival { 0 })
                .on(unival { 0 })
                .set(unival { 42 })
                .commit()
                .error());
  }

  TEST_CASE("can not edit and resize int unival") {
    auto val = unival { 1 };
    REQUIRE(val.edit().resize(2, unival { 42 }).commit().error());
  }

  TEST_CASE("can not edit int unival on cursor") {
    auto val = unival { 1 };
    REQUIRE(val.edit()
                .on(unival { 0 })
                .set(unival { 42 })
                .commit()
                .error());
    REQUIRE(val.edit().on(0).set(unival { 42 }).commit().error());
  }

  TEST_CASE("can not resize composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    REQUIRE(val.edit().resize(3, unival { 42 }).commit().error());
  }

  TEST_CASE(
      "can not edit and resize vector unival to negative size") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(val.edit().resize(-1, unival { 42 }).commit().error());
  }

  TEST_CASE("can not edit vector unival on out of bounds cursor") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(
        val.edit().on(-1).on(0).set(unival { 42 }).commit().error());
    REQUIRE(
        val.edit().on(2).on(0).set(unival { 42 }).commit().error());
  }

  TEST_CASE("can not edit composite unival on out of bounds cursor") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    REQUIRE(val.edit()
                .on(unival { 2 })
                .on(unival { 0 })
                .set(unival { 42 })
                .commit()
                .error());
    REQUIRE(val.edit()
                .on(unival { -1 })
                .on(unival { 0 })
                .set(unival { 42 })
                .commit()
                .error());
    REQUIRE(val.edit()
                .on(unival { 4 })
                .on(unival { 0 })
                .set(unival { 42 })
                .commit()
                .error());
  }

  TEST_CASE("can not edit and remove vector element of int unival") {
    auto val = unival { 1 };
    REQUIRE(val.edit().on(0).remove().commit().error());
  }

  TEST_CASE("can edit and remove element of vector unival") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.edit().on(0).remove().commit();
    REQUIRE(val.size() == 1);
    REQUIRE(val.get(0) == unival { 2 });
  }

  TEST_CASE("can not edit and remove out of bounds element of vector "
            "unival") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    REQUIRE(val.edit().on(2).remove().commit().error());
    REQUIRE(val.edit().on(-1).remove().commit().error());
  }

  TEST_CASE(
      "can not edit and remove composite element of int unival") {
    auto val = unival { 1 };
    REQUIRE(val.edit().on(unival { 0 }).remove().commit().error());
  }

  TEST_CASE("can edit and remove element of composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    val = val.edit().on(unival { 1 }).remove().commit();
    REQUIRE(val.size() == 1);
    REQUIRE(val.get(unival { 3 }) == unival { 4 });
  }

  TEST_CASE("can not edit and remove out of bounds element of "
            "composite unival") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    REQUIRE(val.edit().on(unival { 2 }).remove().commit().error());
    REQUIRE(val.edit().on(unival { -1 }).remove().commit().error());
    REQUIRE(val.edit().on(unival { 4 }).remove().commit().error());
  }

  TEST_CASE(
      "can not edit and remove element on root cursor position") {
    auto val = unival { 1 };
    REQUIRE(val.edit().remove().commit().error());
  }

  TEST_CASE("edit and resize vector will reset cursor position") {
    auto val = unival { vector { unival { 1 }, unival { 2 } } };
    val = val.edit()
              .resize(3, unival { 2 })
              .on(2)
              .set(unival { 3 })
              .commit();
    REQUIRE(val == unival { vector { unival { 1 }, unival { 2 },
                                     unival { 3 } } });
  }

  TEST_CASE(
      "edit and remove vector element will reset cursor position") {
    auto val = unival { vector { unival { 1 }, unival { 2 },
                                 unival { 3 } } };
    val = val.edit().on(1).remove().on(1).set(unival { 2 }).commit();
    REQUIRE(val == unival { vector { unival { 1 }, unival { 2 } } });
  }

  TEST_CASE("edit and remove composite element will reset cursor "
            "position") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    val = val.edit()
              .on(unival { 1 })
              .remove()
              .on(unival { 5 })
              .set(unival { 6 })
              .commit();
    REQUIRE(val ==
            unival { composite { { unival { 3 }, unival { 4 } },
                                 { unival { 5 }, unival { 6 } } } });
  }

  TEST_CASE("can edit empty unival as vector") {
    auto val = unival {};
    val = val.edit().resize(3, unival { 1 }).commit();
    REQUIRE(val == unival { vector { unival { 1 }, unival { 1 },
                                     unival { 1 } } });
  }

  TEST_CASE("can edit empty unival as composite") {
    auto val = unival {};
    val = val.edit().on(unival { 0 }).set(unival { 1 }).commit();
    REQUIRE(val ==
            unival { composite { { unival { 0 }, unival { 1 } } } });
  }

  TEST_CASE("edit and remove element of nested composite") {
    auto val = unival { composite {
        { unival { 1 }, unival { 2 } },
        { unival { 3 }, unival { composite {
                            { unival { 4 }, unival { 5 } },
                            { unival { 6 }, unival { 7 } } } } } } };
    val = val.edit()
              .on(unival { 3 })
              .on(unival { 4 })
              .remove()
              .commit();
    REQUIRE(val ==
            unival { composite {
                { unival { 1 }, unival { 2 } },
                { unival { 3 },
                  unival { composite {
                      { unival { 6 }, unival { 7 } } } } } } });
  }

  TEST_CASE("can not edit and remove out of bounds element of nested "
            "composite") {
    auto val = unival { composite {
        { unival { 1 }, unival { 2 } },
        { unival { 3 }, unival { composite {
                            { unival { 4 }, unival { 5 } },
                            { unival { 6 }, unival { 7 } } } } } } };
    REQUIRE(val.edit()
                .on(unival { 2 })
                .on(unival { 4 })
                .remove()
                .commit()
                .error());
    REQUIRE(val.edit()
                .on(unival { 3 })
                .on(unival { 3 })
                .remove()
                .commit()
                .error());
    REQUIRE(val.edit()
                .on(unival { 3 })
                .on(unival { 7 })
                .remove()
                .commit()
                .error());
  }

  TEST_CASE("edit and remove element of nested vector") {
    auto val = unival { vector {
        unival { 1 },
        unival { vector { unival { 2 }, unival { 3 } } } } };
    val = val.edit().on(1).on(0).remove().commit();
    REQUIRE(val ==
            unival { vector { unival { 1 },
                              unival { vector { unival { 3 } } } } });
  }

  TEST_CASE("can not edit and remove out of bounds element of nested "
            "vector") {
    auto val = unival { vector {
        unival { 1 },
        unival { vector { unival { 2 }, unival { 3 } } } } };
    REQUIRE(val.edit().on(-1).on(0).remove().commit().error());
    REQUIRE(val.edit().on(1).on(-1).remove().commit().error());
    REQUIRE(val.edit().on(1).on(3).remove().commit().error());
  }

  TEST_CASE("edit on integer key") {
    auto val =
        unival { composite { { unival { 1 }, unival { 2 } },
                             { unival { 3 }, unival { 4 } } } };
    val = val.edit().on_key(1).set(unival { 5 }).commit();
    REQUIRE(val ==
            unival { composite { { unival { 1 }, unival { 5 } },
                                 { unival { 3 }, unival { 4 } } } });
  }

  TEST_CASE("edit on ASCII string key") {
    auto val =
        unival { composite { { unival { "a" }, unival { "b" } },
                             { unival { "c" }, unival { "d" } } } };
    val = val.edit().on("c").set(unival { "e" }).commit();
    REQUIRE(val == unival { composite {
                       { unival { "a" }, unival { "b" } },
                       { unival { "c" }, unival { "e" } } } });
  }

  TEST_CASE("edit on UTF-8 string key") {
    auto val = unival { composite {
        { unival { u8"a" }, unival { u8"b" } },
        { unival { u8"c" }, unival { u8"d" } } } };
    val = val.edit().on(u8"c").set(unival { u8"e" }).commit();
    REQUIRE(val == unival { composite {
                       { unival { u8"a" }, unival { u8"b" } },
                       { unival { u8"c" }, unival { u8"e" } } } });
  }
}
