/** test-const.cpp
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
  int y = 2;
  const int z = 1;

  gch::nonnull_ptr<int> rx { x };
  gch::nonnull_ptr<const int> rz = std::pointer_traits<gch::nonnull_ptr<const int>>::pointer_to (z);

  CHECK (*rx == *rz);
  CHECK (rx != rz);

  // set rz with a non-const reference
  rz.emplace (y);

  CHECK (*rx != *rz);
  CHECK (rx != rz);

  rz.emplace (x);

  CHECK (*rx == *rz);
  CHECK (rx == rz);

  return 0;
}
