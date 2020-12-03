#include "gch/nonnull_ptr.hpp"

#include <iostream>
#include <cassert>
#include <unordered_map>
#include <chrono>
#include <random>
#include <algorithm>
#include <numeric>

using namespace gch;

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

template bool gch::operator== (const nonnull_ptr<int>&, const nonnull_ptr<const int>&) noexcept;
template bool gch::operator!= (const nonnull_ptr<int>&, const nonnull_ptr<const int>&) noexcept;
template bool gch::operator<  (const nonnull_ptr<int>&, const nonnull_ptr<const int>&) noexcept;
template bool gch::operator>  (const nonnull_ptr<int>&, const nonnull_ptr<const int>&) noexcept;
template bool gch::operator<= (const nonnull_ptr<int>&, const nonnull_ptr<const int>&) noexcept;
template bool gch::operator>= (const nonnull_ptr<int>&, const nonnull_ptr<const int>&) noexcept;

template bool gch::operator== (const nonnull_ptr<int>&, void*                  ) noexcept;
template bool gch::operator== (void*,                   const nonnull_ptr<int>&) noexcept;
template bool gch::operator!= (const nonnull_ptr<int>&, void*                  ) noexcept;
template bool gch::operator!= (void*,                   const nonnull_ptr<int>&) noexcept;
template bool gch::operator<  (const nonnull_ptr<int>&, void*                  ) noexcept;
template bool gch::operator<  (void*,                   const nonnull_ptr<int>&) noexcept;
template bool gch::operator>  (const nonnull_ptr<int>&, void*                  ) noexcept;
template bool gch::operator>  (void*,                   const nonnull_ptr<int>&) noexcept;
template bool gch::operator<= (const nonnull_ptr<int>&, void*                  ) noexcept;
template bool gch::operator<= (void*,                   const nonnull_ptr<int>&) noexcept;
template bool gch::operator>= (const nonnull_ptr<int>&, void*                  ) noexcept;
template bool gch::operator>= (void*,                   const nonnull_ptr<int>&) noexcept;

static_assert (std::is_trivially_copyable<nonnull_ptr<int>>::value, "");
static_assert (std::is_trivially_destructible<nonnull_ptr<int>>::value, "");

static constexpr int g_x = 0;
static constexpr nonnull_ptr<const int> g_rx { g_x };

void print_test_header (const std::string& str)
{
  std::cout << str << "... ";
}

void print_test_footer ()
{
  std::cout << "pass" << std::endl;
}

void test_const (void)
{
  print_test_header ("test const");
  
  int x = 1;
  int y = 2;
  const int z = 1;
  
  nonnull_ptr<int> rx { x };
  nonnull_ptr<const int> rz = z;
  
  assert (*rx == *rz);
  assert (rx != rz);
  
  // set rz with a non-const reference
  rz = y;
  
  assert (*rx != *rz);
  assert (rx != rz);
  
  rz = x;
  
  assert (*rx == *rz);
  assert (rx == rz);
  
  print_test_footer ();
}

void test_arrow (void)
{
  print_test_header ("test arrow");
  
  struct my_struct
  {
    int x;
  } s { 1 };
  
  const nonnull_ptr<my_struct> r { s };
  assert (r->x == 1);
  
  print_test_footer ();
}

void test_assign (void)
{
  print_test_header ("test assign");
  
  int x = 1;
  nonnull_ptr<int> r { x };
  assert (*r == 1);
  *r = 2;
  assert (*r == 2);
  assert (x == 2);
  
  int y = 3;
  assert (r.emplace (y) == 3);
  assert (*r == 3);
  assert (y == 3);
  
  print_test_footer ();
}

void test_inheritence (void)
{
  print_test_header ("test inheritence");
  struct my_struct_base
  {
    int x;
    constexpr bool operator== (my_struct_base& other) const noexcept
    {
      return this->x == other.x;
    }
  
    constexpr bool operator!= (my_struct_base& other) const noexcept
    {
      return ! operator== (other);
    }
  };
  
  struct my_struct : my_struct_base
  { };
  
  my_struct s0 { };
  my_struct s1 { };
  s1.x = 1;
  const nonnull_ptr<my_struct_base> r0 = s0;
  const nonnull_ptr<my_struct> r1 (s1);
  
  assert (*r0 != *r1);
  assert (r0 != r1);
  
  const nonnull_ptr<my_struct_base> r2 (r1);
  
  assert (*r2 == *r1);
  assert (r2 == r1);
  
  my_struct_base* base_ptr = r1;
  assert (base_ptr == r1);
  
  print_test_footer ();
}

