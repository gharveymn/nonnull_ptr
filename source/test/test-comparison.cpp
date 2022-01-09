/** test-comparison.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "test_common.hpp"

int
main (void)
{
  int a[2] = { 11, 22 };
  int& x = a[0];
  int& y = a[1];

  const gch::nonnull_ptr<int> rx (x);
  gch::nonnull_ptr<const int> ry (y);

  // disparate type comparisons (not equal)
  CHECK (! (rx == ry));
  CHECK (! (ry == rx));
  CHECK (  (rx != ry));
  CHECK (  (ry != rx));
  CHECK (  (rx <  ry));
  CHECK (! (ry <  rx));
  CHECK (! (rx >  ry));
  CHECK (  (ry >  rx));
  CHECK (  (rx <= ry));
  CHECK (! (ry <= rx));
  CHECK (! (rx >= ry));
  CHECK (  (ry >= rx));

  // disparate type comparisons (equal)
  ry = gch::make_nonnull_ptr (a[0]);
  CHECK (  (rx == ry));
  CHECK (  (ry == rx));
  CHECK (! (rx != ry));
  CHECK (! (ry != rx));
  CHECK (! (rx <  ry));
  CHECK (! (ry <  rx));
  CHECK (! (rx >  ry));
  CHECK (! (ry >  rx));
  CHECK (  (rx <= ry));
  CHECK (  (ry <= rx));
  CHECK (  (rx >= ry));
  CHECK (  (ry >= rx));

  // nullopt comparisons
  const gch::nonnull_ptr<int> rz (rx);
  CHECK (! (rz      == nullptr));
  CHECK (! (nullptr == rz     ));
  CHECK (  (rz      != nullptr));
  CHECK (  (nullptr != rz     ));
  CHECK (! (rz      <  nullptr));
  CHECK (  (nullptr <  rz     ));
  CHECK (  (rz      >  nullptr));
  CHECK (! (nullptr >  rz     ));
  CHECK (! (rz      <= nullptr));
  CHECK (  (nullptr <= rz     ));
  CHECK (  (rz      >= nullptr));
  CHECK (! (nullptr >= rz     ));

  // compare with a value of different type (not equal)
  void *vy = &a[1];
  CHECK (! (rx == vy));
  CHECK (! (vy == rx));
  CHECK (  (rx != vy));
  CHECK (  (vy != rx));
  CHECK (  (rx <  vy));
  CHECK (! (vy <  rx));
  CHECK (! (rx >  vy));
  CHECK (  (vy >  rx));
  CHECK (  (rx <= vy));
  CHECK (! (vy <= rx));
  CHECK (! (rx >= vy));
  CHECK (  (vy >= rx));

  // compare with a value of different type (equal)
  void *vx = &a[0];
  CHECK (  (rx == vx));
  CHECK (  (vx == rx));
  CHECK (! (rx != vx));
  CHECK (! (vx != rx));
  CHECK (! (rx <  vx));
  CHECK (! (vx <  rx));
  CHECK (! (rx >  vx));
  CHECK (! (vx >  rx));
  CHECK (  (rx <= vx));
  CHECK (  (vx <= rx));
  CHECK (  (rx >= vx));
  CHECK (  (vx >= rx));

  // compare with rvalue reference (not equal)
  CHECK (! (   rx == &a[1]));
  CHECK (! (&a[1] ==    rx));
  CHECK (  (   rx != &a[1]));
  CHECK (  (&a[1] !=    rx));
  CHECK (  (   rx <  &a[1]));
  CHECK (! (&a[1] <     rx));
  CHECK (! (   rx >  &a[1]));
  CHECK (  (&a[1] >     rx));
  CHECK (  (   rx <= &a[1]));
  CHECK (! (&a[1] <=    rx));
  CHECK (! (   rx >= &a[1]));
  CHECK (  (&a[1] >=    rx));

  // compare with rvalue reference (equal)
  CHECK (  (   rx == &a[0]));
  CHECK (  (&a[0] ==    rx));
  CHECK (! (   rx != &a[0]));
  CHECK (! (&a[0] !=    rx));
  CHECK (! (   rx <  &a[0]));
  CHECK (! (&a[0] <     rx));
  CHECK (! (   rx >  &a[0]));
  CHECK (! (&a[0] >     rx));
  CHECK (  (   rx <= &a[0]));
  CHECK (  (&a[0] <=    rx));
  CHECK (  (   rx >= &a[0]));
  CHECK (  (&a[0] >=    rx));

  return 0;
}
