// include/Beman/Optional26/optional.hpp -*-C++-*-
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

#ifndef BEMAN_OPTIONAL26_OPTIONAL_HPP
#define BEMAN_OPTIONAL26_OPTIONAL_HPP

/*
22.5.2 Header <optional> synopsis[optional.syn]

#include <compare>              // see [compare.syn]

namespace std {
  // [optional.optional], class template optional
  template<class T>
    class optional;

  template<class T>
    concept is-derived-from-optional = requires(const T& t) {       // exposition only
      []<class U>(const optional<U>&){ }(t);
    };

  // [optional.nullopt], no-value state indicator
  struct nullopt_t{see below};
  inline constexpr nullopt_t nullopt(unspecified);

  // [optional.bad.access], class bad_optional_access
  class bad_optional_access;

  // [optional.relops], relational operators
  template<class T, class U>
    constexpr bool operator==(const optional<T>&, const optional<U>&);
  template<class T, class U>
    constexpr bool operator!=(const optional<T>&, const optional<U>&);
  template<class T, class U>
    constexpr bool operator<(const optional<T>&, const optional<U>&);
  template<class T, class U>
    constexpr bool operator>(const optional<T>&, const optional<U>&);
  template<class T, class U>
    constexpr bool operator<=(const optional<T>&, const optional<U>&);
  template<class T, class U>
    constexpr bool operator>=(const optional<T>&, const optional<U>&);
  template<class T, three_way_comparable_with<T> U>
    constexpr compare_three_way_result_t<T, U>
      operator<=>(const optional<T>&, const optional<U>&);

  // [optional.nullops], comparison with nullopt
  template<class T> constexpr bool operator==(const optional<T>&, nullopt_t) noexcept;
  template<class T>
    constexpr strong_ordering operator<=>(const optional<T>&, nullopt_t) noexcept;

  // [optional.comp.with.t], comparison with T
  template<class T, class U> constexpr bool operator==(const optional<T>&, const U&);
  template<class T, class U> constexpr bool operator==(const T&, const optional<U>&);
  template<class T, class U> constexpr bool operator!=(const optional<T>&, const U&);
  template<class T, class U> constexpr bool operator!=(const T&, const optional<U>&);
  template<class T, class U> constexpr bool operator<(const optional<T>&, const U&);
  template<class T, class U> constexpr bool operator<(const T&, const optional<U>&);
  template<class T, class U> constexpr bool operator>(const optional<T>&, const U&);
  template<class T, class U> constexpr bool operator>(const T&, const optional<U>&);
  template<class T, class U> constexpr bool operator<=(const optional<T>&, const U&);
  template<class T, class U> constexpr bool operator<=(const T&, const optional<U>&);
  template<class T, class U> constexpr bool operator>=(const optional<T>&, const U&);
  template<class T, class U> constexpr bool operator>=(const T&, const optional<U>&);
  template<class T, class U>
      requires (!is-derived-from-optional<U>) && three_way_comparable_with<T, U>
    constexpr compare_three_way_result_t<T, U>
      operator<=>(const optional<T>&, const U&);

  // [optional.specalg], specialized algorithms
  template<class T>
    constexpr void swap(optional<T>&, optional<T>&) noexcept(see below);

  template<class T>
    constexpr optional<see below> make_optional(T&&);
  template<class T, class... Args>
    constexpr optional<T> make_optional(Args&&... args);
  template<class T, class U, class... Args>
    constexpr optional<T> make_optional(initializer_list<U> il, Args&&... args);

  // [optional.hash], hash support
  template<class T> struct hash;
  template<class T> struct hash<optional<T>>;
}
*/

