/** test-movement.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "test_common.hpp"

int
main (void)
{
  int a[2] = { 1, 2 };
  const gch::nonnull_ptr<int> rx { a[0] };
  const gch::nonnull_ptr<int> ry { a[1] };
  CHECK (! (rx == ry));
  CHECK (  (rx != ry));
  CHECK (  (rx <  ry));
  CHECK (  (ry >  rx));
  CHECK (  (rx <= ry));
  CHECK (  (ry >= rx));

  // copy constructor
  gch::nonnull_ptr<int> rz { rx };
  CHECK (*rz == *rx);
  CHECK (*rz != *ry);
  CHECK (rz == rx);
  CHECK (rz != ry);

  // move constructor
  gch::nonnull_ptr<int> rm { std::move (rz) };
  CHECK (*rm == *rx);
  CHECK (*rm != *ry);
  CHECK (rm == rx);
  CHECK (rm != ry);

  // copy assignment operator
  rz = ry;
  CHECK (*rz != *rx);
  CHECK (*rz == *ry);
  CHECK (rz != rx);
  CHECK (rz == ry);

  // move assignment operator
  rm = std::move (rz);
  CHECK (*rm != *rx);
  CHECK (*rm == *ry);
  CHECK (rm != rx);
  CHECK (rm == ry);

  // creation using a temporary
  const gch::nonnull_ptr<int> rt { gch::nonnull_ptr<int> (a[0]) };
  CHECK (*rt == a[0]);
  CHECK (*rt != a[1]);

  // swap

  gch::nonnull_ptr<int> rp { a[0] };
  gch::nonnull_ptr<int> rq { a[1] };

  using std::swap;
  swap (rp, rq);

  CHECK (! (rp == rq));
  CHECK (  (rp != rq));
  CHECK (! (rp <  rq));
  CHECK (! (rq >  rp));
  CHECK (! (rp <= rq));
  CHECK (! (rq >= rp));

  return 0;
}
