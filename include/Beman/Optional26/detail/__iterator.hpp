// include/Beman/Optional26/detail/__iterator.hpp                                            -*-C++-*-
#ifndef BEMAN_OPTIONAL26_OPTIONAL_DETAIL__ITERATOR_HPP
#define BEMAN_OPTIONAL26_OPTIONAL_DETAIL__ITERATOR_HPP

#include <iterator>
#include <type_traits>
#include <utility>

namespace beman::optional {
// This iterator adapter is @a normal in the sense that it does not
// change the semantics of any of the operators of its iterator
// parameter.  Its primary purpose is to convert an iterator that is
// not a class, e.g. a pointer, into an iterator that is a class.
// The ContainerType parameter exists solely so that different containers
// using this template can instantiate different types, even if the
// IteratorType parameter is the same.
template <typename IteratorType, typename ContainerType>
class normal_iterator {
  protected:
    IteratorType m_current;

    typedef std::iterator_traits<IteratorType> traits_type;

#if __cplusplus >= 201103L
    template <typename _Iter>
    using __convertible_from = std::__enable_if_t<std::is_convertible_v<_Iter, IteratorType>>;
#endif

  public:
    typedef IteratorType                            iterator_type;
    typedef typename traits_type::iterator_category iterator_category;
    typedef typename traits_type::value_type        value_type;
    typedef typename traits_type::difference_type   difference_type;
    typedef typename traits_type::reference         reference;
    typedef typename traits_type::pointer           pointer;

    // #if __cplusplus > 201703L && __glibcxx_concepts
    //     using iterator_concept = std::__detail::__iter_concept<IteratorType>;
    // #endif

    constexpr normal_iterator() noexcept : m_current(IteratorType()) {}

    explicit constexpr normal_iterator(const IteratorType& __i) noexcept : m_current(__i) {}

#if __cplusplus >= 201103L
    // Allow iterator to const_iterator conversion
    template <typename _Iter, typename = __convertible_from<_Iter>>
    constexpr normal_iterator(const normal_iterator<_Iter, ContainerType>& __i) noexcept
#else
    // N.B. ContainerType::pointer is not actually in container requirements,
    // but is present in std::vector and std::basic_string.
    template <typename _Iter>
    normal_iterator(
        const normal_iterator<_Iter,
                              typename __enable_if<(std::are_same_v<_Iter, typename ContainerType::pointer>),
                                                   ContainerType>::__type>& __i)
#endif
        : m_current(__i.base()) {
    }

    // Forward iterator requirements
    constexpr reference operator*() const noexcept { return *m_current; }

    constexpr pointer operator->() const noexcept { return m_current; }

    constexpr normal_iterator& operator++() noexcept {
        ++m_current;
        return *this;
    }

    constexpr normal_iterator operator++(int) noexcept { return normal_iterator(m_current++); }

    // Bidirectional iterator requirements
    constexpr normal_iterator& operator--() noexcept {
        --m_current;
        return *this;
    }

    constexpr normal_iterator operator--(int) noexcept { return normal_iterator(m_current--); }

    // Random access iterator requirements
    constexpr reference operator[](difference_type __n) const noexcept { return m_current[__n]; }

    constexpr normal_iterator& operator+=(difference_type __n) noexcept {
        m_current += __n;
        return *this;
    }

    constexpr normal_iterator operator+(difference_type __n) const noexcept {
        return normal_iterator(m_current + __n);
    }

    constexpr normal_iterator& operator-=(difference_type __n) noexcept {
        m_current -= __n;
        return *this;
    }

    constexpr normal_iterator operator-(difference_type __n) const noexcept {
        return normal_iterator(m_current - __n);
    }