void test_movement (void)
{
  print_test_header ("test movement");
  
  int a[2] = { 1, 2 };
  const nonnull_ptr<int> rx { a[0] };
  const nonnull_ptr<int> ry { a[1] };
  assert (! (rx == ry));
  assert (  (rx != ry));
  assert (  (rx <  ry));
  assert (  (ry >  rx));
  assert (  (rx <= ry));
  assert (  (ry >= rx));
  
  // copy constructor
  nonnull_ptr<int> rz { rx };
  assert (*rz == *rx);
  assert (*rz != *ry);
  assert (rz == rx);
  assert (rz != ry);
  
  // move constructor
  nonnull_ptr<int> rm { std::move (rz) };
  assert (*rm == *rx);
  assert (*rm != *ry);
  assert (rm == rx);
  assert (rm != ry);
  
  // copy assignment operator
  rz = ry;
  assert (*rz != *rx);
  assert (*rz == *ry);
  assert (rz != rx);
  assert (rz == ry);
  
  // move assignment operator
  rm = std::move (rz);
  assert (*rm != *rx);
  assert (*rm == *ry);
  assert (rm != rx);
  assert (rm == ry);
  
  // creation using a temporary
  const nonnull_ptr<int> rt { nonnull_ptr<int> (a[0]) };
  assert (*rt == a[0]);
  assert (*rt != a[1]);
  
  // swap
  
  nonnull_ptr<int> rp { a[0] };
  nonnull_ptr<int> rq { a[1] };
  
  using std::swap;
  swap (rp, rq);
  
  assert (! (rp == rq));
  assert (  (rp != rq));
  assert (! (rp <  rq));
  assert (! (rq >  rp));
  assert (! (rp <= rq));
  assert (! (rq >= rp));
  
  print_test_footer ();
}

void test_comparison (void)
{
  print_test_header ("test comparison");
  
  int a[2] = { 11, 22 };
  int& x = a[0];
  int& y = a[1];
  
  const nonnull_ptr<int> rx (x);
  nonnull_ptr<const int> ry (y);
  
  // disparate type comparisons (not equal)
  assert (! (rx == ry));
  assert (! (ry == rx));
  assert (  (rx != ry));
  assert (  (ry != rx));
  assert (  (rx <  ry));
  assert (! (ry <  rx));
  assert (! (rx >  ry));
  assert (  (ry >  rx));
  assert (  (rx <= ry));
  assert (! (ry <= rx));
  assert (! (rx >= ry));
  assert (  (ry >= rx));
  
  // disparate type comparisons (equal)
  ry = a[0];
  assert (  (rx == ry));
  assert (  (ry == rx));
  assert (! (rx != ry));
  assert (! (ry != rx));
  assert (! (rx <  ry));
  assert (! (ry <  rx));
  assert (! (rx >  ry));
  assert (! (ry >  rx));
  assert (  (rx <= ry));
  assert (  (ry <= rx));
  assert (  (rx >= ry));
  assert (  (ry >= rx));
  
  // nullopt comparisons
  const nonnull_ptr<int> rz (rx);
  assert (! (rz      == nullptr));
  assert (! (nullptr == rz     ));
  assert (  (rz      != nullptr));
  assert (  (nullptr != rz     ));
  assert (! (rz      <  nullptr));
  assert (  (nullptr <  rz     ));
  assert (  (rz      >  nullptr));
  assert (! (nullptr >  rz     ));
  assert (! (rz      <= nullptr));
  assert (  (nullptr <= rz     ));
  assert (  (rz      >= nullptr));
  assert (! (nullptr >= rz     ));
  
  // compare with a value of different type (not equal)
  void *vy = &a[1];
  assert (! (rx == vy));
  assert (! (vy == rx));
  assert (  (rx != vy));
  assert (  (vy != rx));
  assert (  (rx <  vy));
  assert (! (vy <  rx));
  assert (! (rx >  vy));
  assert (  (vy >  rx));
  assert (  (rx <= vy));
  assert (! (vy <= rx));
  assert (! (rx >= vy));
  assert (  (vy >= rx));
  
  // compare with a value of different type (equal)
  void *vx = &a[0];
  assert (  (rx == vx));
  assert (  (vx == rx));
  assert (! (rx != vx));
  assert (! (vx != rx));
  assert (! (rx <  vx));
  assert (! (vx <  rx));
  assert (! (rx >  vx));
  assert (! (vx >  rx));
  assert (  (rx <= vx));
  assert (  (vx <= rx));
  assert (  (rx >= vx));
  assert (  (vx >= rx));
  
  // compare with rvalue reference (not equal)
  assert (! (   rx == &a[1]));
  assert (! (&a[1] ==    rx));
  assert (  (   rx != &a[1]));
  assert (  (&a[1] !=    rx));
  assert (  (   rx <  &a[1]));
  assert (! (&a[1] <     rx));
  assert (! (   rx >  &a[1]));
  assert (  (&a[1] >     rx));
  assert (  (   rx <= &a[1]));
  assert (! (&a[1] <=    rx));
  assert (! (   rx >= &a[1]));
  assert (  (&a[1] >=    rx));
  
  // compare with rvalue reference (equal)
  assert (  (   rx == &a[0]));
  assert (  (&a[0] ==    rx));
  assert (! (   rx != &a[0]));
  assert (! (&a[0] !=    rx));
  assert (! (   rx <  &a[0]));
  assert (! (&a[0] <     rx));
  assert (! (   rx >  &a[0]));
  assert (! (&a[0] >     rx));
  assert (  (   rx <= &a[0]));
  assert (  (&a[0] <=    rx));
  assert (  (   rx >= &a[0]));
  assert (  (&a[0] >=    rx));
  
  print_test_footer ();
}

