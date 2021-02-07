/** nonnull_ptr.hpp
 * Defines an pointer wrapper which is not nullable.
 *
 * Copyright © 2020-2021 Gene Harvey
 *
 * This software may be modified and distributed under the terms
 * of the MIT license. See the LICENSE file for details.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef GCH_NONNULL_PTR_HPP
#define GCH_NONNULL_PTR_HPP

#include <type_traits>
#include <functional>
#include <utility>

#ifdef __clang__
#  if defined (__cplusplus) && __cplusplus >= 201703L
#    ifndef GCH_CLANG_17
#      define GCH_CLANG_17
#    endif
#  endif
#endif

#ifdef __cpp_constexpr
#  ifndef GCH_CPP14_CONSTEXPR
#    if __cpp_constexpr >= 201304L
#      define GCH_CPP14_CONSTEXPR constexpr
#      ifndef GCH_HAS_CPP14_CONSTEXPR
#        define GCH_HAS_CPP14_CONSTEXPR
#      endif
#    else
#      define GCH_CPP14_CONSTEXPR
#    endif
#  endif
#  ifndef GCH_CPP17_CONSTEXPR
#    if __cpp_constexpr >= 201603L
#      define GCH_CPP17_CONSTEXPR constexpr
#      ifndef GCH_HAS_CPP17_CONSTEXPR
#        define GCH_HAS_CPP17_CONSTEXPR
#      endif
#    else
#      define GCH_CPP17_CONSTEXPR
#    endif
#  endif
#  ifndef GCH_CPP20_CONSTEXPR
#    if __cpp_constexpr >= 201907L
#      define GCH_CPP20_CONSTEXPR constexpr
#      ifndef GCH_HAS_CPP20_CONSTEXPR
#        define GCH_HAS_CPP20_CONSTEXPR
#      endif
#    else
#      define GCH_CPP20_CONSTEXPR
#    endif
#  endif
#endif

#ifndef GCH_CPP20_CONSTEVAL
#  if defined (__cpp_consteval) && __cpp_consteval >= 201811L
#    define GCH_CPP20_CONSTEVAL consteval
#  else
#    define GCH_CPP20_CONSTEVAL constexpr
#  endif
#endif

#ifndef GCH_NODISCARD
#  if defined (__has_cpp_attribute) && __has_cpp_attribute (nodiscard) >= 201603L
#    if ! defined (__clang__) || defined (GCH_CLANG_17)
#      define GCH_NODISCARD [[nodiscard]]
#    else
#      define GCH_NODISCARD
#    endif
#  else
#    define GCH_NODISCARD
#  endif
#endif

#if defined (__cpp_deduction_guides) && __cpp_deduction_guides >= 201703
#  ifndef GCH_CTAD_SUPPORT
#    define GCH_CTAD_SUPPORT
#  endif
#endif

#if defined (__cpp_concepts) && __cpp_concepts >= 201907L
#  ifndef GCH_CONCEPTS
#    define GCH_CONCEPTS
#  endif
#  if defined(__has_include) && __has_include(<concepts>)
#    include <concepts>
#    if defined (__cpp_lib_concepts) && __cpp_lib_concepts >= 202002L
#      if ! defined (GCH_LIB_CONCEPTS) && ! defined (GCH_DISABLE_CONCEPTS)
#        define GCH_LIB_CONCEPTS
#      endif
#    endif
#  endif
#endif

#if defined (__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
#  ifndef GCH_IMPL_THREE_WAY_COMPARISON
#    define GCH_IMPL_THREE_WAY_COMPARISON
#  endif
#  if __has_include(<compare>)
#    include <compare>
#    if defined (__cpp_lib_three_way_comparison) && __cpp_lib_three_way_comparison >= 201907L
#      ifndef GCH_LIB_THREE_WAY_COMPARISON
#        define GCH_LIB_THREE_WAY_COMPARISON
#      endif
#    endif
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

    // std::iterator_traits
    using difference_type   = std::ptrdiff_t; /*!< The same difference type as stored pointer */
    using value_type        = Value;          /*!< An alias for `element_type`          */
    using pointer           = Value *;        /*!< The pointer type to the value type   */
    using reference         = Value&;         /*!< The reference type to be wrapped     */
    using iterator_category = std::random_access_iterator_tag;