/*
22.5.3 Class template optional[optional.optional]
22.5.3.1 General

namespace std {
  template<class T>
  class optional {
  public:
    using value_type = T;

    // [optional.ctor], constructors
    constexpr optional() noexcept;
    constexpr optional(nullopt_t) noexcept;
    constexpr optional(const optional&);
    constexpr optional(optional&&) noexcept(see below);
    template<class... Args>
      constexpr explicit optional(in_place_t, Args&&...);
    template<class U, class... Args>
      constexpr explicit optional(in_place_t, initializer_list<U>, Args&&...);
    template<class U = T>
      constexpr explicit(see below) optional(U&&);
    template<class U>
      constexpr explicit(see below) optional(const optional<U>&);
    template<class U>
      constexpr explicit(see below) optional(optional<U>&&);

    // [optional.dtor], destructor
    constexpr ~optional();

    // [optional.assign], assignment
    constexpr optional& operator=(nullopt_t) noexcept;
    constexpr optional& operator=(const optional&);
    constexpr optional& operator=(optional&&) noexcept(see below);
    template<class U = T> constexpr optional& operator=(U&&);
    template<class U> constexpr optional& operator=(const optional<U>&);
    template<class U> constexpr optional& operator=(optional<U>&&);
    template<class... Args> constexpr T& emplace(Args&&...);
    template<class U, class... Args> constexpr T& emplace(initializer_list<U>, Args&&...);

    // [optional.swap], swap
    constexpr void swap(optional&) noexcept(see below);

    // [optional.observe], observers
    constexpr const T* operator->() const noexcept;
    constexpr T* operator->() noexcept;
    constexpr const T& operator*() const & noexcept;
    constexpr T& operator*() & noexcept;
    constexpr T&& operator*() && noexcept;
    constexpr const T&& operator*() const && noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr bool has_value() const noexcept;
    constexpr const T& value() const &;
    constexpr T& value() &;
    constexpr T&& value() &&;
    constexpr const T&& value() const &&;
    template<class U> constexpr T value_or(U&&) const &;
    template<class U> constexpr T value_or(U&&) &&;

    // [optional.monadic], monadic operations
    template<class F> constexpr auto and_then(F&& f) &;
    template<class F> constexpr auto and_then(F&& f) &&;
    template<class F> constexpr auto and_then(F&& f) const &;
    template<class F> constexpr auto and_then(F&& f) const &&;
    template<class F> constexpr auto transform(F&& f) &;
    template<class F> constexpr auto transform(F&& f) &&;
    template<class F> constexpr auto transform(F&& f) const &;
    template<class F> constexpr auto transform(F&& f) const &&;
    template<class F> constexpr optional or_else(F&& f) &&;
    template<class F> constexpr optional or_else(F&& f) const &;

    // [optional.mod], modifiers
    constexpr void reset() noexcept;

  private:
    T *val;         // exposition only
  };

  template<class T>
    optional(T) -> optional<T>;
}
*/

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

class monostate {};

struct nullopt_t {
    // Used for constructing nullopt.
    enum class Tag { tag };

    // Must be constexpr for nullopt_t to be literal.
    explicit constexpr nullopt_t(Tag) noexcept {}
};

/// Tag to disengage optional objects.
inline constexpr nullopt_t nullopt{nullopt_t::Tag::tag};

struct in_place_t {
    explicit in_place_t() = default;
};

inline constexpr in_place_t in_place{};

class bad_optional_access : public std::exception {
  public:
    bad_optional_access() = default;
    const char* what() const noexcept { return "Optional has no value"; }
};

template <class T>
class optional;

} // namespace beman::optional26

// Since P3168R2: Give std::optional Range Support.
template <typename T>
inline constexpr bool std::ranges::enable_view<beman::optional26::optional<T>> = true;

// TODO: document why this is needed.
template <typename T>
inline constexpr bool std::ranges::enable_borrowed_range<beman::optional26::optional<T&>> = true;

// Since P3168R2: Give std::optional Range Support.
#if defined(__cpp_lib_format_ranges)
template <class T>
inline constexpr auto std::format_kind<beman::optional26::optional<T>> = range_format::disabled;
#endif

