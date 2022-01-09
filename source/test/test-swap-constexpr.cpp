/** test-swap-constexpr.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "test_common.hpp"

static constexpr
bool
test (void)
{
#ifdef GCH_HAS_CPP14_CONSTEXPR
  int x = 1;
  int y = 2;
  gch::nonnull_ptr<int> rx { x };
  gch::nonnull_ptr<int> ry { y };
  gch::nonnull_ptr<int> rz { x };

  bool t1 = (rx != ry) && (rx == rz);

  using std::swap;
  swap (ry, rz);

  bool t2 = (rx == ry) && (rx != rz);

  return t1 && t2;
#else
  return true;
#endif
}

static_assert (test (), "failed swap");

int
main (void)
{
  return 0;
}