#ifdef GCH_LIB_CONCEPTS
    using iterator_concept  = std::contiguous_iterator_tag;
#endif

    // std::pointer_traits
    using element_type = Value;               /*!< The element type of the stored pointer */

    template <typename U>
    using rebind = nonnull_ptr<U>;            /*!< A template for rebinding this type */

    // other
    using const_reference = const Value&;     /*!< A constant reference to `Value`      */
    using const_pointer   = const Value *;    /*!< A constant pointer to `Value`        */

  private:
    template <typename U>
    using constructible_from_pointer_to =
      std::is_constructible<pointer, decltype (&std::declval<U&> ())>;

    template <typename U>
    using pointer_to_is_convertible =
      std::is_convertible<decltype (&std::declval<U&> ()), pointer>;

  public:
    /**
     * Constructor
     *
     * A deleted default constructor
     */
    nonnull_ptr (void) = delete;

    /**
     * Constructor
     *
     * A copy constructor.
     *
     * The value of `other` is copied into `*this`.
     *
     * Note: TriviallyCopyable.
     */
    nonnull_ptr (const nonnull_ptr&) noexcept = default;

    /**
     * Constructor
     *
     * A move constructor.
     *
     * The value of `other` is copied into `*this`.
     *
     * Note: TriviallyCopyable.
     */
    nonnull_ptr (nonnull_ptr&&) noexcept = default;

    /**
     * Assignment operator
     *
     * A copy-assignment operator.
     *
     * The resultant state of `*this` is equivalent
     * to that of `other`.
     *
     * Note: TriviallyCopyable.
     *
     * @return `*this`
     */
    nonnull_ptr&
    operator= (const nonnull_ptr&) noexcept = default;

    /**
     * Assignment operator
     *
     * A move-assignment operator.
     *
     * Sets the state of `*this` to that of `other`.
     * It is implementation defined if `other` contains
     * a value after this.
     *
     * Note: TriviallyCopyable.
     *
     * @return `*this`
     */
    nonnull_ptr&
    operator= (nonnull_ptr&&) noexcept = default;

    /**
     * Destructor
     *
     * A trivial destructor.
     *
     * Note: TriviallyCopyable.
     *
     */
    ~nonnull_ptr (void)                             = default;

    /**
     * Constructor
     *
     * An explicit converting constructor for reference
     * types explicitly convertible to `pointer`.
     *
     * @tparam U a referenced value type.
     * @param ref a argument from which `pointer` may be explicitly constructed.
     */
    template <typename U,
              typename std::enable_if<constructible_from_pointer_to<U>::value>::type * = nullptr>
    constexpr explicit
    nonnull_ptr (U& ref) noexcept
      : m_ptr (&ref)
    { }

    /**
     * Constructor
     *
     * A deleted constructor for the case where `ref` is an rvalue reference.
     */
    template <typename U,
              typename = typename std::enable_if<constructible_from_pointer_to<U>::value>::type>
    nonnull_ptr (const U&&) = delete;

    /**
     * Constructor
     *
     * A copy constructor from another nonnull_ptr for the case
     * where `U *` is implicitly convertible to type `pointer`.
     *
     * @tparam U a referenced value type.
     * @param other a nonnull_ptr whose pointer is implicitly
     *              convertible to type `pointer`.
     */
    template <typename U,
              typename std::enable_if<std::is_constructible<pointer, U *>::value
                                  &&  std::is_convertible<U *, pointer>::value>::type * = nullptr>
    constexpr /* implicit */
    nonnull_ptr (const nonnull_ptr<U>& other) noexcept
      : m_ptr (other.get ())
    { }

    /**
     * Constructor
     *
     * A copy constructor from another nonnull_ptr for the case
     * where `pointer` is explicitly constructible from `U *`.
     *
     * @tparam U a referenced value type.
     * @param other a nonnull_ptr which contains a pointer from
     *              which `pointer` may be explicitly constructed.
     */
    template <typename U,
              typename std::enable_if<std::is_constructible<pointer, U *>::value
                                  &&! std::is_convertible<U *, pointer>::value>::type * = nullptr>
    constexpr explicit
    nonnull_ptr (const nonnull_ptr<U>& other) noexcept
      : m_ptr (other.get ())
    { }

    /**
     * An implicit conversion to `pointer`.
     *
     * This is so we can use `nonnull_ptr` with decaying semantics.
     *
     * @return the stored pointer.
     */
    GCH_NODISCARD constexpr /* implicit */
    operator pointer (void) const noexcept
    {
      return m_ptr;
    }

    /**
     * An explicit conversion to `reference`.
     *
     * This is provided because the class basically
     * acts as a rebindable reference wrapper.
     *
     * @return the dereferenced pointer
     */
    GCH_NODISCARD constexpr explicit
    operator reference (void) const noexcept
    {
      return *m_ptr;
    }

    /**
     * Returns the pointer.
     *
     * @return the stored pointer
     */
    GCH_NODISCARD constexpr
    pointer
    get (void) const noexcept
    {
      return m_ptr;
    }

    /**
     * Returns the dereferenced pointer.
     *
     * @return the dereferenced pointer.
     */
    GCH_NODISCARD constexpr
    reference
    operator* (void) const noexcept
    {
      return *m_ptr;
    }

    /**
     * Returns a pointer to the value.
     *
     * The return is the same as from `get`.
     *
     * @return a pointer to the value.
     */
    GCH_NODISCARD constexpr
    pointer
    operator-> (void) const noexcept
    {
      return m_ptr;
    }

    /**
     * Swap the contained pointer with that of `other`.
     *
     * @param other a reference to another `nonnull_ptr`.
     */
    GCH_CPP14_CONSTEXPR
    void
    swap (nonnull_ptr& other) noexcept
    {
      // manually implemented so we can lower the constexpr version requirements to c++14
      pointer tmp = m_ptr;
      m_ptr       = other.m_ptr;
      other.m_ptr = tmp;
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
              typename = typename std::enable_if<constructible_from_pointer_to<U>::value>::type>
    GCH_CPP14_CONSTEXPR
    reference
    emplace (U& ref) noexcept
    {
      return *(m_ptr = pointer (&ref));
    }

    /**
     * A deleted version for rvalue references.
     *
     * We don't want to allow rvalue references because the internal pointer
     * does not sustain the object lifetime.
     */
    template <typename U,
              typename = typename std::enable_if<constructible_from_pointer_to<U>::value>::type>
    reference
    emplace (const U&&) = delete;

    /**
     * Sets the contained pointer.
     *
     * Sets the pointer to the pointer contained by another nonnull_ptr,
     * where `pointer` is constructible from `U *`.
     *
     * @tparam U a referenced value type.
     * @param other a nonnull_ptr which contains a pointer from
     *              which `pointer` may be constructed.
     * @return a reference to the contained pointer.
     */
    template <typename U,
              typename = typename std::enable_if<std::is_constructible<pointer, U *>::value>>
    GCH_CPP14_CONSTEXPR
    reference
    emplace (const nonnull_ptr<U>& other) noexcept
    {
      return *(m_ptr = pointer (other.get ()));
    }

    /**
     * Constructs a nonnull_ptr using the argument.
     *
     * This is for use with std::pointer_traits.
     *
     * @param r a reference of type `element_type&`.
     * @return a nonnull_ptr containing a pointer to `r`.
     */
    static constexpr
    nonnull_ptr
    pointer_to (element_type& r) noexcept
    {
      return nonnull_ptr { r };
    }

  private:
    /**
     * A pointer to a value.
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
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator== (const nonnull_ptr<T>& l, const nonnull_ptr<U>& r)
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
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator!= (const nonnull_ptr<T>& l, const nonnull_ptr<U>& r)
  {
    return l.get () != r.get ();
  }

  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U the value type of `rhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the less-than comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator< (const nonnull_ptr<T>& lhs, const nonnull_ptr<U>& rhs)
  {
    using common_ty = typename std::common_type<typename nonnull_ptr<T>::pointer,
                                                typename nonnull_ptr<U>::pointer>::type;
    return std::less<common_ty> { } (lhs.get (), rhs.get ());
  }

  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U the value type of `rhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the less-than-equal comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator<= (const nonnull_ptr<T>& lhs, const nonnull_ptr<U>& rhs)
  {
    return ! (rhs < lhs);
  }

  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U the value type of `rhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the greater-than comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator> (const nonnull_ptr<T>& lhs, const nonnull_ptr<U>& rhs)
  {
    return rhs < lhs;
  }

  /**
   * A greater-than-equal comparison function
   *
   * @tparam T the value type of `lhs`.
   * @tparam U the value type of `rhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the greater-than-equal comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator>= (const nonnull_ptr<T>& lhs, const nonnull_ptr<U>& rhs)
  {
    return ! (lhs < rhs);
  }

#ifdef GCH_LIB_THREE_WAY_COMPARISON

  /**
   * A three-way comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U the value type of `rhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the three-way comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  std::compare_three_way_result_t<typename nonnull_ptr<T>::pointer,
                                  typename nonnull_ptr<U>::pointer>
  operator<=> (const nonnull_ptr<T>& lhs, const nonnull_ptr<U>& rhs)
    requires std::three_way_comparable_with<typename nonnull_ptr<T>::pointer,
                                            typename nonnull_ptr<U>::pointer>
  {
    return std::compare_three_way { } (lhs.get (), rhs.get ());
  }

#endif

  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `std::nullptr_t`.
   * @return the result of the equality comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator== (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return false;
  }

#ifdef GCH_LIB_THREE_WAY_COMPARISON

  /**
   * A three-way comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `std::nullptr_t`.
   * @return the result of the three-way comparison.
   */
  template <typename T>
  GCH_NODISCARD constexpr
  std::strong_ordering
  operator<=> (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return std::strong_ordering::greater;
  }

#else

  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @param lhs a `std::nullptr_t`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the equality comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator== (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return false;
  }

  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `std::nullptr_t`.
   * @return the result of the inequality comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator!= (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return true;
  }

  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @param lhs a `std::nullptr_t`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the inequality comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator!= (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return true;
  }

  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `std::nullptr_t`.
   * @return the result of the less-than comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator< (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return false;
  }

  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @param lhs a `std::nullptr_t`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the less-than comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator< (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return true;
  }

  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `std::nullptr_t`.
   * @return the result of the greater-than comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator> (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return true;
  }

  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @param lhs a `std::nullptr_t`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the greater-than comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator> (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return false;
  }

  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `std::nullptr_t`.
   * @return the result of the less-than-equal comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator<= (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return false;
  }

  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @param lhs a `std::nullptr_t`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the less-than-equal comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator<= (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return true;
  }

  /**
   * A greater-than-equal comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `std::nullptr_t`.
   * @return the result of the greater-than-equal comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator>= (const nonnull_ptr<T>&, std::nullptr_t) noexcept
  {
    return true;
  }

  /**
   * A greater-than-equal comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @param lhs a `std::nullptr_t`.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the greater-than-equal comparison.
   */
  template <typename T>
  GCH_NODISCARD GCH_CPP20_CONSTEVAL
  bool
  operator>= (std::nullptr_t, const nonnull_ptr<T>&) noexcept
  {
    return false;
  }

#endif

  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a comparable pointer.
   * @return the result of the equality comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator== (const nonnull_ptr<T>& lhs, U *rhs)
  {
    return lhs.get () == rhs;
  }

  /**
   * An equality comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a comparable pointer.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the equality comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator== (U *lhs, const nonnull_ptr<T>& rhs)
  {
    return lhs == rhs.get ();
  }

  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a comparable pointer.
   * @return the result of the inequality comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator!= (const nonnull_ptr<T>& lhs, U *rhs)
  {
    return lhs.get () != rhs;
  }

  /**
   * An inequality comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a comparable pointer.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the inequality comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator!= (U *lhs, const nonnull_ptr<T>& rhs)
  {
    return lhs != rhs.get ();
  }

  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a comparable pointer.
   * @return the result of the less-than comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator< (const nonnull_ptr<T>& lhs, U *rhs)
  {
    using common_ty = typename std::common_type<typename nonnull_ptr<T>::pointer, U *>::type;
    return std::less<common_ty> { } (lhs.get (), rhs);
  }

  /**
   * A less-than comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a comparable pointer.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the less-than comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator< (U *lhs, const nonnull_ptr<T>& rhs)
  {
    using common_ty = typename std::common_type<U *, typename nonnull_ptr<T>::pointer>::type;
    return std::less<common_ty> { } (lhs, rhs.get ());
  }

  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a comparable pointer.
   * @return the result of the less-than-equal comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator<= (const nonnull_ptr<T>& lhs, U *rhs)
  {
    return ! (rhs < lhs);
  }

  /**
   * A less-than-equal comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a comparable pointer.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the less-than-equal comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator<= (U *lhs, const nonnull_ptr<T>& rhs)
  {
    return ! (rhs < lhs);
  }

  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a comparable pointer.
   * @return the result of the greater-than comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator> (const nonnull_ptr<T>& lhs, U* rhs)
  {
    return rhs < lhs;
  }

  /**
   * A greater-than comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a comparable pointer.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the greater-than comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator> (U* lhs, const nonnull_ptr<T>& rhs)
  {
    return rhs < lhs;
  }

  /**
   * A greater-than-equal comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a `nonnull_ptr`.
   * @param rhs a comparable pointer.
   * @return the result of the greater-than-equal comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator>= (const nonnull_ptr<T>& lhs, U* rhs)
  {
    return ! (lhs < rhs);
  }

  /**
   * A greater-than-equal comparison function.
   *
   * @tparam T the value type of `rhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs a comparable pointer.
   * @param rhs a `nonnull_ptr`.
   * @return the result of the greater-than-equal comparison.
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  bool
  operator>= (U* lhs, const nonnull_ptr<T>& rhs)
  {
    return ! (lhs < rhs);
  }

#ifdef GCH_LIB_THREE_WAY_COMPARISON

  /**
   * A three-way comparison function.
   *
   * @tparam T the value type of `lhs`.
   * @tparam U a value type which is comparable to `T *`.
   * @param lhs an `nonnull_ptr`.
   * @param rhs a comparable lvalue reference.
   * @return the result of the three-way comparison.
   *
   * @see std::optional::operator<=>
   */
  template <typename T, typename U>
  GCH_NODISCARD constexpr
  std::compare_three_way_result_t<typename nonnull_ptr<T>::pointer, U *>
  operator<=> (const nonnull_ptr<T>& lhs, U *rhs)
    requires std::three_way_comparable_with<typename nonnull_ptr<T>::pointer, U *>
  {
    return std::compare_three_way { } (lhs.get (), rhs);
  }

#endif

  /**
   * A swap function.
   *
   * Swaps the two `nonnull_ptr`s of the same type.
   *
   * @tparam T the value type pointed to by the `nonnull_ptr`s
   * @param lhs a `nonnull_ptr`.
   * @param rhs a `nonnull_ptr`.
   */
  template <typename T>
  inline GCH_CPP14_CONSTEXPR
  void
  swap (nonnull_ptr<T>& lhs, nonnull_ptr<T>& rhs) noexcept
  {
    lhs.swap (rhs);
  }

  /**
   * An nonnull_ptr creation function.
   *
   * Creates a `nonnull_ptr` with the specified argument.
   *
   * @tparam U a value type.
   * @param ref a reference.
   * @return a `nonnull_ptr` created from the argument.
   */
  template <typename U>
  GCH_NODISCARD constexpr
  nonnull_ptr<U>
  make_nonnull_ptr (U& ref) noexcept
  {
    return nonnull_ptr<U> { ref };
  }

  /**
   * A deleted version for the case where `ref` is an rvalue reference.
   */
  template <typename U>
  GCH_NODISCARD constexpr
  nonnull_ptr<U>
  make_nonnull_ptr (const U&& ref) = delete;

#ifdef GCH_CTAD_SUPPORT

  template <typename U>
  nonnull_ptr (U&) -> nonnull_ptr<U>;

#endif

  /**
   * A convenience alias for const pointers.
   *
   * @tparam T an unqualified value type.
   */
  template <typename T>
  using nonnull_cptr = nonnull_ptr<const T>;

}

namespace std
{

  /**
   * A specialization of `std::hash` for `gch::nonnull_ptr`.
   *
   * @tparam T the value type of `gch::nonnull_ptr`.
   */
  template <typename T>
  struct hash<gch::nonnull_ptr<T>>
  {
    /**
     * An invokable operator.
     *
     * We just forward to std::hash on the underlying pointer.
     *
     * @param nn a reference to a value of type `gch::nonnull_ptr`.
     * @return a hash of the argument.
     */
    std::size_t
    operator() (const gch::nonnull_ptr<T>& ptr) const noexcept
    {
      return std::hash<typename gch::nonnull_ptr<T>::pointer> { } (ptr.get ());
    }
  };

}

#endif
