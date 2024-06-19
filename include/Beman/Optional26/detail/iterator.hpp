// include/Beman/Optional26/detail/iterator.hpp                                            -*-C++-*-
#ifndef BEMAN_OPTIONAL26_OPTIONAL_DETAIL_ITERATOR_HPP
#define BEMAN_OPTIONAL26_OPTIONAL_DETAIL_ITERATOR_HPP

#include <iterator>
#include <type_traits>

namespace beman::optional::detail {
// Implementation inspired by libstdc++'s normal_iterator
// (https://github.com/gcc-mirror/gcc/blob/master/libstdc%2B%2B-v3/include/bits/stl_iterator.h)
//
//
// This iterator adapter is a normal iterator in the sense that it does not
// change the semantics of any of the operators of its iterator
// parameter.  Its primary purpose is to convert an iterator that is
// not a class, e.g. a pointer, into an iterator that is a class.
//
// The ContainerType parameter exists solely so that different containers
// using this template can instantiate different types, even if the
// IteratorType parameter is the same.
//
// FUTURE Use std::iterator_interface from https://wg21.link/P2727R4.
// Adopt Boost's iterator_interface (https://github.com/boostorg/stl_interfaces) into a Beman library.
//
template <typename IteratorType, typename ContainerType>
class normal_iterator {
  public:
    using iterator_type     = IteratorType;
    using iterator_category = std::iterator_traits<iterator_type>::iterator_category;
    using iterator_concept  = std::iterator_traits<iterator_type>::iterator_concept;
    using value_type        = std::iterator_traits<iterator_type>::value_type;
    using difference_type   = std::iterator_traits<iterator_type>::difference_type;
    using reference         = std::iterator_traits<iterator_type>::reference;
    using pointer           = std::iterator_traits<iterator_type>::pointer;

    // Default constructor.
    constexpr normal_iterator() noexcept : m_current(IteratorType()) {}

    // Constructor from an iterator.
    explicit constexpr normal_iterator(const IteratorType& iter) noexcept : m_current(iter) {}

    // Allow iterator to const_iterator conversion.
    template <typename OtherIteratorType,
              typename = std::enable_if_t<std::is_convertible_v<OtherIteratorType, IteratorType>>>
    constexpr normal_iterator(const normal_iterator<OtherIteratorType, ContainerType>& iter) noexcept
        : m_current(iter.base()) {}

    constexpr const auto& base() const noexcept { return m_current; }

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
    constexpr reference operator[](difference_type n) const noexcept { return m_current[n]; }

    constexpr normal_iterator& operator+=(difference_type n) noexcept {
        m_current += n;
        return *this;
    }

    constexpr normal_iterator operator+(difference_type n) const noexcept { return normal_iterator(m_current + n); }

    constexpr normal_iterator& operator-=(difference_type n) noexcept {
        m_current -= n;
        return *this;
    }

    constexpr normal_iterator operator-(difference_type n) const noexcept { return normal_iterator(m_current - n); }

  protected:
    // The underlying pointer iterator.
    IteratorType m_current;
};

// Forward iterator requirements
template <typename IteratorTypeL, typename IteratorTypeR, typename ContainerType>
[[nodiscard]] constexpr inline bool operator==(const normal_iterator<IteratorTypeL, ContainerType>& lhs,
                                               const normal_iterator<IteratorTypeR, ContainerType>& rhs) noexcept {
    return lhs.base() == rhs.base();
}

template <typename IteratorTypeL, typename IteratorTypeR, typename ContainerType>
[[nodiscard]] constexpr inline bool operator!=(const normal_iterator<IteratorTypeL, ContainerType>& lhs,
                                               const normal_iterator<IteratorTypeR, ContainerType>& rhs) noexcept {
    return lhs.base() != rhs.base();
}

// Random access iterator requirements
template <typename IteratorTypeL, typename IteratorTypeR, typename ContainerType>
[[nodiscard]] inline bool operator<(const normal_iterator<IteratorTypeL, ContainerType>& lhs,
                                    const normal_iterator<IteratorTypeR, ContainerType>& rhs) noexcept {
    return lhs.base() < rhs.base();
}

template <typename IteratorTypeL, typename IteratorTypeR, typename ContainerType>
[[nodiscard]] inline bool operator>(const normal_iterator<IteratorTypeL, ContainerType>& lhs,
                                    const normal_iterator<IteratorTypeR, ContainerType>& rhs) noexcept {
    return lhs.base() > rhs.base();
}

template <typename IteratorTypeL, typename IteratorTypeR, typename ContainerType>
[[nodiscard]] inline bool operator<=(const normal_iterator<IteratorTypeL, ContainerType>& lhs,
                                     const normal_iterator<IteratorTypeR, ContainerType>& rhs) noexcept {
    return lhs.base() <= rhs.base();
}

template <typename IteratorTypeL, typename IteratorTypeR, typename ContainerType>
[[nodiscard]] inline bool operator>=(const normal_iterator<IteratorTypeL, ContainerType>& lhs,
                                     const normal_iterator<IteratorTypeR, ContainerType>& rhs) noexcept {
    return lhs.base() >= rhs.base();
}

// According to the resolution of DR179 not only the various comparison
// operators but also operator- must accept mixed iterator/const_iterator
template <typename IteratorTypeL, typename IteratorTypeR, typename ContainerType>
[[nodiscard]] constexpr inline auto operator-(const normal_iterator<IteratorTypeL, ContainerType>& lhs,
                                              const normal_iterator<IteratorTypeR, ContainerType>& rhs) noexcept {
    return lhs.base() - rhs.base();
}

template <typename IteratorType, typename ContainerType>
[[nodiscard]] constexpr inline normal_iterator<IteratorType, ContainerType>
operator+(typename normal_iterator<IteratorType, ContainerType>::difference_type n,
          const normal_iterator<IteratorType, ContainerType>&                    iter) noexcept {
    return normal_iterator(iter.base() + n);
}

} // namespace beman::optional::detail

#endif // BEMAN_OPTIONAL26_OPTIONAL_DETAIL_ITERATOR_HPP
