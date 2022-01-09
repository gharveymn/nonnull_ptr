/** test-inheritance.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct my_struct_base
{
  int x;
};

struct my_struct : my_struct_base
{ };

constexpr
bool
operator== (const my_struct_base& lhs, const my_struct_base& rhs) noexcept
{
  return lhs.x == rhs.x;
}

constexpr
bool
operator!= (const my_struct_base& lhs, const my_struct_base& rhs) noexcept
{
  return lhs.x != rhs.x;
}

#include "test_common.hpp"

int
main (void)
{
  my_struct s0 { };
  my_struct s1 { };
  s1.x = 1;
  const gch::nonnull_ptr<my_struct_base> r0 { s0 };
  const gch::nonnull_ptr<my_struct> r1 (s1);

  CHECK (*r0 != *r1);
  CHECK (r0 != r1);

  const gch::nonnull_ptr<my_struct_base> r2 (r1);

  CHECK (*r2 == *r1);
  CHECK (r2 == r1);

  my_struct_base* base_ptr = r1;
  CHECK (base_ptr == r1);

  return 0;
}
