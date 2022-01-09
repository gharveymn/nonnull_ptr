/** test-instantiation.cpp
 * Copyright © 2022 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "test_common.hpp"

struct base { };
struct derived : base { };

// these kill MSVC for some reason
#ifndef _MSC_VER

template class gch::nonnull_ptr<int>;
template gch::nonnull_ptr<int>::nonnull_ptr (const int&&);

template class gch::nonnull_ptr<base>;
template class gch::nonnull_ptr<derived>;
template gch::nonnull_ptr<base>::nonnull_ptr (derived&);
template gch::nonnull_ptr<base>::nonnull_ptr (const base&&);
template gch::nonnull_ptr<base>::nonnull_ptr (const derived&&);
template auto gch::nonnull_ptr<base>::emplace (base&) -> reference;
template auto gch::nonnull_ptr<base>::emplace (derived&) -> reference;
template auto gch::nonnull_ptr<base>::emplace (const base&&) -> reference;
template auto gch::nonnull_ptr<base>::emplace (const derived&&) -> reference;

#endif

template bool gch::operator== (const nonnull_ptr<int>&, const nonnull_ptr<int>&);
template bool gch::operator!= (const nonnull_ptr<int>&, const nonnull_ptr<int>&);
template bool gch::operator<  (const nonnull_ptr<int>&, const nonnull_ptr<int>&);
template bool gch::operator>  (const nonnull_ptr<int>&, const nonnull_ptr<int>&);
template bool gch::operator<= (const nonnull_ptr<int>&, const nonnull_ptr<int>&);
template bool gch::operator>= (const nonnull_ptr<int>&, const nonnull_ptr<int>&);
#ifdef GCH_LIB_THREE_WAY_COMPARISON
template std::strong_ordering gch::operator<=> (const nonnull_ptr<int>&, const nonnull_ptr<int>&);
#endif

template bool gch::operator== (const nonnull_ptr<int>&, std::nullptr_t) noexcept;
#ifdef GCH_LIB_THREE_WAY_COMPARISON
template std::strong_ordering gch::operator<=> (const nonnull_ptr<int>&, std::nullptr_t) noexcept;
#else
template bool gch::operator== (std::nullptr_t,          const nonnull_ptr<int>&) noexcept;
template bool gch::operator!= (const nonnull_ptr<int>&, std::nullptr_t         ) noexcept;
template bool gch::operator!= (std::nullptr_t,          const nonnull_ptr<int>&) noexcept;
template bool gch::operator<  (const nonnull_ptr<int>&, std::nullptr_t         ) noexcept;
template bool gch::operator<  (std::nullptr_t,          const nonnull_ptr<int>&) noexcept;
template bool gch::operator>  (const nonnull_ptr<int>&, std::nullptr_t         ) noexcept;
template bool gch::operator>  (std::nullptr_t,          const nonnull_ptr<int>&) noexcept;
template bool gch::operator<= (const nonnull_ptr<int>&, std::nullptr_t         ) noexcept;
template bool gch::operator<= (std::nullptr_t,          const nonnull_ptr<int>&) noexcept;
template bool gch::operator>= (const nonnull_ptr<int>&, std::nullptr_t         ) noexcept;
template bool gch::operator>= (std::nullptr_t,          const nonnull_ptr<int>&) noexcept;
#endif

template bool gch::operator== (const nonnull_ptr<int>&, const int *            ) noexcept;
template bool gch::operator== (const int *,             const nonnull_ptr<int>&) noexcept;
template bool gch::operator!= (const nonnull_ptr<int>&, const int *            ) noexcept;
template bool gch::operator!= (const int *,             const nonnull_ptr<int>&) noexcept;
template bool gch::operator<  (const nonnull_ptr<int>&, const int *            );
template bool gch::operator<  (const int *,             const nonnull_ptr<int>&);
template bool gch::operator>  (const nonnull_ptr<int>&, const int *            );
template bool gch::operator>  (const int *,             const nonnull_ptr<int>&);
template bool gch::operator<= (const nonnull_ptr<int>&, const int *            );
template bool gch::operator<= (const int *,             const nonnull_ptr<int>&);
template bool gch::operator>= (const nonnull_ptr<int>&, const int *            );
template bool gch::operator>= (const int *,             const nonnull_ptr<int>&);
#ifdef GCH_LIB_THREE_WAY_COMPARISON
template std::strong_ordering gch::operator<=> (const nonnull_ptr<int>&, const int *);
#endif

static_assert (std::is_trivially_copyable<gch::nonnull_ptr<int>>::value, "");
static_assert (std::is_trivially_destructible<gch::nonnull_ptr<int>>::value, "");

static constexpr int g_x = 0;
static constexpr gch::nonnull_ptr<const int> g_rx { g_x };

int
main (void)
{
  static_cast<void> (g_rx);

  return 0;
}