void test_make_optional_ref (void)
{
  print_test_header ("test make_optional_ref");
  
  int x = 1;
  const int y = 2;
  nonnull_ptr<int>       rx = make_nonnull_ptr (x);
  nonnull_ptr<const int> ry = make_nonnull_ptr (y);
  assert (rx != ry);
  
  print_test_footer ();
}

void test_hash (void)
{
  print_test_header ("test hash");
  
  std::unordered_map<nonnull_ptr<int>, const std::string *> map { };
  int x = 1;
  int y = 2;
  int z = 3;
  std::string xs = "x";
  std::string ys = "y";
  std::string zs = "z";
  map.emplace (x, &xs);
  map.emplace (y, &ys);
  map.emplace (z, &zs);
  
  assert (&xs == map[x]);
  assert (&ys == map[y]);
  assert (&zs == map[z]);
  
  print_test_footer ();
}

void test_deduction (void)
{
#if __cpp_deduction_guides >= 201703
  print_test_header ("test deduction");
  
  int  x = 1;
  long y = 2;
  
  nonnull_ptr rx (x);
  nonnull_ptr ry (y);
  
  print_test_footer ();
#endif
}

double test_perf_equality_worker_double (void)
{
  using namespace std::chrono;
  using clock = high_resolution_clock;
  using time = clock::time_point;
  time t1 = clock::now ();
  
  constexpr auto v_sz = 100;
  constexpr auto num  = 10000000;
  
  std::vector<double> v (v_sz);
  std::generate (v.begin (), v.end (), rand);
  
  std::random_device rd;
  std::mt19937 gen (rd ());
  std::uniform_int_distribution<std::size_t> dist (0, v_sz - 1);
  
  for (auto i = 0; i < num; ++i)
  {
    double* l = &v[dist (gen)];
    double* r = &v[dist (gen)];
    static_cast<void> (l == r);
  }
  
  time t2 = clock::now ();
  return duration_cast<duration<double>> (t2 - t1).count ();
}

double test_perf_equality_worker (void)
{
  using namespace std::chrono;
  using clock = high_resolution_clock;
  using time = clock::time_point;
  time t1 = clock::now ();
  
  constexpr auto v_sz = 100;
  constexpr auto num  = 10000000;
  
  std::vector<double> v (v_sz);
  std::generate (v.begin (), v.end (), rand);
  
  std::random_device rd;
  std::mt19937 gen (rd ());
  std::uniform_int_distribution<std::size_t> dist (0, v_sz - 1);
  
  for (auto i = 0; i < num; ++i)
  {
    nonnull_ptr<double> l = v[dist (gen)];
    nonnull_ptr<double> r = v[dist (gen)];
    static_cast<void> (l == r);
  }
  
  time t2 = clock::now ();
  return duration_cast<duration<double>> (t2 - t1).count ();
}

void test_perf_equality (void)
{
  constexpr auto num_tests = 10;
  std::vector<double> times (num_tests);
  {
    std::generate (times.begin (), times.end (), test_perf_equality_worker);
    auto mean = std::accumulate (times.begin (), times.end (), 0.0) / num_tests;
    
    auto variance = std::accumulate (times.begin (), times.end (), 0.0,
                                     [&mean, num_tests] (double accum, const double& val)
                                     {
                                       return accum +
                                         (std::pow (val - mean, 2.0) / (num_tests - 1));
                                     });
    
    auto stddev = std::sqrt (variance);
    
    std::cout << "  nonnull mean: " << mean << std::endl;
    std::cout << "nonnull stddev: " << stddev << std::endl;
  }
  {
    std::generate (times.begin (), times.end (), test_perf_equality_worker_double);
    auto mean = std::accumulate (times.begin (), times.end (), 0.0) / num_tests;
  
    auto variance = std::accumulate (times.begin (), times.end (), 0.0,
                                     [&mean, num_tests] (double accum, const double& val)
                                     {
                                       return accum +
                                         (std::pow (val - mean, 2.0) / (num_tests - 1));
                                     });
  
    auto stddev = std::sqrt (variance);
  
    std::cout << "  double* mean: " << mean << std::endl;
    std::cout << "double* stddev: " << stddev << std::endl;
  }
}

int main (void) 
{
  std::cout << g_rx << std::endl;
  test_const ();
  test_arrow ();
  test_assign ();
  test_inheritence ();
  test_movement ();
  test_comparison ();
  test_make_optional_ref ();
  test_hash ();
  test_deduction ();
  // test_perf_equality ();
  return 0;
}
