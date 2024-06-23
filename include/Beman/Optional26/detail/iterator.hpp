// include/Beman/Optional26/detail/iterator.hpp                                            -*-C++-*-
#ifndef BEMAN_OPTIONAL26_DETAIL_ITERATOR_HPP
#define BEMAN_OPTIONAL26_DETAIL_ITERATOR_HPP

#include <Beman/Optional26/detail/stl_interfaces/iterator_interface.hpp>

#include <iterator>

namespace beman::optional::detail {

// This is a minimal contiguous iterator.  It uses stl_interfaces library from Boost
// (current implementation based on https://wg21.link/P2727R4).
//
// TODO: Change this to use the stl_interfaces library from Beman when available.
//
// @tparam T - The type of the elements the iterator points to.
// @tparam Container - The type of the container the iterator points to. This parameter exists solely so that different
// containers using this template can instantiate different types, even if the T parameter is the same.
template <class T, class Container>
struct contiguous_iterator : stl_interfaces::iterator_interface<
#if !BEMAN_OPTIONAL26_DETAIL_STL_INTERFACES_USE_DEDUCED_THIS
                                 contiguous_iterator<T, Container>,
#endif
                                 std::contiguous_iterator_tag,
                                 T> {
    using iterator_type     = T*;
    using iterator_category = std::iterator_traits<iterator_type>::iterator_category;
    using iterator_concept  = std::iterator_traits<iterator_type>::iterator_concept;
    using value_type        = std::iterator_traits<iterator_type>::value_type;
    using difference_type   = std::iterator_traits<iterator_type>::difference_type;
    using reference         = std::iterator_traits<iterator_type>::reference;
    using pointer           = std::iterator_traits<iterator_type>::pointer;

    // Default constructor.
    contiguous_iterator() noexcept : m_current() {}

    // Pointer to iterator constructor.
    contiguous_iterator(pointer it) noexcept : m_current(it) {}

    // As per P2727R4, for contiguous iterator we only need to provide operator*, operator+= and operator-.
    reference operator*() const noexcept { return *m_current; }
    auto&     operator+=(difference_type pos) noexcept {
        m_current += pos;
        return *this;
    }
    difference_type operator-(contiguous_iterator other) const noexcept { return m_current - other.m_current; }

  private:
    T* m_current;
};

} // namespace beman::optional::detail

#endif // BEMAN_OPTIONAL26_DETAIL_ITERATOR_HPP
