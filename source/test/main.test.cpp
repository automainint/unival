/*  Copyright (c) 2022 Mitya Selivanov
 *
 *  This file is part of the Laplace project.
 *
 *  Laplace is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty
 *  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
 *  the MIT License for more details.
 */

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

auto main(int argc, char **argv) -> int {
  return Catch::Session().run(argc, argv);
}