namespace beman::optional26 {
template <class T>
concept is_derived_from_optional = requires(const T& t) { []<class U>(const optional<U>&) {}(t); };

namespace detail {
template <class T>
inline constexpr bool is_optional = false;
template <class T>
inline constexpr bool is_optional<optional<T>> = true;
} // namespace detail

template <typename T>
constexpr optional<std::decay_t<T>>
make_optional(T&& t) noexcept(std::is_nothrow_constructible_v<optional<std::decay_t<T>>, T>)
    requires std::is_constructible_v<std::decay_t<T>, T>
{
    return optional<std::decay_t<T>>{std::forward<T>(t)};
}

template <typename T, typename... Args>
constexpr optional<T> make_optional(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    requires std::is_constructible_v<T, Args...>
{
    return optional<T>{in_place, std::forward<Args>(args)...};
}

template <typename T, typename _Up, typename... Args>
constexpr optional<T>
make_optional(std::initializer_list<_Up> init_list,
              Args&&... args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<_Up>&, Args...>)
    requires std::is_constructible_v<T, std::initializer_list<_Up>&, Args...>
{
    return optional<T>{in_place, init_list, std::forward<Args>(args)...};
}

template <class T, class U>
concept enable_forward_value = std::is_constructible_v<T, U&&> && !std::is_same_v<std::decay_t<U>, in_place_t> &&
                               !std::is_same_v<optional<T>, std::decay_t<U>>;

template <class T, class U, class Other>
concept enable_from_other =
    std::is_constructible_v<T, Other> && !std::is_constructible_v<T, optional<U>&> &&
    !std::is_constructible_v<T, optional<U>&&> && !std::is_constructible_v<T, const optional<U>&> &&
    !std::is_constructible_v<T, const optional<U>&&> && !std::is_convertible_v<optional<U>&, T> &&
    !std::is_convertible_v<optional<U>&&, T> && !std::is_convertible_v<const optional<U>&, T> &&
    !std::is_convertible_v<const optional<U>&&, T>;

template <class T, class U>
concept enable_assign_forward = !std::is_same_v<optional<T>, std::decay_t<U>> &&
                                !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
                                std::is_constructible_v<T, U> && std::is_assignable_v<T&, U>;

template <class T, class U, class Other>
concept enable_assign_from_other =
    std::is_constructible_v<T, Other> && std::is_assignable_v<T&, Other> &&
    !std::is_constructible_v<T, optional<U>&> && !std::is_constructible_v<T, optional<U>&&> &&
    !std::is_constructible_v<T, const optional<U>&> && !std::is_constructible_v<T, const optional<U>&&> &&
    !std::is_convertible_v<optional<U>&, T> && !std::is_convertible_v<optional<U>&&, T> &&
    !std::is_convertible_v<const optional<U>&, T> && !std::is_convertible_v<const optional<U>&&, T> &&
    !std::is_assignable_v<T&, optional<U>&> && !std::is_assignable_v<T&, optional<U>&&> &&
    !std::is_assignable_v<T&, const optional<U>&> && !std::is_assignable_v<T&, const optional<U>&&>;

template <class T>
class optional {
    static_assert((!std::is_same_v<T, std::remove_cv_t<in_place_t>>) &&
                  (!std::is_same_v<std::remove_cv_t<T>, nullopt_t>));

    struct empty {};
    union {
        empty _{};
        T     value_;
    };
    bool engaged_ = false;

    template <class... Args>
    constexpr void construct(Args&&... args) {
        std::construct_at(std::addressof(value_), std::forward<Args>(args)...);
        engaged_ = true;
    }

    constexpr void hard_reset() noexcept {
        value_.~T();
        engaged_ = false;
    }

  public:
    using value_type = T;
    // Since P3168R2: Give std::optional Range Support.
    using iterator       = detail::contiguous_iterator<T, optional>;       // see [optional.iterators]
    using const_iterator = detail::contiguous_iterator<const T, optional>; // see [optional.iterators]

    constexpr optional() noexcept : _(), engaged_(false) {}

    constexpr ~optional()
        requires(!std::is_trivially_destructible_v<T>)
    {
        if (has_value())
            value_.~T();
    }

    constexpr ~optional()
        requires std::is_trivially_destructible_v<T>
    = default;

    constexpr optional(nullopt_t) noexcept {}

    constexpr optional(const optional& rhs)
        requires std::is_copy_constructible_v<T> && (!std::is_trivially_copy_constructible_v<T>)
    {
        if (rhs.has_value()) {

            std::construct_at(std::addressof(value_), rhs.value_);
            engaged_ = true;
        }
    }

    optional(const optional&)
        requires std::is_copy_constructible_v<T> && std::is_trivially_copy_constructible_v<T>
    = default;

    constexpr optional(optional&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T> && (!std::is_trivially_move_constructible_v<T>)
    {
        if (rhs.has_value()) {
            std::construct_at(std::addressof(value_), std::move(rhs.value()));
            engaged_ = true;
        }
    }

    optional(optional&&)
        requires std::is_move_constructible_v<T> && std::is_trivially_move_constructible_v<T>
    = default;

    /// Constructs the stored value in-place using the given arguments.
    template <class... Args>
    constexpr explicit optional(in_place_t, Args&&... args)
        requires std::is_constructible_v<T, Args...>
        : value_(std::forward<Args>(args)...), engaged_(true) {}

    template <class U, class... Args>
    constexpr explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args)
        requires std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>
        : value_(il, std::forward<Args>(args)...), engaged_(true) {}

    /// Constructs the stored value with `u`.
    template <class U = T>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(U&& u)
        requires enable_forward_value<T, U> && std::is_convertible_v<U&&, T>
        : optional(in_place, std::forward<U>(u)) {}

    template <class U = T>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(U&& u)
        requires enable_forward_value<T, U> && (!std::is_convertible_v<U &&, T>)
        : optional(in_place, std::forward<U>(u)) {}

    /// Converting copy constructor.
    template <class U>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(const optional<U>& rhs)
        requires enable_from_other<T, U, const U&> && std::is_convertible_v<const U&, T>
    {
        if (rhs.has_value()) {
            std::construct_at(std::addressof(value_), rhs.value());
            engaged_ = true;
        }
    }

    template <class U>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(const optional<U>& rhs)
        requires enable_from_other<T, U, const U&> && (!std::is_convertible_v<const U&, T>)
    {
        if (rhs.has_value()) {
            construct(*rhs);
        }
    }

    /// Converting move constructor.
    template <class U>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(optional<U>&& rhs)
        requires enable_from_other<T, U, U&&> && std::is_convertible_v<U&&, T>
    {
        if (rhs.has_value()) {
            construct(std::move(*rhs));
        }
    }

    template <class U>
    constexpr explicit(!std::is_convertible_v<U, T>) optional(optional<U>&& rhs)
        requires enable_from_other<T, U, U&&> && (!std::is_convertible_v<U &&, T>)
    {
        if (rhs.has_value()) {
            construct(std::move(*rhs));
        }
    }

    constexpr optional& operator=(const optional& rhs)
        requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
                 (!std::is_trivially_copy_assignable_v<T>)
    {
        if (!rhs.has_value())
            reset();
        else if (has_value())
            value_ = rhs.value_;
        else
            std::construct_at(std::addressof(value_), rhs.value_);
        return *this;
    }

    optional& operator=(const optional&)
        requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
                     std::is_trivially_copy_constructible_v<T> && std::is_trivially_copy_assignable_v<T>
    = default;

    constexpr optional& operator=(optional&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T> &&
                 (!std::is_trivially_move_assignable_v<T>)
    {
        if (!rhs.has_value())
            reset();
        else if (has_value())
            value_ = std::move(rhs.value_);
        else
            std::construct_at(std::addressof(value_), std::move(rhs.value_));
        return *this;
    }

    optional& operator=(optional&&)
        requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T> &&
                     std::is_trivially_move_constructible_v<T> && std::is_trivially_move_assignable_v<T>
    = default;

    /// Returns the contained value if there is one, otherwise throws
    /// bad_optional_access
    constexpr T& value() & {
        if (has_value())
            return value_;
        throw bad_optional_access();
    }
    constexpr const T& value() const& {
        if (has_value())
            return value_;
        throw bad_optional_access();
    }
    constexpr T&& value() && {
        if (has_value())
            return std::move(value_);
        throw bad_optional_access();
    }

    /// Returns the stored value if there is one, otherwise returns `u`
    template <class U>
    constexpr T value_or(U&& u) const& {
        static_assert(std::is_copy_constructible_v<T> && std::is_convertible_v<U&&, T>);
        return has_value() ? value() : static_cast<T>(std::forward<U>(u));
    }

    template <class U>
    constexpr T value_or(U&& u) && {
        static_assert(std::is_move_constructible_v<T> && std::is_convertible_v<U&&, T>);
        return has_value() ? std::move(value()) : static_cast<T>(std::forward<U>(u));
    }

    template <class F>
    constexpr auto and_then(F&& f) & {
        using U = std::invoke_result_t<F, T&>;
        static_assert(detail::is_optional<std::remove_cvref_t<U>>);
        if (has_value()) {
            return std::invoke(std::forward<F>(f), value_);
        } else {
            return std::remove_cvref_t<U>();
        }
    }

    template <class F>
    constexpr auto and_then(F&& f) && {
        using U = std::invoke_result_t<F, T&&>;
        static_assert(detail::is_optional<std::remove_cvref_t<U>>);
        if (has_value()) {
            return std::invoke(std::forward<F>(f), std::move(value_));
        } else {
            return std::remove_cvref_t<U>();
        }
    }

    template <class F>
    constexpr auto and_then(F&& f) const& {
        using U = std::invoke_result_t<F, const T&>;
        static_assert(detail::is_optional<std::remove_cvref_t<U>>);
        if (has_value()) {
            return std::invoke(std::forward<F>(f), value_);
        } else {
            return std::remove_cvref_t<U>();
        }
    }

    template <class F>
    constexpr auto and_then(F&& f) const&& {
        using U = std::invoke_result_t<F, const T&&>;
        static_assert(detail::is_optional<std::remove_cvref_t<U>>);
        if (has_value()) {
            return std::invoke(std::forward<F>(f), std::move(value_));
        } else {
            return std::remove_cvref_t<U>();
        }
    }

    /// Carries out some operation on the stored object if there is one.
    template <class F>
    constexpr auto transform(F&& f) & {
        using U = std::invoke_result_t<F, T&>;
        static_assert(!std::is_array_v<U>);
        static_assert(!std::is_same_v<U, in_place_t>);
        static_assert(!std::is_same_v<U, nullopt_t>);
        static_assert(std::is_object_v<U> || std::is_reference_v<U>); /// References now allowed
        return (has_value()) ? optional<U>{std::invoke(std::forward<F>(f), value_)} : optional<U>{};
    }

    template <class F>
    constexpr auto transform(F&& f) && {
        using U = std::invoke_result_t<F, T&&>;
        static_assert(!std::is_array_v<U>);
        static_assert(!std::is_same_v<U, in_place_t>);
        static_assert(!std::is_same_v<U, nullopt_t>);
        static_assert(std::is_object_v<U> || std::is_reference_v<U>); /// References now allowed
        return (has_value()) ? optional<U>{std::invoke(std::forward<F>(f), std::move(value_))} : optional<U>{};
    }

    template <class F>
    constexpr auto transform(F&& f) const& {
        using U = std::invoke_result_t<F, const T&>;
        static_assert(!std::is_array_v<U>);
        static_assert(!std::is_same_v<U, in_place_t>);
        static_assert(!std::is_same_v<U, nullopt_t>);
        static_assert(std::is_object_v<U> || std::is_reference_v<U>); /// References now allowed
        return (has_value()) ? optional<U>{std::invoke(std::forward<F>(f), value_)} : optional<U>{};
    }

    template <class F>
    constexpr auto transform(F&& f) const&& {
        using U = std::invoke_result_t<F, const T&>;
        static_assert(!std::is_array_v<U>);
        static_assert(!std::is_same_v<U, in_place_t>);
        static_assert(!std::is_same_v<U, nullopt_t>);
        static_assert(std::is_object_v<U> || std::is_reference_v<U>); /// References now allowed
        return (has_value()) ? optional<U>{std::invoke(std::forward<F>(f), value_)} : optional<U>{};
    }

    /// Calls `f` if the optional is empty
    template <class F>
    constexpr optional<T> or_else(F&& f) const& {
        static_assert(std::is_same_v<std::remove_cvref_t<std::invoke_result_t<F>>, optional>);
        if (has_value())
            return value_;

        return std::forward<F>(f)();
    }

    template <class F>
    constexpr optional<T> or_else(F&& f) && {
        static_assert(std::is_same_v<std::remove_cvref_t<std::invoke_result_t<F>>, optional>);
        if (has_value())
            return std::move(value_);

        return std::forward<F>(f)();
    }

    /// Assigns the stored value from `u`, destroying the old value if there
    /// was one.
    template <class U = T>
    constexpr optional& operator=(U&& u)
        requires enable_assign_forward<T, U>
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
    template <class U>
    constexpr optional& operator=(const optional<U>& rhs)
        requires enable_assign_from_other<T, U, const U&>
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
    template <class U>
    constexpr optional& operator=(optional<U>&& rhs)
        requires enable_assign_from_other<T, U, U>
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
    template <class... Args>
    constexpr T& emplace(Args&&... args) {
        static_assert(std::is_constructible_v<T, Args&&...>);
        *this = nullopt;
        construct(std::forward<Args>(args)...);
        return value();
    }

    template <class U, class... Args>
    constexpr T& emplace(std::initializer_list<U> il, Args&&... args) {
        static_assert(std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>);
        *this = nullopt;
        construct(il, std::forward<Args>(args)...);
        return value();
    }

    /// Swaps this optional with the other.
    ///
    /// If neither optionals have a value, nothing happens.
    /// If both have a value, the values are swapped.
    /// If one has a value, it is moved to the other and the movee is left
    /// valueless.
    constexpr void swap(optional& rhs) noexcept(std::is_nothrow_move_constructible<T>::value &&
                                                std::is_nothrow_swappable<T>::value) {
        static_assert(std::is_move_constructible_v<T>);
        using std::swap;
        if (has_value()) {
            if (rhs.has_value()) {
                swap(value(), *rhs);
            } else {
                std::construct_at(std::addressof(rhs.value_), std::move(value_));
                value_.T::~T();
            }
        } else if (rhs.has_value()) {

            std::construct_at(std::addressof(value_), std::move(rhs.value_));
            rhs.value_.T::~T();
        }
        swap(engaged_, rhs.engaged_);
    }

    // Since P3168R2: Give std::optional Range Support.
    // [optional.iterators], iterator support
    constexpr iterator       begin() noexcept { return iterator(has_value() ? std::addressof(value_) : nullptr); }
    constexpr const_iterator begin() const noexcept {
        return const_iterator(has_value() ? std::addressof(value_) : nullptr);
    }
    constexpr iterator       end() noexcept { return begin() + has_value(); }
    constexpr const_iterator end() const noexcept { return begin() + has_value(); }

    /// Returns a pointer to the stored value
    constexpr const T* operator->() const { return std::addressof(value_); }

    constexpr T* operator->() { return std::addressof(value_); }

    /// Returns the stored value
    constexpr T& operator*() & { return value_; }

    constexpr const T& operator*() const& { return value_; }

    constexpr T&& operator*() && { return std::move(value_); }

    /// Returns whether or not the optional has a value
    constexpr bool has_value() const noexcept { return engaged_; }

    constexpr explicit operator bool() const noexcept { return engaged_; }

    constexpr void reset() noexcept {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            if (has_value())
                value_.~T();
        }
        engaged_ = false;
    }
};

template <typename T, typename U>
concept optional_eq_rel = requires(const T& t, const U& u) {
    { t == u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_ne_rel = requires(const T& t, const U& u) {
    { t != u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_lt_rel = requires(const T& t, const U& u) {
    { t < u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_gt_rel = requires(const T& t, const U& u) {
    { t > u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_le_rel = requires(const T& t, const U& u) {
    { t <= u } -> std::convertible_to<bool>;
};

template <typename T, typename U>
concept optional_ge_rel = requires(const T& t, const U& u) {
    { t >= u } -> std::convertible_to<bool>;
};

// Comparisons between optional values.
template <typename T, typename U>
constexpr bool operator==(const optional<T>& lhs, const optional<U>& rhs)
    requires optional_eq_rel<T, U>
{
    return static_cast<bool>(lhs) == static_cast<bool>(rhs) && (!lhs || *lhs == *rhs);
}

template <typename T, typename U>
constexpr bool operator!=(const optional<T>& lhs, const optional<U>& rhs)
    requires optional_ne_rel<T, U>
{
    return static_cast<bool>(lhs) != static_cast<bool>(rhs) || (static_cast<bool>(lhs) && *lhs != *rhs);
}

template <typename T, typename U>
constexpr bool operator<(const optional<T>& lhs, const optional<U>& rhs)
    requires optional_lt_rel<T, U>
{
    return static_cast<bool>(rhs) && (!lhs || *lhs < *rhs);
}

template <typename T, typename U>
constexpr bool operator>(const optional<T>& lhs, const optional<U>& rhs)
    requires optional_gt_rel<T, U>
{
    return static_cast<bool>(lhs) && (!rhs || *lhs > *rhs);
}

template <typename T, typename U>
constexpr bool operator<=(const optional<T>& lhs, const optional<U>& rhs)
    requires optional_le_rel<T, U>
{
    return !lhs || (static_cast<bool>(rhs) && *lhs <= *rhs);
}

template <typename T, typename U>
constexpr bool operator>=(const optional<T>& lhs, const optional<U>& rhs)
    requires optional_ge_rel<T, U>
{
    return !rhs || (static_cast<bool>(lhs) && *lhs >= *rhs);
}

template <typename T, std::three_way_comparable_with<T> U>
constexpr std::compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const optional<U>& y) {
    return x && y ? *x <=> *y : bool(x) <=> bool(y);
}

// Comparisons with nullopt.
template <typename T>
constexpr bool operator==(const optional<T>& lhs, nullopt_t) noexcept {
    return !lhs;
}

template <typename T>
constexpr std::strong_ordering operator<=>(const optional<T>& x, nullopt_t) noexcept {
    return bool(x) <=> false;
}

// Comparisons with value type.
template <typename T, typename U>
constexpr bool operator==(const optional<T>& lhs, const U& rhs)
    requires optional_eq_rel<T, U>
{
    return lhs && *lhs == rhs;
}

template <typename T, typename U>
constexpr bool operator==(const U& lhs, const optional<T>& rhs)
    requires optional_eq_rel<U, T>
{
    return rhs && lhs == *rhs;
}

template <typename T, typename U>
constexpr bool operator!=(const optional<T>& lhs, const U& rhs)
    requires optional_ne_rel<T, U>
{
    return !lhs || *lhs != rhs;
}

template <typename T, typename U>
constexpr bool operator!=(const U& lhs, const optional<T>& rhs)
    requires optional_ne_rel<U, T>
{
    return !rhs || lhs != *rhs;
}

template <typename T, typename U>
constexpr bool operator<(const optional<T>& lhs, const U& rhs)
    requires optional_lt_rel<T, U>
{
    return !lhs || *lhs < rhs;
}

template <typename T, typename U>
constexpr bool operator<(const U& lhs, const optional<T>& rhs)
    requires optional_lt_rel<U, T>
{
    return rhs && lhs < *rhs;
}

template <typename T, typename U>
constexpr bool operator>(const optional<T>& lhs, const U& rhs)
    requires optional_gt_rel<T, U>
{
    return lhs && *lhs > rhs;
}

template <typename T, typename U>
constexpr bool operator>(const U& lhs, const optional<T>& rhs)
    requires optional_gt_rel<U, T>
{
    return !rhs || lhs > *rhs;
}

template <typename T, typename U>
constexpr bool operator<=(const optional<T>& lhs, const U& rhs)
    requires optional_le_rel<T, U>
{
    return !lhs || *lhs <= rhs;
}

template <typename T, typename U>
constexpr bool operator<=(const U& lhs, const optional<T>& rhs)
    requires optional_le_rel<U, T>
{
    return rhs && lhs <= *rhs;
}

template <typename T, typename U>
constexpr bool operator>=(const optional<T>& lhs, const U& rhs)
    requires optional_ge_rel<T, U>
{
    return lhs && *lhs >= rhs;
}

template <typename T, typename U>
constexpr bool operator>=(const U& lhs, const optional<T>& rhs)
    requires optional_ge_rel<U, T>
{
    return !rhs || lhs >= *rhs;
}

template <typename T, typename U>
    requires(!is_derived_from_optional<U>) && std::three_way_comparable_with<T, U>
constexpr std::compare_three_way_result_t<T, U> operator<=>(const optional<T>& x, const U& v) {
    return bool(x) ? *x <=> v : std::strong_ordering::less;
}

template <class T>
void swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    requires std::is_move_constructible_v<T> && std::is_swappable_v<T>
{
    return lhs.swap(rhs);
}

namespace detail {
template <class Opt, class F, class Ret = decltype(std::invoke(std::declval<F>(), *std::declval<Opt>()))>
constexpr auto optional_map_impl(Opt&& opt, F&& f)
    requires(!std::is_void_v<Ret>)
{
    return opt.has_value() ? std::invoke(std::forward<F>(f), *std::forward<Opt>(opt)) : optional<Ret>(nullopt);
}

template <class Opt, class F, class Ret = decltype(std::invoke(std::declval<F>(), *std::declval<Opt>()))>
auto optional_map_impl(Opt&& opt, F&& f)
    requires std::is_void_v<Ret>
{
    if (opt.has_value()) {
        std::invoke(std::forward<F>(f), *std::forward<Opt>(opt));
        return make_optional(monostate{});
    }

    return optional<monostate>{nullopt};
}
} // namespace detail

/****************/
/* optional<T&> */
/****************/

namespace detail {

#ifdef __cpp_lib_reference_from_temporary
using std::reference_constructs_from_temporary_v;
using std::reference_converts_from_temporary_v;
#else
template <class To, class From>
concept reference_converts_from_temporary_v =
    std::is_reference_v<To> &&
    (
        // A prvalue of a type similar to To, so that we're binding directly to the materialized prvalue of type From
        (!std::is_reference_v<From> && std::is_convertible_v<std::remove_cvref_t<From>*, std::remove_cvref_t<To>*>) ||
        // A value of an unrelated type, convertible to To, but only by materializing a To and binding a const
        // reference; if we were trying to bind a non-const reference, we'd be unable to. (This is not quite exhaustive
        // of the problem cases, but I think it's fairly close in practice.)
        (std::is_lvalue_reference_v<To> && std::is_const_v<std::remove_reference_t<To>> &&
         std::is_convertible_v<From, const std::remove_cvref_t<To>&&> &&
         !std::is_convertible_v<From, std::remove_cvref_t<To>&>));

template <class To, class From>
concept reference_constructs_from_temporary_v =
    // This is close in practice, because cases where conversion and construction differ in semantics are rare.
    reference_converts_from_temporary_v<To, From>;
#endif

template <class To, class From>
concept safely_convertible = std::is_convertible_v<From, To> && !reference_converts_from_temporary_v<To, From>;

template <class To, class From>
concept safely_constructible_not_convertible = std::is_constructible_v<To, From> && !std::is_convertible_v<From, To> &&
                                               !reference_constructs_from_temporary_v<To, From>;

template <class To, class From>
concept safely_constructible = std::is_constructible_v<To, From> && !reference_constructs_from_temporary_v<To, From>;

} // namespace detail

template <class T>
class optional<T&> {
  public:
    using value_type = T&;
    // Since ${PAPER_NUMBER}: ${PAPER_TITLE}.
    // Note: P3168 and P2988 may have different flows inside LEWG/LWG.
    // Implementation of the range support for optional<T&> reflects P3168R2 for now.
    // [optional.iterators], iterator support
    using iterator       = detail::contiguous_iterator<T, optional>;       // see [optional.iterators]
    using const_iterator = detail::contiguous_iterator<const T, optional>; // see [optional.iterators]

    // [optional.ctor], constructors
    //    constexpr optional() noexcept;
    //    constexpr optional(nullopt_t) noexcept;
    //    constexpr optional(const optional&);
    //    constexpr optional(optional&&) noexcept(/* see below */);
    //    template<class U = T>
    //      constexpr optional(U&&);
    //    template <class U>
    //       constexpr explicit optional(const optional<U>& rhs) noexcept;

    // [optional.dtor], destructor
    //    constexpr ~optional();

    // [optional.assign], assignment
    //    constexpr optional& operator=(nullopt_t) noexcept;
    //    constexpr optional& operator=(const optional&);
    //    constexpr optional& operator=(optional&&) noexcept(/* see below */);
    //    template <class U = T>
    //    constexpr optional& operator=(U&&);
    //    template <class U>
    //    constexpr optional& operator=(const optional<U>&);
    //    template <class U>
    //    constexpr optional& operator=(optional<U>&&);

    // [optional.swap], swap
    //    constexpr void swap(optional&) noexcept(/* see below */);

    // [optional.observe], observers
    //    constexpr T*  operator->() const noexcept;
    //    constexpr T&  operator*() const& noexcept;
    //    constexpr T&& operator*() const&& noexcept;
    //    constexpr explicit  operator bool() const noexcept;
    //    constexpr bool      has_value() const noexcept;
    //    constexpr T&  value() const&;
    //    constexpr T&& value() const&&;
    //    template <class U>
    //      constexpr T value_or(U&&) const&;
    //    template <class U>
    //      constexpr T value_or(U&&) &&;

    // [optional.monadic], monadic operations
    // template <class F>
    // constexpr auto and_then(F&& f) &;
    // template <class F>
    // constexpr auto and_then(F&& f) &&;
    // template <class F>
    // constexpr auto and_then(F&& f) const&;
    // template <class F>
    // constexpr auto and_then(F&& f) const&&;
    // template <class F>
    // constexpr auto transform(F&& f) &;
    // template <class F>
    // constexpr auto transform(F&& f) &&;
    // template <class F>
    // constexpr auto transform(F&& f) const&;
    // template <class F>
    // constexpr auto transform(F&& f) const&&;
    // template <class F>
    // constexpr optional or_else(F&& f) &&;
    // template <class F>
    // constexpr optional or_else(F&& f) const&;

    // [optional.mod], modifiers
    // constexpr void reset() noexcept;

  private:
    T* value_ = nullptr; // exposition only

  public:
    //  \rSec3[optional.ctor]{Constructors}

    optional() = default;

    constexpr optional(nullopt_t) noexcept : value_(nullptr) {}

    optional(const optional& rhs) = default;
    optional(optional&& rhs)      = default;

    template <class U>
    constexpr explicit optional(U&& u) noexcept(std::is_nothrow_constructible_v<U&&, T&>)
        requires detail::safely_constructible_not_convertible<T&, U&&>
        : value_(std::addressof(static_cast<T&>(std::forward<U>(u)))) {}

    template <class U>
    constexpr optional(U&& u) noexcept(std::is_nothrow_convertible_v<U&&, T&>)
        requires detail::safely_convertible<T&, U&&>
        : value_(std::addressof(static_cast<T&>(std::forward<U>(u)))) {}

    template <class U>
    constexpr explicit optional(const optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<T&, const U&>)
        requires detail::safely_constructible_not_convertible<T&, const U&>
        : value_(rhs ? std::addressof(static_cast<T&>(*rhs)) : nullptr) {}

    template <class U>
    constexpr optional(const optional<U>& rhs) noexcept(std::is_nothrow_convertible_v<const U&, T&>)
        requires detail::safely_convertible<T&, const U&>
        : value_(rhs ? std::addressof(static_cast<T&>(*rhs)) : nullptr) {}

    template <class U>
    constexpr explicit optional(optional<U>& rhs) noexcept(std::is_nothrow_constructible_v<T&, U&>)
        requires detail::safely_constructible_not_convertible<T&, U&>
        : value_(rhs ? std::addressof(static_cast<T&>(*rhs)) : nullptr) {}

    template <class U>
    constexpr optional(optional<U>& rhs) noexcept(std::is_nothrow_convertible_v<U&, T&>)
        requires detail::safely_convertible<T&, U&>
        : value_(rhs ? std::addressof(static_cast<T&>(*rhs)) : nullptr) {}

    //  \rSec3[optional.dtor]{Destructor}

    ~optional() = default;

    // \rSec3[optional.assign]{Assignment}

    constexpr optional& operator=(nullopt_t) noexcept {
        value_ = nullptr;
        return *this;
    }

    optional& operator=(const optional&) = default;
    optional& operator=(optional&&)      = default;

    template <class U>
        requires detail::safely_constructible<T&, U&&>
    constexpr T& emplace(U&& u) {
        value_ = std::addressof(static_cast<T&>(std::forward<U>(u)));
        return *value_;
    }

    //   \rSec3[optional.swap]{Swap}

    constexpr void swap(optional& rhs) noexcept { std::swap(value_, rhs.value_); }

    // Since ${PAPER_NUMBER}: ${PAPER_TITLE}.
    // Note: P3168 and P2988 may have different flows inside LEWG/LWG.
    // Implementation of the range support for optional<T&> reflects P3168R2 for now.
    // [optional.iterators], iterator support
    constexpr iterator       begin() noexcept { return iterator(has_value() ? value_ : nullptr); };
    constexpr const_iterator begin() const noexcept { return const_iterator(has_value() ? value_ : nullptr); };
    constexpr iterator       end() noexcept { return begin() + has_value(); }
    constexpr const_iterator end() const noexcept { return begin() + has_value(); }

    // \rSec3[optional.observe]{Observers}
    constexpr T* operator->() const noexcept { return value_; }

    constexpr T& operator*() const noexcept { return *value_; }

    constexpr explicit operator bool() const noexcept { return value_ != nullptr; }
    constexpr bool     has_value() const noexcept { return value_ != nullptr; }

    constexpr T& value() const {
        if (has_value())
            return *value_;
        throw bad_optional_access();
    }

    template <class U>
    constexpr T value_or(U&& u) const {
        static_assert(std::is_constructible_v<std::add_lvalue_reference_t<T>, decltype(u)>,
                      "Must be able to bind u to T&");
        return has_value() ? *value_ : std::forward<U>(u);
    }

    //   \rSec3[optional.monadic]{Monadic operations}

    template <class F>
    constexpr auto and_then(F&& f) const {
        using U = std::invoke_result_t<F, T&>;
        static_assert(detail::is_optional<U>, "F must return an optional");
        return (has_value()) ? std::invoke(std::forward<F>(f), *value_) : std::remove_cvref_t<U>();
    }

    template <class F>
    constexpr auto transform(F&& f) const -> optional<std::invoke_result_t<F, T&>> {
        using U = std::invoke_result_t<F, T&>;
        return (has_value()) ? optional<U>{std::invoke(std::forward<F>(f), *value_)} : optional<U>{};
    }

    template <class F>
    constexpr optional or_else(F&& f) const {
        using U = std::invoke_result_t<F>;
        static_assert(std::is_same_v<std::remove_cvref_t<U>, optional>);
        return has_value() ? *value_ : std::forward<F>(f)();
    }

    constexpr void reset() noexcept { value_ = nullptr; }
};

} // namespace beman::optional26

#endif // BEMAN_OPTIONAL26_OPTIONAL_HPP
