/** test_common.hpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef NONNULL_PTR_TEST_COMMON_HPP
#define NONNULL_PTR_TEST_COMMON_HPP

#include "gch/nonnull_ptr.hpp"

#include <cstdio>

#define CHECK(EXPR)                                                           \
if (! (EXPR))                                                                 \
{                                                                             \
  printf ("Check failed in file " __FILE__ " at line %i:\n" #EXPR, __LINE__); \
  return 1;                                                                   \
} (void)0

#endif // NONNULL_PTR_TEST_COMMON_HPP
