/** test-assign.cpp
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
  gch::nonnull_ptr<int> r { x };
  CHECK (*r == 1);
  *r = 2;
  CHECK (*r == 2);
  CHECK (x == 2);

  int y = 3;
  CHECK (r.emplace (y) == 3);
  CHECK (*r == 3);
  CHECK (y == 3);

  return 0;
}
