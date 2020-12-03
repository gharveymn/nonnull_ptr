/** nonnull_ptr.hpp
 * Defines an non-null pointer.
 *
 * Copyright © 2019 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef NONNULL_PTR_HPP
#define NONNULL_PTR_HPP

#include <iosfwd>
#include <type_traits>
#include <typeindex>
#include <utility>

#ifndef GCH_CPP14_CONSTEXPR
#  if __cpp_constexpr >= 201304L
#    define GCH_CPP14_CONSTEXPR constexpr
#  else
#    define GCH_CPP14_CONSTEXPR
#  endif
#endif

#ifndef GCH_CPP17_CONSTEXPR
#  if __cpp_constexpr >= 201603L
#    define GCH_CPP17_CONSTEXPR constexpr
#  else
#    define GCH_CPP17_CONSTEXPR
#  endif
#endif

#ifndef GCH_CPP20_CONSTEXPR
#  if __cpp_constexpr >= 201907L
#    define GCH_CPP20_CONSTEXPR constexpr
#  else
#    define GCH_CPP20_CONSTEXPR
#  endif
#endif

#ifndef GCH_NODISCARD
#  if __has_cpp_attribute(nodiscard) >= 201603L
#    define GCH_NODISCARD [[nodiscard]]
#  else
#    define GCH_NODISCARD
#  endif
#endif

#ifndef GCH_INLINE_VARS
#  if __cpp_inline_variables >= 201606
#    define GCH_INLINE_VARS inline
#  else
#    define GCH_INLINE_VARS
#  endif
#endif

#ifndef GCH_CONSTEXPR_SWAP
#  if __cpp_lib_constexpr_algorithms >= 201806L
#    define GCH_CONSTEXPR_SWAP constexpr
#  else
#    define GCH_CONSTEXPR_SWAP
#  endif
#endif

#ifndef GCH_CTAD_SUPPORT
#  if __cpp_deduction_guides >= 201703
#    define GCH_CTAD_SUPPORT
#  endif
#endif

namespace gch
{  
  /**
   * A pointer wrapper which is not nullable.
   * 
   * @tparam Value the value type of the stored pointer.
   */
  template <typename Value>
  class nonnull_ptr
  {
  public:
    
    static_assert(! std::is_reference<Value>::value, 
      "nonnull_ptr expects a value type as a template argument, not a reference.");
    
    using element_type    = Value;        /*!< The value type of the stored pointer */
    using value_type      = Value;        /*!< An alias for `element_type`          */
    using reference       = Value&;       /*!< The reference type to be wrapped     */
    using pointer         = Value*;       /*!< The pointer type to the value type   */
    using const_reference = const Value&; /*!< A constant reference to `Value`      */
    using const_pointer   = const Value*; /*!< A constant pointer to `Value`        */
    
  private:
    
    template <typename U>
    using constructible_from = std::is_constructible<pointer, U*>;
  
    template <typename U>
    using convertible_from = std::is_convertible<U*, pointer>;
    
    template <typename T> struct is_nonnull_ptr                 : std::false_type { };
    template <typename T> struct is_nonnull_ptr<nonnull_ptr<T>> : std::true_type  { };
    
    template <typename ...>
    using void_t = void;
  
    template <typename T, typename Enable = void>
    struct has_constexpr_ampersand
      : std::false_type
    { };
  
    template <typename T>
    struct has_constexpr_ampersand<T, void_t<decltype (&std::declval<T&> ())>>
      : std::true_type
    { };
    
  public:
    
    //! For the record, I don't think the explicit conversions are even possible
    //! since we are only using raw pointers, but I can't find a definitive 
    //! reference to that fact, so they stay.
    
    nonnull_ptr (void)                              = delete;
    nonnull_ptr (const nonnull_ptr&)                = default;
    nonnull_ptr (nonnull_ptr&&) noexcept            = default;
    nonnull_ptr& operator= (const nonnull_ptr&)     = default;
    nonnull_ptr& operator= (nonnull_ptr&&) noexcept = default;
    ~nonnull_ptr (void)                             = default;
  
    /**
     * Constructor
     * 
     * A constructor for the case where `U*` is
     * implicitly convertible to type `pointer`.
     * 
     * @tparam U a referenced value type.
     * @param ref a reference whose pointer is implicitly convertible to type `pointer`.
     */
    template <typename U,
              typename = typename std::enable_if<constructible_from<U>::value>::type,
              typename std::enable_if<convertible_from<U>::value, bool>::type = true>
    constexpr /* implicit */ nonnull_ptr (U& ref) noexcept
      : m_ptr (&ref)
    { }
  
    /**
     * Constructor
     * 
     * A constructor for the case where `pointer` is 
     * explicitly constructable from `U*`.
     * 
     * @tparam U a referenced value type.
     * @param ref a argument from which `pointer` may be explicitly constructed.
     */
    template <typename U,
              typename = typename std::enable_if<constructible_from<U>::value>::type,
              typename std::enable_if<! convertible_from<U>::value, bool>::type = false>
    constexpr explicit nonnull_ptr (U& ref) noexcept
      : m_ptr (&static_cast<reference> (ref))
    { }
  
    /**
     * Constructor
     * 
     * A deleted contructor for the case where `ref` is an rvalue reference.
     */
    template <typename U,
              typename = typename std::enable_if<! is_nonnull_ptr<U>::value>::type>
    nonnull_ptr (const U&&) = delete;
  
    /**
     * Constructor
     * 
     * A copy constructor from another nonnull_ptr for the case 
     * where `U*` is implicitly convertible to type `pointer`.
     * 
     * @tparam U a referenced value type.
     * @param other an nonnull_ptr whose pointer is implicitly 
     *              convertible to type `pointer`.
     */
    template <typename U,
              typename = typename std::enable_if<constructible_from<U>::value>::type,
              typename std::enable_if<convertible_from<U>::value, bool>::type = true>
    constexpr /* implicit */ nonnull_ptr (const nonnull_ptr<U>& other) noexcept
      : m_ptr (other.get ())
    { }
  
    /**
     * Constructor
     * 
     * A copy constructor from another nonnull_ptr for the case 
     * where `pointer` is explicitly constructable from `U*`.
     * 
     * @tparam U a referenced value type.
     * @param ref an nonnull_ptr which contains a pointer from 
     *            which `pointer` may be explicitly constructed.
     */
    template <typename U,
              typename = typename std::enable_if<constructible_from<U>::value>::type,
              typename std::enable_if<! convertible_from<U>::value, bool>::type = false>
    constexpr explicit nonnull_ptr (const nonnull_ptr<U>& other) noexcept
      : m_ptr (static_cast<pointer> (other.get ()))
    { }
    
    /**
     * A conversion operator to `pointer`.
     *
     * This is so we can use `nonnull_ptr` with decaying semantics.
     *
     * @return the stored pointer.
     */
    constexpr /* implicit */ operator pointer (void) const noexcept
    {
      return m_ptr;
    }
    
    /**
     * Returns a pointer.
     * 
     * @return the stored pointer
     */
    GCH_NODISCARD
    constexpr pointer get (void) const noexcept 
    {
      return m_ptr;
    }
    
    /**
     * Returns a reference.
     * 
     * @return the dereferenced pointer.
     */
    GCH_NODISCARD 
    constexpr reference operator* (void) const noexcept 
    {
      return *m_ptr;
    }
    
    /**
     * Returns a pointer to the value.
     * 
     * Never fails. The return is the same as from `get`.
     * 
     * @return a pointer to the value.
     */
    GCH_NODISCARD 
    constexpr pointer operator-> (void) const noexcept 
    { 
      return m_ptr; 
    }
  
    /**
     * Swap the contained pointer with that of `other`.
     * 
     * @param other a reference to another `nonnull_ptr`.
     */
    GCH_CONSTEXPR_SWAP void swap (nonnull_ptr& other) noexcept 
    {
      using std::swap;
      swap (this->m_ptr, other.m_ptr);
    }
    
    /**
     * Sets the contained pointer.
     * 
     * Internally, sets the a pointer to the address of the referenced value.
     * 
     * @tparam U a reference type convertible to `reference`.
     * @param ref an lvalue reference.
     * @return the argument `ref`.
     */
    template <typename U,
              typename = typename std::enable_if<constructible_from<U>::value>::type>
    GCH_CPP14_CONSTEXPR reference emplace (U& ref) noexcept
    {
      return *(m_ptr = &static_cast<reference> (ref));
    }
  
    /**
     * A deleted version for convertible rvalue references.
     * 
     * We don't want to allow rvalue references because the internal pointer 
     * does not sustain the object lifetime.
     */
    template <typename U,
              typename = typename std::enable_if<constructible_from<U>::value>::type>
    reference emplace (const U&&) = delete;
    
  private:
    
    /**
     * A pointer to the value.
     */
    pointer m_ptr;
    
  };
  
  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U the value type of `r`.
   * @param l a `nonnull_ptr`.
   * @param r a `nonnull_ptr`.
   * @return the result of the equality comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator== (const nonnull_ptr<T>& l, const nonnull_ptr<U>& r)
    noexcept (noexcept (l.get () == r.get ())) -> decltype (l.get () == r.get ())
  {
    return l.get () == r.get ();
  }
  
  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U the value type of `r`.
   * @param l a `nonnull_ptr`.
   * @param r a `nonnull_ptr`.
   * @return the result of the inequality comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator!= (const nonnull_ptr<T>& l, const nonnull_ptr<U>& r)
    noexcept (noexcept (l.get () != r.get ())) -> decltype (l.get () != r.get ())
  {
    return l.get () != r.get ();
  }
  
  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U the value type of `r`.
   * @param l a `nonnull_ptr`.
   * @param r a `nonnull_ptr`.
   * @return the result of the less-than comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator< (const nonnull_ptr<T>& l, const nonnull_ptr<U>& r)
    noexcept (noexcept (l.get () < r.get ())) -> decltype (l.get () < r.get ())
  {
    return l.get () < r.get ();
  }
  
  /**
   * A greater-than comparison function
   *
   * @tparam T the value type of `l`.
   * @tparam U the value type of `r`.
   * @param l a `nonnull_ptr`.
   * @param r a `nonnull_ptr`.
   * @return the result of the greater-than comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator> (const nonnull_ptr<T>& l, const nonnull_ptr<U>& r)
    noexcept (noexcept (l.get () > r.get ())) -> decltype (l.get () > r.get ())
  {
    return l.get () > r.get ();
  }
  
  /**
   * A less-than-equal comparison function
   *
   * @tparam T the value type of `l`.
   * @tparam U the value type of `r`.
   * @param l a `nonnull_ptr`.
   * @param r a `nonnull_ptr`.
   * @return the result of the less-than-equal comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator<= (const nonnull_ptr<T>& l, const nonnull_ptr<U>& r)
    noexcept (noexcept (l.get () <= r.get ())) -> decltype (l.get () <= r.get ())
  {
    return l.get () <= r.get ();
  }
  
  /**
   * A greater-than-equal comparison function
   *
   * @tparam T the value type of `l`.
   * @tparam U the value type of `r`.
   * @param l a `nonnull_ptr`.
   * @param r a `nonnull_ptr`.
   * @return the result of the greater-than-equal comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator>= (const nonnull_ptr<T>& l, const nonnull_ptr<U>& r)
    noexcept (noexcept (l.get () >= r.get ())) -> decltype (l.get () >= r.get ())
  {
    return l.get () >= r.get ();
  }
  
  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `l`.
   * @param l a `nonnull_ptr`.
   * @param r a `std::nullptr_t`.
   * @return the result of the equality comparison.
   * 
   * @see std::optional::operator==
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator== (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return false;
  }
  
  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `r`.
   * @param l a `std::nullptr_t`.
   * @param r a `nonnull_ptr`.
   * @return the result of the equality comparison.
   * 
   * @see std::optional::operator==
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator== (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return false;
  }
  
  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `l`.
   * @param l a `nonnull_ptr`.
   * @param r a `std::nullptr_t`.
   * @return the result of the inequality comparison.
   * 
   * @see std::optional::operator!=
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator!= (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return true;
  }
  
  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `r`.
   * @param l a `std::nullptr_t`.
   * @param r a `nonnull_ptr`.
   * @return the result of the inequality comparison.
   * 
   * @see std::optional::operator!=
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator!= (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return true;
  }
  
  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `l`.
   * @param l a `nonnull_ptr`.
   * @param r a `std::nullptr_t`.
   * @return the result of the less-than comparison.
   * 
   * @see std::optional::operator<
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator< (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return false;
  }
  
  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `r`.
   * @param l a `std::nullptr_t`.
   * @param r a `nonnull_ptr`.
   * @return the result of the less-than comparison.
   * 
   * @see std::optional::operator<
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator< (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return true;
  }
  
  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `l`.
   * @param l a `nonnull_ptr`.
   * @param r a `std::nullptr_t`.
   * @return the result of the greater-than comparison.
   * 
   * @see std::optional::operator>
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator> (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return true;
  }
  
  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `r`.
   * @param l a `std::nullptr_t`.
   * @param r a `nonnull_ptr`.
   * @return the result of the greater-than comparison.
   * 
   * @see std::optional::operator>
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator> (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return false;
  }
  
  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `l`.
   * @param l a `nonnull_ptr`.
   * @param r a `std::nullptr_t`.
   * @return the result of the less-than-equal comparison.
   * 
   * @see std::optional::operator<=
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator<= (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return false;
  }
  
  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `r`.
   * @param l a `std::nullptr_t`.
   * @param r a `nonnull_ptr`.
   * @return the result of the less-than-equal comparison.
   * 
   * @see std::optional::operator<=
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator<= (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return true;
  }
  
  /**
   * A greater-than-equal comparison function.
   *
   * @tparam T the value type of `l`.
   * @param l a `nonnull_ptr`.
   * @param r a `std::nullptr_t`.
   * @return the result of the greater-than-equal comparison.
   * 
   * @see std::optional::operator>=
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator>= (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return true;
  }
  
  /**
   * A greater-than-equal comparison function.
   *
   * @tparam T the value type of `r`.
   * @param l a `std::nullptr_t`.
   * @param r a `nonnull_ptr`.
   * @return the result of the greater-than-equal comparison.
   * 
   * @see std::optional::operator>=
   */
  template <typename T> GCH_NODISCARD
  constexpr bool operator>= (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return false;
  }
  
  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U a value type which is comparable to T*.
   * @param l a `nonnull_ptr`.
   * @param r a comparable pointer.
   * @return the result of the equality comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator== (const nonnull_ptr<T>& l, U* r)
    noexcept (noexcept (l.get () == r)) -> decltype (l.get () == r)
  {
    return l.get () == r;
  }
  
  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `r`.
   * @tparam U a value type which is comparable to T*.
   * @param l a comparable pointer.
   * @param r a `nonnull_ptr`.
   * @return the result of the equality comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator== (U* l, const nonnull_ptr<T>& r)
    noexcept (noexcept (l == r.get ())) -> decltype (l == r.get ())
  {
    return l == r.get ();
  }
  
  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U a value type which is comparable to T*.
   * @param l a `nonnull_ptr`.
   * @param r a comparable pointer.
   * @return the result of the inequality comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator!= (const nonnull_ptr<T>& l, U* r)
    noexcept (noexcept (l.get () != r)) -> decltype (l.get () != r)
  {
    return l.get () != r;
  }
  
  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `r`.
   * @tparam U a value type which is comparable to T*.
   * @param l a comparable pointer.
   * @param r a `nonnull_ptr`.
   * @return the result of the inequality comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator!= (U* l, const nonnull_ptr<T>& r)
    noexcept (noexcept (l != r.get ())) -> decltype (l != r.get ())
  {
    return l != r.get ();
  }
  
  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U a value type which is comparable to T*.
   * @param l a `nonnull_ptr`.
   * @param r a comparable pointer.
   * @return the result of the less-than comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator< (const nonnull_ptr<T>& l, U* r)
    noexcept (noexcept (l.get () < r)) -> decltype (l.get () < r)
  {
    return l.get () < r;
  }
  
  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `r`.
   * @tparam U a value type which is comparable to T*.
   * @param l a comparable pointer.
   * @param r a `nonnull_ptr`.
   * @return the result of the less-than comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator< (U* l, const nonnull_ptr<T>& r)
    noexcept (noexcept (l < r.get ())) -> decltype (l < r.get ())
  {
    return l < r.get ();
  }
  
  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U a value type which is comparable to T*.
   * @param l a `nonnull_ptr`.
   * @param r a comparable pointer.
   * @return the result of the greater-than comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator> (const nonnull_ptr<T>& l, U* r)
    noexcept (noexcept (l.get () > r)) -> decltype (l.get () > r)
  {
    return l.get () > r;
  }
  
  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `r`.
   * @tparam U a value type which is comparable to T*.
   * @param l a comparable pointer.
   * @param r a `nonnull_ptr`.
   * @return the result of the greater-than comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator> (U* l, const nonnull_ptr<T>& r)
    noexcept (noexcept (l > r.get ())) -> decltype (l > r.get ())
  {
    return l > r.get ();
  }
  
  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U a value type which is comparable to T*.
   * @param l a `nonnull_ptr`.
   * @param r a comparable pointer.
   * @return the result of the less-than-equal comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator<= (const nonnull_ptr<T>& l, U* r)
    noexcept (noexcept (l.get () <= r)) -> decltype (l.get () <= r)
  {
    return l.get () <= r;
  }
  
  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `r`.
   * @tparam U a value type which is comparable to T*.
   * @param l a comparable pointer.
   * @param r a `nonnull_ptr`.
   * @return the result of the less-than-equal comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator<= (U* l, const nonnull_ptr<T>& r)
    noexcept (noexcept (l <= r.get ())) -> decltype (l <= r.get ())
  {
    return l <= r.get ();
  }
  
  /**
   * A greater-than-equal comparison function.
   *
   * @tparam T the value type of `l`.
   * @tparam U a value type which is comparable to T*.
   * @param l a `nonnull_ptr`.
   * @param r a comparable pointer.
   * @return the result of the greater-than-equal comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator>= (const nonnull_ptr<T>& l, U* r)
    noexcept (noexcept (l.get () >= r)) -> decltype (l.get () >= r)
  {
    return l.get () >= r;
  }
  
  /**
   * A greater-than-equal comparison function.
   *
   * @tparam T the value type of `r`.
   * @tparam U a value type which is comparable to T*.
   * @param l a comparable pointer.
   * @param r a `nonnull_ptr`.
   * @return the result of the greater-than-equal comparison.
   */
  template <typename T, typename U> GCH_NODISCARD
  constexpr auto operator>= (U* l, const nonnull_ptr<T>& r)
    noexcept (noexcept (l >= r.get ())) -> decltype (l >= r.get ())
  {
    return l >= r.get ();
  }
  
  /**
   * A swap function.
   * 
   * Swaps the two `nonnull_ptr`s of the same type.
   * 
   * @tparam T the value type pointed to by the `nonnull_ptr`s
   * @param l a `nonnull_ptr`.
   * @param r a `nonnull_ptr`.
   */
  template <typename T>
  GCH_CONSTEXPR_SWAP inline void 
  swap (nonnull_ptr<T>& l, nonnull_ptr<T>& r) noexcept
  {
    l.swap (r);
  }
  
  /**
   * An nonnull_ptr creation function.
   * 
   * Creates a `nonnull_ptr` with the specified argument.
   * 
   * @tparam U a forwarded type.
   * @param ref a forwarded value.
   * @return a `nonnull_ptr` created from the argument.
   * 
   * @see std::make_optional
   */
  template <typename U>
  GCH_NODISCARD
  constexpr nonnull_ptr<typename std::remove_reference<U>::type>
  make_nonnull_ptr (U&& ref) noexcept
  {
    return nonnull_ptr<typename std::remove_reference<U>::type> { std::forward<U> (ref) };
  }

