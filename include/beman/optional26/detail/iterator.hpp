// include/beman/optional26/detail/iterator.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_OPTIONAL26_DETAIL_ITERATOR_HPP
#define BEMAN_OPTIONAL26_DETAIL_ITERATOR_HPP

#include <beman/optional26/detail/stl_interfaces/iterator_interface.hpp>

#include <concepts>
#include <iterator>

namespace beman::optional26::detail {

// Forward declaration.
template <class T, class Container>
struct contiguous_iterator;

// Base class for contiguous iterator types with Boost stl_interfaces library.
// Current implementation based on P2727R4: std::iterator_interface.
template <class T, class Container>
using base_contiguous_iterator = stl_interfaces::iterator_interface<
#if !BEMAN_OPTIONAL26_DETAIL_STL_INTERFACES_USE_DEDUCED_THIS
    contiguous_iterator<T, Container>, // Required for P2727R4 to work with C++20/C++23. TODO: Do more experiments.
#endif
    std::contiguous_iterator_tag,
    T>;

// This is a minimal contiguous iterator.  It uses stl_interfaces library from Boost
// (current implementation based on https://wg21.link/P2727R4).
//
// TODO: Change this to use the stl_interfaces library from Beman if/when available.
//
// @tparam T - The type of the elements the iterator points to.
// @tparam Container - The type of the container the iterator points to. This parameter exists solely so that different
// containers using this template can instantiate different types, even if the T parameter is the same.
template <class T, class Container>
struct contiguous_iterator : public base_contiguous_iterator<T, Container> {
    // Alias for the base class.
    using base_type = base_contiguous_iterator<T, Container>;
    // Alias for types from the base class.
    using typename base_type::difference_type;
    using typename base_type::pointer;
    using typename base_type::reference;

    // Default constructor.
    constexpr contiguous_iterator() noexcept : m_current() {}

    // Pointer to iterator constructor.
    constexpr contiguous_iterator(pointer it) noexcept : m_current(it) {}

    // As per P2727R4, for contiguous iterator we only need to provide operator*, operator+= and operator-.
    constexpr reference operator*() const noexcept { return *m_current; }
    constexpr auto&     operator+=(difference_type pos) noexcept {
        m_current += pos;
        return *this;
    }
    constexpr difference_type operator-(contiguous_iterator other) const noexcept {
        return m_current - other.m_current;
    }

  private:
    T* m_current;
};

} // namespace beman::optional26::detail

#endif // BEMAN_OPTIONAL26_DETAIL_ITERATOR_HPP
