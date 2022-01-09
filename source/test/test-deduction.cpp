/** test-deduction.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "test_common.hpp"

int
main (void)
{
#ifdef GCH_CTAD_SUPPORT
  int  x = 1;
  long y = 2;

  gch::nonnull_ptr rx (x);
  gch::nonnull_ptr ry (y);
#endif

  return 0;
}
