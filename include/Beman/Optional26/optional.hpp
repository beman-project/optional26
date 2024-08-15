// Beman/Optional26/optional.hpp                                     -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_OPTIONAL26_OPTIONAL_HPP
#define BEMAN_OPTIONAL26_OPTIONAL_HPP

#include <compare>
#include <concepts>
#if defined(__cpp_lib_format_ranges)
#include <format>
#endif
#include <functional>
#include <ranges>
#include <type_traits>
#include <utility>

#include <Beman/Optional26/detail/iterator.hpp>

namespace beman::optional26 {
// Using declarations of traits used
using std::add_lvalue_reference_t;
using std::add_rvalue_reference_t;
using std::addressof;
using std::compare_three_way_result_t;
using std::conjunction_v;
using std::construct_at;
using std::convertible_to;
using std::decay_t;
using std::destroy_at;
using std::initializer_list;
using std::invoke;
using std::invoke_result_t;
using std::is_array_v;
using std::is_assignable_v;
using std::is_constructible_v;
using std::is_convertible_v;
using std::is_copy_assignable_v;
using std::is_copy_constructible_v;
using std::is_lvalue_reference;
using std::is_rvalue_reference;
using std::is_move_assignable_v;
using std::is_move_constructible_v;
using std::is_nothrow_constructible_v;
using std::is_nothrow_move_constructible;
using std::is_nothrow_move_constructible_v;
using std::is_nothrow_swappable;
using std::is_object_v;
using std::is_reference_v;
using std::is_same;
using std::is_same_v;
using std::is_scalar;
using std::is_swappable_v;
using std::is_trivially_copy_assignable_v;
using std::is_trivially_copy_constructible_v;
using std::is_trivially_destructible_v;
using std::is_trivially_move_assignable_v;
using std::is_trivially_move_constructible_v;
using std::remove_cv_t;
using std::remove_cvref_t;
using std::strong_ordering;
using std::three_way_comparable_with;
using std::to_address;

namespace detail {
template <typename T, typename U>
concept optional_eq_rel = requires(const T& t, const U& u) {
    { t == u } -> convertible_to<bool>;
};

template <typename T, typename U>
concept optional_ne_rel = requires(const T& t, const U& u) {
    { t != u } -> convertible_to<bool>;
};

template <typename T, typename U>
concept optional_lt_rel = requires(const T& t, const U& u) {
    { t < u } -> convertible_to<bool>;
};

template <typename T, typename U>
concept optional_gt_rel = requires(const T& t, const U& u) {
    { t > u } -> convertible_to<bool>;
};

template <typename T, typename U>
concept optional_le_rel = requires(const T& t, const U& u) {
    { t <= u } -> convertible_to<bool>;
};

template <typename T, typename U>
concept optional_ge_rel = requires(const T& t, const U& u) {
    { t >= u } -> convertible_to<bool>;
};
} // namespace detail

struct in_place_t {
    explicit in_place_t() = default;
};

inline constexpr in_place_t in_place{};

} // namespace beman::optional26

namespace beman::optional26 {
template <class T>
class optional; // partially freestanding
} // namespace beman::optional26

namespace std { // trait specializations
// Since P3168R2: Give std::optional Range Support.
template <class T>
inline constexpr bool ranges::enable_view<beman::optional26::optional<T>> = true;

// TODO: document why this is needed.
template <class T>
inline constexpr bool ranges::enable_borrowed_range<beman::optional26::optional<T&>> = true;

// Since P3168R2: Give std::optional Range Support.
#if defined(__cpp_lib_format_ranges)
template <class T>
inline constexpr auto format_kind<beman::optional26::optional<T>> = range_format::disabled;
#endif

} // namespace std

namespace beman::optional26 {
template <class T>
concept is_derived_from_optional = requires(const T& t) { // exposition only
    []<class U>(const optional<U>&) {}(t);
};

// \ref{optional.nullopt}, no-value state indicator
struct nullopt_t {
    // Used for constructing nullopt.
    enum class Tag { tag };