    constexpr const IteratorType& base() const noexcept { return m_current; }
};

// Note: In what follows, the left- and right-hand-side iterators are
// allowed to vary in types (conceptually in cv-qualification) so that
// comparison between cv-qualified and non-cv-qualified iterators be
// valid.  However, the greedy and unfriendly operators in std::rel_ops
// will make overload resolution ambiguous (when in scope) if we don't
// provide overloads whose operands are of the same type.  Can someone
// remind me what generic programming is about? -- Gaby

#if __cpp_lib_three_way_comparison
template <typename _IteratorL, typename _IteratorR, typename ContainerType>
[[nodiscard]] constexpr bool
operator==(const normal_iterator<_IteratorL, ContainerType>& __lhs,
           const normal_iterator<_IteratorR, ContainerType>& __rhs) noexcept(noexcept(__lhs.base() == __rhs.base()))
    requires requires {
        { __lhs.base() == __rhs.base() } -> std::convertible_to<bool>;
    }
{
    return __lhs.base() == __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename ContainerType>
[[nodiscard]] constexpr std::__detail::__synth3way_t<_IteratorR, _IteratorL>
operator<=>(const normal_iterator<_IteratorL, ContainerType>& __lhs,
            const normal_iterator<_IteratorR, ContainerType>&
                __rhs) noexcept(noexcept(std::__detail::__synth3way(__lhs.base(), __rhs.base()))) {
    return std::__detail::__synth3way(__lhs.base(), __rhs.base());
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr bool
operator==(const normal_iterator<IteratorType, ContainerType>& __lhs,
           const normal_iterator<IteratorType, ContainerType>& __rhs) noexcept(noexcept(__lhs.base() == __rhs.base()))
    requires requires {
        { __lhs.base() == __rhs.base() } -> std::convertible_to<bool>;
    }
{
    return __lhs.base() == __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr std::__detail::__synth3way_t<IteratorType>
operator<=>(const normal_iterator<IteratorType, ContainerType>& __lhs,
            const normal_iterator<IteratorType, ContainerType>&
                __rhs) noexcept(noexcept(std::__detail::__synth3way(__lhs.base(), __rhs.base()))) {
    return std::__detail::__synth3way(__lhs.base(), __rhs.base());
}
#else
// Forward iterator requirements
template <typename _IteratorL, typename _IteratorR, typename ContainerType>
[[nodiscard]] constexpr inline bool operator==(const normal_iterator<_IteratorL, ContainerType>& __lhs,
                                               const normal_iterator<_IteratorR, ContainerType>& __rhs) noexcept {
    return __lhs.base() == __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline bool operator==(const normal_iterator<IteratorType, ContainerType>& __lhs,
                                               const normal_iterator<IteratorType, ContainerType>& __rhs) noexcept {
    return __lhs.base() == __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename ContainerType>
[[nodiscard]] constexpr inline bool operator!=(const normal_iterator<_IteratorL, ContainerType>& __lhs,
                                               const normal_iterator<_IteratorR, ContainerType>& __rhs) noexcept {
    return __lhs.base() != __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline bool operator!=(const normal_iterator<IteratorType, ContainerType>& __lhs,
                                               const normal_iterator<IteratorType, ContainerType>& __rhs) noexcept {
    return __lhs.base() != __rhs.base();
}

// Random access iterator requirements
template <typename _IteratorL, typename _IteratorR, typename ContainerType>
[[nodiscard]] inline bool operator<(const normal_iterator<_IteratorL, ContainerType>& __lhs,
                                    const normal_iterator<_IteratorR, ContainerType>& __rhs) noexcept {
    return __lhs.base() < __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline bool operator<(const normal_iterator<IteratorType, ContainerType>& __lhs,
                                              const normal_iterator<IteratorType, ContainerType>& __rhs) noexcept {
    return __lhs.base() < __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename ContainerType>
[[nodiscard]] inline bool operator>(const normal_iterator<_IteratorL, ContainerType>& __lhs,
                                    const normal_iterator<_IteratorR, ContainerType>& __rhs) noexcept {
    return __lhs.base() > __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline bool operator>(const normal_iterator<IteratorType, ContainerType>& __lhs,
                                              const normal_iterator<IteratorType, ContainerType>& __rhs) noexcept {
    return __lhs.base() > __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename ContainerType>
[[nodiscard]] inline bool operator<=(const normal_iterator<_IteratorL, ContainerType>& __lhs,
                                     const normal_iterator<_IteratorR, ContainerType>& __rhs) noexcept {
    return __lhs.base() <= __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline bool operator<=(const normal_iterator<IteratorType, ContainerType>& __lhs,
                                               const normal_iterator<IteratorType, ContainerType>& __rhs) noexcept {
    return __lhs.base() <= __rhs.base();
}

template <typename _IteratorL, typename _IteratorR, typename ContainerType>
[[nodiscard]] inline bool operator>=(const normal_iterator<_IteratorL, ContainerType>& __lhs,
                                     const normal_iterator<_IteratorR, ContainerType>& __rhs) noexcept {
    return __lhs.base() >= __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline bool operator>=(const normal_iterator<IteratorType, ContainerType>& __lhs,
                                               const normal_iterator<IteratorType, ContainerType>& __rhs) noexcept {
    return __lhs.base() >= __rhs.base();
}
#endif // three-way comparison

// _GLIBCXX_RESOLVE_LIB_DEFECTS
// According to the resolution of DR179 not only the various comparison
// operators but also operator- must accept mixed iterator/const_iterator
// parameters.
template <typename _IteratorL, typename _IteratorR, typename ContainerType>
#if __cplusplus >= 201103L
// DR 685.
[[__nodiscard__]] constexpr inline auto
operator-(const normal_iterator<_IteratorL, ContainerType>& __lhs,
          const normal_iterator<_IteratorR, ContainerType>& __rhs) noexcept -> decltype(__lhs.base() - __rhs.base())
#else
inline typename normal_iterator<_IteratorL, ContainerType>::difference_type
operator-(const normal_iterator<_IteratorL, ContainerType>& __lhs,
          const normal_iterator<_IteratorR, ContainerType>& __rhs)
#endif
{
    return __lhs.base() - __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline typename normal_iterator<IteratorType, ContainerType>::difference_type
operator-(const normal_iterator<IteratorType, ContainerType>& __lhs,
          const normal_iterator<IteratorType, ContainerType>& __rhs) noexcept {
    return __lhs.base() - __rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline normal_iterator<IteratorType, ContainerType>
operator+(typename normal_iterator<IteratorType, ContainerType>::difference_type __n,
          const normal_iterator<IteratorType, ContainerType>&                    __i) noexcept {
    return normal_iterator<IteratorType, ContainerType>(__i.base() + __n);
}

} // namespace beman::optional

#endif // BEMAN_OPTIONAL26_OPTIONAL_DETAIL__ITERATOR_HPP