#ifdef GCH_CTAD_SUPPORT
  template <typename U>
  nonnull_ptr (U&&) -> nonnull_ptr<std::remove_reference_t<U>>;
#endif
  
  
  /**
   * Prints the given `nonnull_ptr`.
   *
   * We defer to the `T*` overload of `operator<<`.
   *
   * @tparam T the element type of the `nonnull_ptr`.
   * @param o a `std::ostream`.
   * @param nn a `nonnull_ptr`.
   * @return a reference to the modified `std::ostream`.
   */
  template <typename T>
  std::ostream& operator<< (std::ostream& o, const nonnull_ptr<T>& nn)
  {
    return o << nn.get ();
  }
}

/**
 * A specialization of `std::hash` for `gch::nonnull_ptr`.
 *
 * @tparam T the value type of `gch::nonnull_ptr`.
 */
template <typename T>
struct std::hash<gch::nonnull_ptr<T>>
{
  /**
   * An invokable operator.
   *
   * We just do a noop pointer hash (which is unique).
   *
   * @param nn a reference to a value of type `gch::nonnull_ptr`.
   * @return a hash of the argument.
   */
  constexpr std::size_t operator() (const gch::nonnull_ptr<T>& nn) const noexcept
  {
    return reinterpret_cast<std::size_t> (nn.get ());
  }
};

#endif
