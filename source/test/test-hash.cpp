/** test-hash.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "test_common.hpp"

#include <unordered_map>
#include <string>

int
main (void)
{
  std::unordered_map<gch::nonnull_ptr<int>, const std::string *> map { };
  int x = 1;
  int y = 2;
  int z = 3;
  std::string xs = "x";
  std::string ys = "y";
  std::string zs = "z";
  map.emplace (x, &xs);
  map.emplace (y, &ys);
  map.emplace (z, &zs);

  CHECK (&xs == map[gch::nonnull_ptr<int> { x }]);
  CHECK (&ys == map[gch::make_nonnull_ptr (y)]);
  CHECK (&zs == map[gch::make_nonnull_ptr (z)]);

  return 0;
}
