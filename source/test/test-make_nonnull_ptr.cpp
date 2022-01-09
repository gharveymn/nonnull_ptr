/** test-make_nonnull_ptr.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "test_common.hpp"

int
main (void)
{
  int x = 1;
  const int y = 2;
  gch::nonnull_ptr<int>       rx = gch::make_nonnull_ptr (x);
  gch::nonnull_ptr<const int> ry = gch::make_nonnull_ptr (y);
  CHECK (rx != ry);

  return 0;
}