    // Must be constexpr for nullopt_t to be literal.
    explicit constexpr nullopt_t(Tag) noexcept {}
};

/// Tag to disengage optional objects.
inline constexpr nullopt_t nullopt{nullopt_t::Tag::tag};

// \ref{optional.bad.access}, class bad_optional_access
class bad_optional_access;

// \ref{optional.relops}, relational operators
template <typename T, typename U>
constexpr bool operator==(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_eq_rel<T, U>;
template <typename T, typename U>
constexpr bool operator!=(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_ne_rel<T, U>;
template <typename T, typename U>
constexpr bool operator<(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_lt_rel<T, U>;
template <typename T, typename U>
constexpr bool operator>(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_gt_rel<T, U>;
template <typename T, typename U>
constexpr bool operator<=(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_le_rel<T, U>;
template <typename T, typename U>
constexpr bool operator>=(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_ge_rel<T, U>;
template <class T, three_way_comparable_with<T> U>
constexpr compare_three_way_result_t<T, U> operator<=>(const optional<T>&, const optional<U>&);

// \ref{optional.nullops}, comparison with \tcode{nullopt}
template <class T>
constexpr bool operator==(const optional<T>&, nullopt_t) noexcept;
template <class T>
constexpr strong_ordering operator<=>(const optional<T>&, nullopt_t) noexcept;

// \ref{optional.comp.with.t}, comparison with \tcode{T}
template <typename T, typename U>
constexpr bool operator==(const optional<T>& lhs, const U& rhs)
    requires detail::optional_eq_rel<T, U>;
template <typename T, typename U>
constexpr bool operator==(const T& lhs, const optional<U>& rhs)
    requires detail::optional_eq_rel<T, U>;
template <typename T, typename U>
constexpr bool operator!=(const optional<T>& lhs, const U& rhs)
    requires detail::optional_ne_rel<T, U>;
template <typename T, typename U>
constexpr bool operator!=(const T& lhs, const optional<U>& rhs)
    requires detail::optional_ne_rel<T, U>;
template <typename T, typename U>
constexpr bool operator<(const optional<T>& lhs, const U& rhs)
    requires detail::optional_lt_rel<T, U>;
template <typename T, typename U>
constexpr bool operator<(const T& lhs, const optional<U>& rhs)
    requires detail::optional_lt_rel<T, U>;
template <typename T, typename U>
constexpr bool operator>(const optional<T>& lhs, const U& rhs)
    requires detail::optional_gt_rel<T, U>;
template <typename T, typename U>
constexpr bool operator>(const T& lhs, const optional<U>& rhs)
    requires detail::optional_gt_rel<T, U>;
template <typename T, typename U>
constexpr bool operator<=(const optional<T>& lhs, const U& rhs)
    requires detail::optional_le_rel<T, U>;
template <typename T, typename U>
constexpr bool operator<=(const T& lhs, const optional<U>& rhs)
    requires detail::optional_le_rel<T, U>;
template <typename T, typename U>
constexpr bool operator>=(const optional<T>& lhs, const U& rhs)
    requires detail::optional_ge_rel<T, U>;
template <typename T, typename U>
constexpr bool operator>=(const T& lhs, const optional<U>& rhs)
    requires detail::optional_ge_rel<T, U>;
template <typename T, typename U>
    requires(!is_derived_from_optional<U>) && three_way_comparable_with<T, U>
constexpr compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const U& v);

// \ref{optional.specalg}, specialized algorithms
template <class T>
constexpr void swap(optional<T>& x, optional<T>& y) noexcept(noexcept(x.swap(y)))
    requires is_move_constructible_v<T> && is_swappable_v<T>;

template <class T>
constexpr optional<decay_t<T>> make_optional(T&&) noexcept(is_nothrow_constructible_v<optional<decay_t<T>>, T>)
    requires is_constructible_v<decay_t<T>, T>;

template <class T, class... Args>
constexpr optional<T> make_optional(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>)
    requires is_constructible_v<T, Args...>;

template <class T, class U, class... Args>
constexpr optional<T>
make_optional(initializer_list<U> il,
              Args&&... args) noexcept(is_nothrow_constructible_v<T, initializer_list<U>&, Args...>)
    requires is_constructible_v<T, initializer_list<U>&, Args...>;

// \ref{optional.hash}, hash support
template <class T>
struct hash;
template <class T>
struct hash<optional<T>>;

/// END [optional.syn]

namespace detail {
template <class T, class U>
concept enable_forward_value =
    is_constructible_v<T, U&&> && !is_same_v<decay_t<U>, in_place_t> && !is_same_v<optional<T>, decay_t<U>>;

template <class T, class U, class Other>
concept enable_from_other =
    is_constructible_v<T, Other> && !is_constructible_v<T, optional<U>&> && !is_constructible_v<T, optional<U>&&> &&
    !is_constructible_v<T, const optional<U>&> && !is_constructible_v<T, const optional<U>&&> &&
    !is_convertible_v<optional<U>&, T> && !is_convertible_v<optional<U>&&, T> &&
    !is_convertible_v<const optional<U>&, T> && !is_convertible_v<const optional<U>&&, T>;

template <class T, class U>
concept enable_assign_forward =
    !is_same_v<optional<T>, decay_t<U>> && !conjunction_v<is_scalar<T>, is_same<T, decay_t<U>>> &&
    is_constructible_v<T, U> && is_assignable_v<T&, U>;

template <class T, class U, class Other>
concept enable_assign_from_other =
    is_constructible_v<T, Other> && is_assignable_v<T&, Other> && !is_constructible_v<T, optional<U>&> &&
    !is_constructible_v<T, optional<U>&&> && !is_constructible_v<T, const optional<U>&> &&
    !is_constructible_v<T, const optional<U>&&> && !is_convertible_v<optional<U>&, T> &&
    !is_convertible_v<optional<U>&&, T> && !is_convertible_v<const optional<U>&, T> &&
    !is_convertible_v<const optional<U>&&, T> && !is_assignable_v<T&, optional<U>&> &&
    !is_assignable_v<T&, optional<U>&&> && !is_assignable_v<T&, const optional<U>&> &&
    !is_assignable_v<T&, const optional<U>&&>;
} // namespace detail

namespace detail {
template <class T>
    concept is_optional = requires(const T& t) { // exposition only
    []<class U>(const optional<U>&) {}(t);
};
} // namespace detail

// 22.5.3.1 General[optional.optional.general]

template <class T>
class optional {
    static_assert((!is_same_v<T, remove_cv_t<in_place_t>>) && (!is_same_v<remove_cv_t<T>, nullopt_t>));
    static_assert(is_object_v<T> && !is_array_v<T>);

  public:
    using value_type = T;
    // Since P3168R2: Give std::optional Range Support.
    using iterator       = detail::contiguous_iterator<T, optional>; // see~\ref{optional.iterators}
    using const_iterator = detail::contiguous_iterator<const T,
                                                       optional>; // see~\ref{optional.iterators}

    // \ref{optional.ctor}, constructors
    constexpr optional() noexcept;
    constexpr optional(nullopt_t) noexcept;
    constexpr optional(const optional& rhs)
        requires is_copy_constructible_v<T> && (!is_trivially_copy_constructible_v<T>);
    constexpr optional(const optional&)
        requires is_copy_constructible_v<T> && is_trivially_copy_constructible_v<T>
    = default;
    constexpr optional(optional&& rhs) noexcept(is_nothrow_move_constructible_v<T>)
        requires is_move_constructible_v<T> && (!is_trivially_move_constructible_v<T>);
    constexpr optional(optional&&)
        requires is_move_constructible_v<T> && is_trivially_move_constructible_v<T>
    = default;

    template <class... Args>
    constexpr explicit optional(in_place_t, Args&&... args)
        requires is_constructible_v<T, Args...>;

    template <class U, class... Args>
    constexpr explicit optional(in_place_t, initializer_list<U> il, Args&&... args)
        requires is_constructible_v<T, initializer_list<U>&, Args&&...>;

    template <class U = T>
    constexpr explicit(!is_convertible_v<U, T>) optional(U&& u)
        requires detail::enable_forward_value<T, U>;

    template <class U>
    constexpr explicit(!is_convertible_v<U, T>) optional(const optional<U>& rhs)
        requires detail::enable_from_other<T, U, const U&> && is_convertible_v<const U&, T>;

    template <class U>
    constexpr explicit(!is_convertible_v<U, T>) optional(const optional<U>& rhs)
        requires detail::enable_from_other<T, U, const U&> && (!is_convertible_v<const U&, T>);

    template <class U>
    constexpr explicit(!is_convertible_v<U, T>) optional(optional<U>&& rhs)
        requires detail::enable_from_other<T, U, U&&> && is_convertible_v<U&&, T>;

    template <class U>
    constexpr explicit(!is_convertible_v<U, T>) optional(optional<U>&& rhs)
        requires detail::enable_from_other<T, U, U&&> && (!is_convertible_v<U &&, T>);

    // \ref{optional.dtor}, destructor
    constexpr ~optional()
        requires is_trivially_destructible_v<T>
    = default;

    constexpr ~optional()
        requires(!is_trivially_destructible_v<T>);

    // \ref{optional.assign}, assignment
    constexpr optional& operator=(const optional& rhs)
        requires is_copy_constructible_v<T> && is_copy_assignable_v<T> && (!is_trivially_copy_assignable_v<T>);

    constexpr optional& operator=(const optional&)
        requires is_copy_constructible_v<T> && is_copy_assignable_v<T> && is_trivially_copy_constructible_v<T> &&
                     is_trivially_copy_assignable_v<T>
    = default;

    constexpr optional& operator=(optional&& rhs) noexcept(is_nothrow_move_constructible_v<T>)
        requires is_move_constructible_v<T> && is_move_assignable_v<T> && (!is_trivially_move_assignable_v<T>);

    constexpr optional& operator=(optional&&)
        requires is_move_constructible_v<T> && is_move_assignable_v<T> && is_trivially_move_constructible_v<T> &&
                     is_trivially_move_assignable_v<T>
    = default;

    template <class U = T>
    constexpr optional& operator=(U&& u)
        requires detail::enable_assign_forward<T, U>;

    template <class U>
    constexpr optional& operator=(const optional<U>& rhs)
        requires detail::enable_assign_from_other<T, U, const U&>;

    template <class U>
    constexpr optional& operator=(optional<U>&& rhs)
        requires detail::enable_assign_from_other<T, U, U>;

    template <class... Args>
    constexpr T& emplace(Args&&... args);

    template <class U, class... Args>
    constexpr T& emplace(initializer_list<U> il, Args&&... args);

    // \ref{optional.swap}, swap
    constexpr void swap(optional& rhs) noexcept(is_nothrow_move_constructible<T>::value &&
                                                is_nothrow_swappable<T>::value);

    // \ref{optional.iterators}, iterator support
    constexpr iterator       begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator       end() noexcept;
    constexpr const_iterator end() const noexcept;

    // \ref{optional.observe}, observers
    constexpr const T* operator->() const;
    constexpr T*       operator->();
    constexpr T&       operator*() &;
    constexpr const T& operator*() const&;
    constexpr T&&      operator*() &&;
    constexpr explicit operator bool() const noexcept;
    constexpr bool     has_value() const noexcept;
    constexpr T&       value() &;
    constexpr const T& value() const&;
    constexpr T&&      value() &&;
    template <class U>
    constexpr T value_or(U&& u) const&;
    template <class U>
    constexpr T value_or(U&& u) &&;

    // \ref{optional.monadic}, monadic operations
    template <class F>
    constexpr auto and_then(F&& f) &;
    template <class F>
    constexpr auto and_then(F&& f) &&;
    template <class F>
    constexpr auto and_then(F&& f) const&;
    template <class F>
    constexpr auto and_then(F&& f) const&&;
    template <class F>
    constexpr auto transform(F&& f) &;
    template <class F>
    constexpr auto transform(F&& f) &&;
    template <class F>
    constexpr auto transform(F&& f) const&;
    template <class F>
    constexpr auto transform(F&& f) const&&;
    template <class F>
    constexpr optional or_else(F&& f) const&;
    template <class F>
    constexpr optional or_else(F&& f) &&;

    // \ref{optional.mod}, modifiers
    constexpr void reset() noexcept;

  private:
    struct empty {};
    union {
        empty _{};
        T     value_;
    };
    bool engaged_ = false;

    template <class... Args>
    constexpr void construct(Args&&... args) {
        construct_at(addressof(value_), std::forward<Args>(args)...);
        engaged_ = true;
    }

    constexpr void hard_reset() noexcept {
        destroy_at(addressof(value_));
        engaged_ = false;
    }
};

class bad_optional_access : public std::exception {
  public:
    bad_optional_access() = default;
    const char* what() const noexcept { return "Optional has no value"; }
};

// \rSec3[optional.ctor]{Constructors}
template <class T>
inline constexpr optional<T>::optional() noexcept : _(), engaged_(false) {}

template <class T>
inline constexpr optional<T>::optional(nullopt_t) noexcept {}

template <class T>
inline constexpr optional<T>::optional(const optional& rhs)
    requires is_copy_constructible_v<T> && (!is_trivially_copy_constructible_v<T>)
{
    if (rhs.has_value()) {

        construct_at(addressof(value_), rhs.value_);
        engaged_ = true;
    }
}

template <class T>
inline constexpr optional<T>::optional(optional&& rhs) noexcept(is_nothrow_move_constructible_v<T>)
    requires is_move_constructible_v<T> && (!is_trivially_move_constructible_v<T>)
{
    if (rhs.has_value()) {
        construct_at(addressof(value_), std::move(rhs.value()));
        engaged_ = true;
    }
}

/// Constructs the stored value in-place using the given arguments.
template <class T>
template <class... Args>
inline constexpr optional<T>::optional(in_place_t, Args&&... args)
    requires is_constructible_v<T, Args...>
    : value_(std::forward<Args>(args)...), engaged_(true) {}

template <class T>
template <class U, class... Args>
inline constexpr optional<T>::optional(in_place_t, initializer_list<U> il, Args&&... args)
    requires is_constructible_v<T, initializer_list<U>&, Args&&...>
    : value_(il, std::forward<Args>(args)...), engaged_(true) {}

/// Constructs the stored value with `u`.
template <class T>
template <class U>
inline constexpr optional<T>::optional(U&& u)
    requires detail::enable_forward_value<T, U> //&& is_convertible_v<U&&, T>
    : optional(in_place, std::forward<U>(u)) {}

/// Converting copy constructor.
template <class T>
template <class U>
inline constexpr optional<T>::optional(const optional<U>& rhs)
    requires detail::enable_from_other<T, U, const U&> && is_convertible_v<const U&, T>
{
    if (rhs.has_value()) {
        construct_at(addressof(value_), rhs.value());
        engaged_ = true;
    }
}

template <class T>
template <class U>
inline constexpr optional<T>::optional(const optional<U>& rhs)
    requires detail::enable_from_other<T, U, const U&> && (!is_convertible_v<const U&, T>)
{
    if (rhs.has_value()) {
        construct(*rhs);
    }
}

/// Converting move constructor.
template <class T>
template <class U>
inline constexpr optional<T>::optional(optional<U>&& rhs)
    requires detail::enable_from_other<T, U, U&&> && is_convertible_v<U&&, T>
{
    if (rhs.has_value()) {
        construct(std::move(*rhs));
    }
}

template <class T>
template <class U>
inline constexpr optional<T>::optional(optional<U>&& rhs)
    requires detail::enable_from_other<T, U, U&&> && (!is_convertible_v<U &&, T>)
{
    if (rhs.has_value()) {
        construct(std::move(*rhs));
    }
}

// 22.5.3.3 Destructor[optional.dtor]

template <class T>
inline constexpr optional<T>::~optional()
    requires(!is_trivially_destructible_v<T>)
{
    if (has_value())
        destroy_at(addressof(value_));
}

// 22.5.3.4 Assignment[optional.assign]

template <class T>
inline constexpr optional<T>& optional<T>::operator=(const optional<T>& rhs)
    requires is_copy_constructible_v<T> && is_copy_assignable_v<T> && (!is_trivially_copy_assignable_v<T>)
{
    if (!rhs.has_value())
        reset();
    else if (has_value())
        value_ = rhs.value_;
    else
        construct_at(addressof(value_), rhs.value_);
    return *this;
}

template <class T>
inline constexpr optional<T>& optional<T>::operator=(optional<T>&& rhs) noexcept(is_nothrow_move_constructible_v<T>)
    requires is_move_constructible_v<T> && is_move_assignable_v<T> && (!is_trivially_move_assignable_v<T>)
{
    if (!rhs.has_value())
        reset();
    else if (has_value())
        value_ = std::move(rhs.value_);
    else
        construct_at(addressof(value_), std::move(rhs.value_));
    return *this;
}

/// Assigns the stored value from `u`, destroying the old value if there
/// was one.
template <class T>
template <class U>
inline constexpr optional<T>& optional<T>::operator=(U&& u)
    requires detail::enable_assign_forward<T, U>
{
    if (has_value()) {
        value_ = std::forward<U>(u);
    } else {
        construct(std::forward<U>(u));
    }

    return *this;
}

/// Converting copy assignment operator.
///
/// Copies the value from `rhs` if there is one. Otherwise resets the
/// stored value in `*this`.
template <class T>
template <class U>
inline constexpr optional<T>& optional<T>::operator=(const optional<U>& rhs)
    requires detail::enable_assign_from_other<T, U, const U&>
{
    if (has_value()) {
        if (rhs.has_value()) {
            value_ = *rhs;
        } else {
            hard_reset();
        }
    }

    else if (rhs.has_value()) {
        construct(*rhs);
    }

    return *this;
}

/// Converting move assignment operator.
///
/// Moves the value from `rhs` if there is one. Otherwise resets the stored
/// value in `*this`.
template <class T>
template <class U>
inline constexpr optional<T>& optional<T>::operator=(optional<U>&& rhs)
    requires detail::enable_assign_from_other<T, U, U>
{
    if (has_value()) {
        if (rhs.has_value()) {
            value_ = std::move(*rhs);
        } else {
            hard_reset();
        }
    }

    else if (rhs.has_value()) {
        construct(std::move(*rhs));
    }

    return *this;
}

/// Constructs the value in-place, destroying the current one if there is
/// one.
template <class T>
template <class... Args>
constexpr T& optional<T>::emplace(Args&&... args) {
    static_assert(is_constructible_v<T, Args&&...>);
    *this = nullopt;
    construct(std::forward<Args>(args)...);
    return value();
}

template <class T>
template <class U, class... Args>
constexpr T& optional<T>::emplace(initializer_list<U> il, Args&&... args) {
    static_assert(is_constructible_v<T, initializer_list<U>&, Args&&...>);
    *this = nullopt;
    construct(il, std::forward<Args>(args)...);
    return value();
}

// 22.5.3.5 Swap[optional.swap]
/// Swaps this optional with the other.
///
/// If neither optionals have a value, nothing happens.
/// If both have a value, the values are swapped.
/// If one has a value, it is moved to the other and the movee is left
/// valueless.
template <class T>
inline constexpr void optional<T>::swap(optional<T>& rhs) noexcept(is_nothrow_move_constructible<T>::value &&
                                                                   is_nothrow_swappable<T>::value) {
    static_assert(is_move_constructible_v<T>);
    using std::swap;
    if (has_value()) {
        if (rhs.has_value()) {
            swap(value(), *rhs);
        } else {
            construct_at(addressof(rhs.value_), std::move(value_));
            value_.T::~T();
        }
    } else if (rhs.has_value()) {
        construct_at(addressof(value_), std::move(rhs.value_));
        rhs.value_.T::~T();
    }
    swap(engaged_, rhs.engaged_);
}

// 22.5.3.6 Iterator support[optional.iterators]
// Since P3168R2: Give std::optional Range Support.
template <class T>
inline constexpr optional<T>::iterator optional<T>::begin() noexcept {
    return iterator(has_value() ? addressof(value_) : nullptr);
}

template <class T>
inline constexpr optional<T>::const_iterator optional<T>::begin() const noexcept {
    return const_iterator(has_value() ? addressof(value_) : nullptr);
}
template <class T>
inline constexpr optional<T>::iterator optional<T>::end() noexcept {
    return begin() + has_value();
}

template <class T>
inline constexpr optional<T>::const_iterator optional<T>::end() const noexcept {
    return begin() + has_value();
}

// 22.5.3.7 Observers[optional.observe]

/// Returns a pointer to the stored value
template <class T>
inline constexpr const T* optional<T>::operator->() const {
    return addressof(value_);
}

template <class T>
inline constexpr T* optional<T>::operator->() {
    return addressof(value_);
}

/// Returns the stored value
template <class T>
inline constexpr T& optional<T>::operator*() & {
    return value_;
}

template <class T>
inline constexpr const T& optional<T>::operator*() const& {
    return value_;
}

template <class T>
inline constexpr T&& optional<T>::operator*() && {
    return std::move(value_);
}

template <class T>
inline constexpr optional<T>::operator bool() const noexcept {
    return engaged_;
}

/// Returns whether or not the optional has a value
template <class T>
inline constexpr bool optional<T>::has_value() const noexcept {
    return engaged_;
}

/// Returns the contained value if there is one, otherwise throws
/// bad_optional_access
template <class T>
inline constexpr T& optional<T>::value() & {
    if (has_value())
        return value_;
    throw bad_optional_access();
}
template <class T>
inline constexpr const T& optional<T>::value() const& {
    if (has_value())
        return value_;
    throw bad_optional_access();
}
template <class T>
inline constexpr T&& optional<T>::value() && {
    if (has_value())
        return std::move(value_);
    throw bad_optional_access();
}

/// Returns the stored value if there is one, otherwise returns `u`
template <class T>
template <class U>
inline constexpr T optional<T>::value_or(U&& u) const& {
    static_assert(is_copy_constructible_v<T> && is_convertible_v<U&&, T>);
    return has_value() ? value() : static_cast<T>(std::forward<U>(u));
}

template <class T>
template <class U>
inline constexpr T optional<T>::value_or(U&& u) && {
    static_assert(is_move_constructible_v<T>);
    static_assert(is_convertible_v<decltype(u), T>, "Must be able to convert u to T");
    return has_value() ? std::move(value()) : static_cast<T>(std::forward<U>(u));
}

// 22.5.3.8 Monadic operations[optional.monadic]
template <class T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) & {
    using U = invoke_result_t<F, T&>;
    static_assert(detail::is_optional<remove_cvref_t<U>>);
    if (has_value()) {
        return invoke(std::forward<F>(f), value_);
    } else {
        return remove_cvref_t<U>();
    }
}

template <class T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) && {
    using U = invoke_result_t<F, T&&>;
    static_assert(detail::is_optional<remove_cvref_t<U>>);
    if (has_value()) {
        return invoke(std::forward<F>(f), std::move(value_));
    } else {
        return remove_cvref_t<U>();
    }
}

template <class T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) const& {
    using U = invoke_result_t<F, const T&>;
    static_assert(detail::is_optional<remove_cvref_t<U>>);
    if (has_value()) {
        return invoke(std::forward<F>(f), value_);
    } else {
        return remove_cvref_t<U>();
    }
}

template <class T>
template <class F>
constexpr auto optional<T>::and_then(F&& f) const&& {
    using U = invoke_result_t<F, const T&&>;
    static_assert(detail::is_optional<remove_cvref_t<U>>);
    if (has_value()) {
        return invoke(std::forward<F>(f), std::move(value_));
    } else {
        return remove_cvref_t<U>();
    }
}

/// Carries out some operation on the stored object if there is one.
template <class T>
template <class F>
constexpr auto optional<T>::transform(F&& f) & {
    using U = invoke_result_t<F, T&>;
    static_assert(!is_array_v<U>);
    static_assert(!is_same_v<U, in_place_t>);
    static_assert(!is_same_v<U, nullopt_t>);
    static_assert(is_object_v<U> || is_reference_v<U>); /// References now allowed
    return (has_value()) ? optional<U>{invoke(std::forward<F>(f), value_)} : optional<U>{};
}

template <class T>
template <class F>
constexpr auto optional<T>::transform(F&& f) && {
    using U = invoke_result_t<F, T&&>;
    static_assert(!is_array_v<U>);
    static_assert(!is_same_v<U, in_place_t>);
    static_assert(!is_same_v<U, nullopt_t>);
    static_assert(is_object_v<U> || is_reference_v<U>); /// References now allowed
    return (has_value()) ? optional<U>{invoke(std::forward<F>(f), std::move(value_))} : optional<U>{};
}

template <class T>
template <class F>
constexpr auto optional<T>::transform(F&& f) const& {
    using U = invoke_result_t<F, const T&>;
    static_assert(!is_array_v<U>);
    static_assert(!is_same_v<U, in_place_t>);
    static_assert(!is_same_v<U, nullopt_t>);
    static_assert(is_object_v<U> || is_reference_v<U>); /// References now allowed
    return (has_value()) ? optional<U>{invoke(std::forward<F>(f), value_)} : optional<U>{};
}

template <class T>
template <class F>
constexpr auto optional<T>::transform(F&& f) const&& {
    using U = invoke_result_t<F, const T&>;
    static_assert(!is_array_v<U>);
    static_assert(!is_same_v<U, in_place_t>);
    static_assert(!is_same_v<U, nullopt_t>);
    static_assert(is_object_v<U> || is_reference_v<U>); /// References now allowed
    return (has_value()) ? optional<U>{invoke(std::forward<F>(f), value_)} : optional<U>{};
}

/// Calls `f` if the optional is empty
template <class T>
template <class F>
constexpr optional<T> optional<T>::or_else(F&& f) const& {
    static_assert(is_same_v<remove_cvref_t<invoke_result_t<F>>, optional>);
    if (has_value())
        return value_;

    return std::forward<F>(f)();
}

template <class T>
template <class F>
constexpr optional<T> optional<T>::or_else(F&& f) && {
    static_assert(is_same_v<remove_cvref_t<invoke_result_t<F>>, optional>);
    if (has_value())
        return std::move(value_);

    return std::forward<F>(f)();
}

// 22.5.3.9 Modifiers[optional.mod]
template <class T>
constexpr void optional<T>::reset() noexcept {
    if constexpr (!is_trivially_destructible_v<T>) {
        if (has_value())
            value_.~T();
    }
    engaged_ = false;
}

// 22.5.4 No-value state indicator[optional.nullopt]

// 22.5.5 Class bad_optional_access[optional.bad.access]

// 22.5.6 Relational operators[optional.relops]
template <typename T, typename U>
constexpr bool operator==(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_eq_rel<T, U>
{
    return static_cast<bool>(lhs) == static_cast<bool>(rhs) && (!lhs || *lhs == *rhs);
}

template <typename T, typename U>
constexpr bool operator!=(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_ne_rel<T, U>
{
    return static_cast<bool>(lhs) != static_cast<bool>(rhs) || (static_cast<bool>(lhs) && *lhs != *rhs);
}

template <typename T, typename U>
constexpr bool operator<(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_lt_rel<T, U>
{
    return static_cast<bool>(rhs) && (!lhs || *lhs < *rhs);
}

template <typename T, typename U>
constexpr bool operator>(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_gt_rel<T, U>
{
    return static_cast<bool>(lhs) && (!rhs || *lhs > *rhs);
}

template <typename T, typename U>
constexpr bool operator<=(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_le_rel<T, U>
{
    return !lhs || (static_cast<bool>(rhs) && *lhs <= *rhs);
}

template <typename T, typename U>
constexpr bool operator>=(const optional<T>& lhs, const optional<U>& rhs)
    requires detail::optional_ge_rel<T, U>
{
    return !rhs || (static_cast<bool>(lhs) && *lhs >= *rhs);
}

template <typename T, three_way_comparable_with<T> U>
constexpr compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const optional<U>& y) {
    return x && y ? *x <=> *y : bool(x) <=> bool(y);
}

// 22.5.7 Comparison with nullopt[optional.nullops]
template <class T>
constexpr bool operator==(const optional<T>& lhs, nullopt_t) noexcept {
    return !lhs;
}

template <class T>
constexpr strong_ordering operator<=>(const optional<T>& x, nullopt_t) noexcept {
    return bool(x) <=> false;
}

// 22.5.8 Comparison with T[optional.comp.with.t]
template <typename T, typename U>
constexpr bool operator==(const optional<T>& lhs, const U& rhs)
    requires detail::optional_eq_rel<T, U>
{
    return lhs && *lhs == rhs;
}

template <typename T, typename U>
constexpr bool operator==(const T& lhs, const optional<U>& rhs)
    requires detail::optional_eq_rel<T, U>
{
    return rhs && lhs == *rhs;
}

template <typename T, typename U>
constexpr bool operator!=(const optional<T>& lhs, const U& rhs)
    requires detail::optional_ne_rel<T, U>
{
    return !lhs || *lhs != rhs;
}

template <typename T, typename U>
constexpr bool operator!=(const T& lhs, const optional<U>& rhs)
    requires detail::optional_ne_rel<T, U>
{
    return !rhs || lhs != *rhs;
}

template <typename T, typename U>
constexpr bool operator<(const optional<T>& lhs, const U& rhs)
    requires detail::optional_lt_rel<T, U>
{
    return !lhs || *lhs < rhs;
}

template <typename T, typename U>
constexpr bool operator<(const T& lhs, const optional<U>& rhs)
    requires detail::optional_lt_rel<T, U>
{
    return rhs && lhs < *rhs;
}

template <typename T, typename U>
constexpr bool operator>(const optional<T>& lhs, const U& rhs)
    requires detail::optional_gt_rel<T, U>
{
    return lhs && *lhs > rhs;
}

template <typename T, typename U>
constexpr bool operator>(const T& lhs, const optional<U>& rhs)
    requires detail::optional_gt_rel<T, U>
{
    return !rhs || lhs > *rhs;
}

template <typename T, typename U>
constexpr bool operator<=(const optional<T>& lhs, const U& rhs)
    requires detail::optional_le_rel<T, U>
{
    return !lhs || *lhs <= rhs;
}

template <typename T, typename U>
constexpr bool operator<=(const T& lhs, const optional<U>& rhs)
    requires detail::optional_le_rel<T, U>
{
    return rhs && lhs <= *rhs;
}

template <typename T, typename U>
constexpr bool operator>=(const optional<T>& lhs, const U& rhs)
    requires detail::optional_ge_rel<T, U>
{
    return lhs && *lhs >= rhs;
}

template <typename T, typename U>
constexpr bool operator>=(const T& lhs, const optional<U>& rhs)
    requires detail::optional_ge_rel<T, U>
{
    return !rhs || lhs >= *rhs;
}

template <typename T, typename U>
    requires(!is_derived_from_optional<U>) && three_way_comparable_with<T, U>
constexpr compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const U& v) {
    return bool(x) ? *x <=> v : strong_ordering::less;
}

// 22.5.9 Specialized algorithms[optional.specalg]

template <class T>
constexpr void swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    requires is_move_constructible_v<T> && is_swappable_v<T>
{
    return lhs.swap(rhs);
}

template <class T>
constexpr optional<decay_t<T>> make_optional(T&& t) noexcept(is_nothrow_constructible_v<optional<decay_t<T>>, T>)
    requires is_constructible_v<decay_t<T>, T>
{
    return optional<decay_t<T>>{std::forward<T>(t)};
}

template <typename T, typename... Args>
constexpr optional<T> make_optional(Args&&... args) noexcept(is_nothrow_constructible_v<T, Args...>)
    requires is_constructible_v<T, Args...>
{
    return optional<T>{in_place, std::forward<Args>(args)...};
}

template <typename T, typename U, typename... Args>
constexpr optional<T>
make_optional(initializer_list<U> init_list,
              Args&&... args) noexcept(is_nothrow_constructible_v<T, initializer_list<U>&, Args...>)
    requires is_constructible_v<T, initializer_list<U>&, Args...>
{
    return optional<T>{in_place, init_list, std::forward<Args>(args)...};
}

/****************/
/* optional<T&> */
/****************/

template <class T>
class optional<T&> {
  public:
    using value_type     = T&;
    using iterator       = detail::contiguous_iterator<T, optional>; // see [optionalref.iterators]
    using const_iterator = detail::contiguous_iterator<const T,
                                                       optional>; // see [optionalref.iterators]

  public:
    // \ref{optionalref.ctor}, constructors

    constexpr optional() noexcept;
    constexpr optional(nullopt_t) noexcept;
    constexpr optional(const optional& rhs) noexcept = default;
    constexpr optional(optional&& rhs) noexcept      = default;
    template <class U = T>
        requires(!detail::is_optional<decay_t<U>>)
    constexpr explicit(!is_convertible_v<U, T>) optional(U&& u) noexcept;
    template <class U>
    constexpr explicit(!is_convertible_v<U, T>) optional(const optional<U>& rhs) noexcept;

    // \ref{optionalref.dtor}, destructor
    constexpr ~optional() = default;

    // \ref{optionalref.assign}, assignment
    constexpr optional& operator=(nullopt_t) noexcept;

    constexpr optional& operator=(const optional& rhs) noexcept = default;
    constexpr optional& operator=(optional&& rhs) noexcept      = default;

    template <class U = T>
        requires(!detail::is_optional<decay_t<U>>)
    constexpr optional& operator=(U&& u);

    template <class U>
    constexpr optional& operator=(const optional<U>& rhs) noexcept;

    template <class U>
    constexpr optional& operator=(optional<U>&& rhs) = delete;

    template <class U>
        requires(!detail::is_optional<decay_t<U>>)
    constexpr optional& emplace(U&& u) noexcept;

    // \ref{optionalref.swap}, swap
    constexpr void swap(optional& rhs) noexcept;

    // \ref{optional.iterators}, iterator support
    constexpr iterator       begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator       end() noexcept;
    constexpr const_iterator end() const noexcept;

    // \ref{optionalref.observe}, observers
    constexpr T*       operator->() const noexcept;
    constexpr T&       operator*() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr bool     has_value() const noexcept;
    constexpr T&       value() const;
    template <class U>
    constexpr T value_or(U&& u) const;

    // \ref{optionalref.monadic}, monadic operations
    template <class F>
    constexpr auto and_then(F&& f) const;
    template <class F>
    constexpr auto transform(F&& f) const -> optional<invoke_result_t<F, T&>>;
    template <class F>
    constexpr optional or_else(F&& f) const;

    // \ref{optional.mod}, modifiers
    constexpr void reset() noexcept;

  private:
    T* value_; // exposition only
};

//  \rSec3[optionalref.ctor]{Constructors}
template <class T>
constexpr optional<T&>::optional() noexcept : value_(nullptr) {}

template <class T>
constexpr optional<T&>::optional(nullopt_t) noexcept : value_(nullptr) {}

template <class T>
template <class U>
    requires(!detail::is_optional<decay_t<U>>)
constexpr optional<T&>::optional(U&& u) noexcept : value_(addressof(u)) {
    static_assert(is_constructible_v<add_lvalue_reference_t<T>, U>, "Must be able to bind U to T&");
    static_assert(is_lvalue_reference<U>::value, "U must be an lvalue");
}

template <class T>
template <class U>
constexpr optional<T&>::optional(const optional<U>& rhs) noexcept {
    static_assert(is_constructible_v<add_lvalue_reference_t<T>, U>, "Must be able to bind U to T&");
    if (rhs.has_value())
        value_ = to_address(rhs);
    else
        value_ = nullptr;
}

// \rSec3[optionalref.assign]{Assignment}
template <class T>
constexpr optional<T&>& optional<T&>::operator=(nullopt_t) noexcept {
    value_ = nullptr;
    return *this;
}

template <class T>
template <class U>
    requires(!detail::is_optional<decay_t<U>>)
constexpr optional<T&>& optional<T&>::operator=(U&& u) {
    static_assert(is_constructible_v<add_lvalue_reference_t<T>, U>, "Must be able to bind U to T&");
    static_assert(is_lvalue_reference<U>::value, "U must be an lvalue");
    value_ = addressof(u);
    return *this;
}

template <class T>
template <class U>
constexpr optional<T&>& optional<T&>::operator=(const optional<U>& rhs) noexcept {
    static_assert(is_constructible_v<add_lvalue_reference_t<T>, U>, "Must be able to bind U to T&");
    if (rhs.has_value())
        value_ = to_address(rhs);
    else
        value_ = nullptr;
    return *this;
}

template <class T>
template <class U>
    requires(!detail::is_optional<decay_t<U>>)
constexpr optional<T&>& optional<T&>::emplace(U&& u) noexcept {
    return *this = std::forward<U>(u);
}

//   \rSec3[optionalref.swap]{Swap}

template <class T>
constexpr void optional<T&>::swap(optional<T&>& rhs) noexcept {
    std::swap(value_, rhs.value_);
}

// \rSec3[optionalref.iterators]{Iterator Support}
template <class T>
constexpr optional<T&>::iterator optional<T&>::begin() noexcept {
    return iterator(has_value() ? value_ : nullptr);
};

template <class T>
constexpr optional<T&>::const_iterator optional<T&>::begin() const noexcept {
    return const_iterator(has_value() ? value_ : nullptr);
};

template <class T>
constexpr optional<T&>::iterator optional<T&>::end() noexcept {
    return begin() + has_value();
}

template <class T>
constexpr optional<T&>::const_iterator optional<T&>::end() const noexcept {
    return begin() + has_value();
}

// \rSec3[optionalref.observe]{Observers}
template <class T>
constexpr T* optional<T&>::operator->() const noexcept {
    return value_;
}

template <class T>
constexpr T& optional<T&>::operator*() const noexcept {
    return *value_;
}

template <class T>
constexpr optional<T&>::operator bool() const noexcept {
    return value_ != nullptr;
}
template <class T>
constexpr bool optional<T&>::has_value() const noexcept {
    return value_ != nullptr;
}

template <class T>
constexpr T& optional<T&>::value() const {
    if (has_value())
        return *value_;
    throw bad_optional_access();
}

template <class T>
template <class U>
constexpr T optional<T&>::value_or(U&& u) const {
    static_assert(is_copy_constructible_v<T>, "T must be copy constructible");
    static_assert(is_convertible_v<decltype(u), T>, "Must be able to convert u to T");
    return has_value() ? *value_ : std::forward<U>(u);
}

//   \rSec3[optionalref.monadic]{Monadic operations}
template <class T>
template <class F>
constexpr auto optional<T&>::and_then(F&& f) const {
    using U = invoke_result_t<F, T&>;
    static_assert(detail::is_optional<U>, "F must return an optional");
    return (has_value()) ? invoke(std::forward<F>(f), *value_) : remove_cvref_t<U>();
}

template <class T>
template <class F>
constexpr auto optional<T&>::transform(F&& f) const -> optional<invoke_result_t<F, T&>> {
    using U = invoke_result_t<F, T&>;
    return (has_value()) ? optional<U>{invoke(std::forward<F>(f), *value_)} : optional<U>{};
}

template <class T>
template <class F>
constexpr optional<T&> optional<T&>::or_else(F&& f) const {
    using U = invoke_result_t<F>;
    static_assert(is_same_v<remove_cvref_t<U>, optional>);
    return has_value() ? *value_ : std::forward<F>(f)();
}

// \rSec3[optional.mod]{modifiers}
template <class T>
constexpr void optional<T&>::reset() noexcept {
    value_ = nullptr;
}

/*****************/
/* optional<T&&> */
/*****************/

template <class T>
class optional<T&&> {
  public:
    using value_type     = T&&;
    using iterator       = detail::contiguous_iterator<T, optional>; // see [optionalrref.iterators]
    using const_iterator = detail::contiguous_iterator<const T,
                                                       optional>; // see [optionalrref.iterators]

  public:
    // \ref{optionalrref.ctor}, constructors

    constexpr optional() noexcept;
    constexpr optional(nullopt_t) noexcept;
    constexpr optional(const optional& rhs) noexcept = default;
    constexpr optional(optional&& rhs) noexcept      = default;
    template <class U = T>
    requires(!detail::is_optional<decay_t<U>>)
    constexpr explicit(!is_convertible_v<U, T>) optional(U&& u) noexcept;
    template <class U>
    constexpr explicit(!is_convertible_v<U, T>) optional(const optional<U>& rhs) noexcept;

    // \ref{optionalrref.dtor}, destructor
    constexpr ~optional() = default;

    // \ref{optionalrref.assign}, assignment
    constexpr optional& operator=(nullopt_t) noexcept;

    constexpr optional& operator=(const optional& rhs) noexcept = default;
    constexpr optional& operator=(optional&& rhs) noexcept      = default;

    template <class U = T>
    requires(!detail::is_optional<decay_t<U>>)
    constexpr optional& operator=(U&& u);

    template <class U>
    constexpr optional& operator=(const optional<U>& rhs) noexcept;

    template <class U>
    constexpr optional& operator=(optional<U>&& rhs) = delete;

    template <class U>
    requires(!detail::is_optional<decay_t<U>>)
    constexpr optional& emplace(U&& u) noexcept;

    // \ref{optionalrref.swap}, swap
    constexpr void swap(optional& rhs) noexcept;

    // \ref{optional.iterators}, iterator support
    constexpr iterator       begin() noexcept;
    constexpr const_iterator begin() const noexcept;
    constexpr iterator       end() noexcept;
    constexpr const_iterator end() const noexcept;

    // \ref{optionalrref.observe}, observers
    constexpr T*       operator->() const noexcept;
    constexpr T&&      operator*() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr bool     has_value() const noexcept;
    constexpr T&&      value() const;
    template <class U>
    constexpr T value_or(U&& u) const;

    // \ref{optionalrref.monadic}, monadic operations
    template <class F>
    constexpr auto and_then(F&& f) const;
    template <class F>
    constexpr auto transform(F&& f) const -> optional<invoke_result_t<F, T&&>>;
    template <class F>
    constexpr optional or_else(F&& f) const;

    // \ref{optional.mod}, modifiers
    constexpr void reset() noexcept;

  private:
    T* value_; // exposition only
};

//  \rSec3[optionalrref.ctor]{Constructors}
template <class T>
constexpr optional<T&&>::optional() noexcept : value_(nullptr) {}

template <class T>
constexpr optional<T&&>::optional(nullopt_t) noexcept : value_(nullptr) {}

template <class T>
template <class U>
requires(!detail::is_optional<decay_t<U>>)
constexpr optional<T&&>::optional(U&& u) noexcept : value_(addressof(u)) {
    static_assert(is_constructible_v<add_rvalue_reference_t<T>, U>, "Must be able to bind U to T&&");
}

template <class T>
template <class U>
constexpr optional<T&&>::optional(const optional<U>& rhs) noexcept {
    static_assert(is_constructible_v<add_rvalue_reference_t<T>, U>, "Must be able to bind U to T&&");
    if (rhs.has_value())
        value_ = to_address(rhs);
    else
        value_ = nullptr;
}

// \rSec3[optionalrref.assign]{Assignment}
template <class T>
constexpr optional<T&&>& optional<T&&>::operator=(nullopt_t) noexcept {
    value_ = nullptr;
    return *this;
}

template <class T>
template <class U>
requires(!detail::is_optional<decay_t<U>>)
constexpr optional<T&&>& optional<T&&>::operator=(U&& u) {
    static_assert(is_constructible_v<add_rvalue_reference_t<T>, U>, "Must be able to bind U to T&&");
    value_ = addressof(u);
    return *this;
}

template <class T>
template <class U>
constexpr optional<T&&>& optional<T&&>::operator=(const optional<U>& rhs) noexcept {
    static_assert(is_constructible_v<add_rvalue_reference_t<T>, U>, "Must be able to bind U to T&");
    if (rhs.has_value())
        value_ = to_address(rhs);
    else
        value_ = nullptr;
    return *this;
}

template <class T>
template <class U>
requires(!detail::is_optional<decay_t<U>>)
constexpr optional<T&&>& optional<T&&>::emplace(U&& u) noexcept {
    return *this = std::forward<U>(u);
}

//   \rSec3[optionalrref.swap]{Swap}

template <class T>
constexpr void optional<T&&>::swap(optional<T&&>& rhs) noexcept {
    std::swap(value_, rhs.value_);
}

// \rSec3[optionalrref.iterators]{Iterator Support}
template <class T>
constexpr optional<T&&>::iterator optional<T&&>::begin() noexcept {
    return iterator(has_value() ? value_ : nullptr);
};

template <class T>
constexpr optional<T&&>::const_iterator optional<T&&>::begin() const noexcept {
    return const_iterator(has_value() ? value_ : nullptr);
};

template <class T>
constexpr optional<T&&>::iterator optional<T&&>::end() noexcept {
    return begin() + has_value();
}

template <class T>
constexpr optional<T&&>::const_iterator optional<T&&>::end() const noexcept {
    return begin() + has_value();
}

// \rSec3[optionalrref.observe]{Observers}
template <class T>
constexpr T* optional<T&&>::operator->() const noexcept {
    return value_;
}

template <class T>
constexpr T&& optional<T&&>::operator*() const noexcept {
    return std::move(*value_);
}

template <class T>
constexpr optional<T&&>::operator bool() const noexcept {
    return value_ != nullptr;
}
template <class T>
constexpr bool optional<T&&>::has_value() const noexcept {
    return value_ != nullptr;
}

template <class T>
constexpr T&& optional<T&&>::value() const {
    if (has_value())
        return std::move(*value_);
    throw bad_optional_access();
}

template <class T>
template <class U>
constexpr T optional<T&&>::value_or(U&& u) const {
    static_assert(is_copy_constructible_v<T>, "T must be copy constructible");
    static_assert(is_convertible_v<decltype(u), T>, "Must be able to bind u to T");
    return has_value() ? std::move(*value_) : std::forward<U>(u);
}

//   \rSec3[optionalrref.monadic]{Monadic operations}
template <class T>
template <class F>
constexpr auto optional<T&&>::and_then(F&& f) const {
    using U = invoke_result_t<F, T&&>;
    static_assert(detail::is_optional<U>, "F must return an optional");
    return (has_value()) ? invoke(std::forward<F>(f), std::move(*value_)) : remove_cvref_t<U>();
}

template <class T>
template <class F>
constexpr auto optional<T&&>::transform(F&& f) const -> optional<invoke_result_t<F, T&&>> {
    using U = invoke_result_t<F, T&&>;
    return (has_value()) ? optional<U>{invoke(std::forward<F>(f), std::move(*value_))} : optional<U>{};
}

template <class T>
template <class F>
constexpr optional<T&&> optional<T&&>::or_else(F&& f) const {
    using U = invoke_result_t<F>;
    static_assert(is_same_v<remove_cvref_t<U>, optional>);
    return has_value() ? std::move(*value_) : std::forward<F>(f)();
}

// \rSec3[optional.mod]{modifiers}
template <class T>
constexpr void optional<T&&>::reset() noexcept {
    value_ = nullptr;
}
} // namespace beman::optional26



#endif // BEMAN_OPTIONAL26_OPTIONAL_HPP
