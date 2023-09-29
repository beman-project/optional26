// smd/optional/optional.h                                            -*-C++-*-
#ifndef INCLUDED_SMD_OPTIONAL_OPTIONAL
#define INCLUDED_SMD_OPTIONAL_OPTIONAL

/*
22.5.2 Header <optional> synopsis[optional.syn]
🔗

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
#include <utility>
#include <functional>

namespace smd::optional {

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

static constexpr in_place_t in_place{};

class bad_optional_access : public std::exception {
  public:
    bad_optional_access() = default;
    const char* what() const noexcept { return "Optional has no value"; }
};

template <class T>
    requires(!std::is_same_v<T, in_place_t>) && (!std::is_same_v<std::decay_t<T>, nullopt_t>)
class optional;

template <class T>
concept is_derived_from_optional = requires(const T& t) { []<class U>(const optional<U>&) {}(t); };

namespace detail {

template <class T>
struct is_optional_impl : std::false_type {};
template <class T>
struct is_optional_impl<optional<T>> : std::true_type {};
template <class T>
using is_optional = is_optional_impl<std::decay_t<T>>;

} // namespace detail

template <typename T>
constexpr optional<std::decay_t<T>>
make_optional(T&& __t) noexcept(std::is_nothrow_constructible_v<optional<std::decay_t<T>>, T>)
    requires std::is_constructible_v<std::decay_t<T>, T> && (!std::is_reference_v<T>)
{
    return optional<std::decay_t<T>>{std::forward<T>(__t)};
}

template <typename T>
constexpr optional<std::decay_t<T>&>
make_optional(T&& __t) noexcept(std::is_nothrow_constructible_v<optional<std::decay_t<T>>, T>)
    requires std::is_constructible_v<std::decay_t<T>, T> && (std::is_reference_v<T>)
{
    return optional<std::decay_t<T>&>{std::forward<T>(__t)};
}

template <typename T, typename... _Args>
constexpr optional<T> make_optional(_Args&&... __args) noexcept(std::is_nothrow_constructible_v<T, _Args...>)
    requires std::is_constructible_v<T, _Args...>
{
    return optional<T>{in_place, std::forward<_Args>(__args)...};
}

template <typename T, typename _Up, typename... _Args>
constexpr optional<T>
make_optional(std::initializer_list<_Up> __il,
              _Args&&... __args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<_Up>&, _Args...>)
    requires std::is_constructible_v<T, std::initializer_list<_Up>&, _Args...>
{
    return optional<T>{in_place, __il, std::forward<_Args>(__args)...};
}

template <class T_, class U_>
concept enable_forward_value = std::is_constructible_v<T_, U_&&> && !std::is_same_v<std::decay_t<U_>, in_place_t> &&
                               !std::is_same_v<optional<T_>, std::decay_t<U_>>;

template <class T_, class U_, class Other>
concept enable_from_other =
    std::is_constructible_v<T_, Other> && !std::is_constructible_v<T_, optional<U_>&> &&
    !std::is_constructible_v<T_, optional<U_>&&> && !std::is_constructible_v<T_, const optional<U_>&> &&
    !std::is_constructible_v<T_, const optional<U_>&&> && !std::is_convertible_v<optional<U_>&, T_> &&
    !std::is_convertible_v<optional<U_>&&, T_> && !std::is_convertible_v<const optional<U_>&, T_> &&
    !std::is_convertible_v<const optional<U_>&&, T_>;

template <class T, class U>
concept enable_assign_forward = !std::is_same_v<optional<T>, std::decay_t<U>> &&
                                !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
                                std::is_constructible_v<T, U> && std::is_assignable_v<T&, U>;

template <class T_, class U_, class Other>
concept enable_assign_from_other =
    std::is_constructible_v<T_, Other> && std::is_assignable_v<T_&, Other> &&
    !std::is_constructible_v<T_, optional<U_>&> && !std::is_constructible_v<T_, optional<U_>&&> &&
    !std::is_constructible_v<T_, const optional<U_>&> && !std::is_constructible_v<T_, const optional<U_>&&> &&
    !std::is_convertible_v<optional<U_>&, T_> && !std::is_convertible_v<optional<U_>&&, T_> &&
    !std::is_convertible_v<const optional<U_>&, T_> && !std::is_convertible_v<const optional<U_>&&, T_> &&
    !std::is_assignable_v<T_&, optional<U_>&> && !std::is_assignable_v<T_&, optional<U_>&&> &&
    !std::is_assignable_v<T_&, const optional<U_>&> && !std::is_assignable_v<T_&, const optional<U_>&&>;

template <class T>
    requires(!std::is_same_v<T, in_place_t>) && (!std::is_same_v<std::decay_t<T>, nullopt_t>)
class optional {

    struct empty {};
    union {
        empty _{};
        T     value_;
    };
    bool engaged = false;

    template <class... Args>
    void construct(Args&&... args) {
        new (std::addressof(value_)) T(std::forward<Args>(args)...);
        engaged = true;
    }

    void hard_reset() noexcept {
        get().~T();
        engaged = false;
    }
    constexpr T&        get() & { return value_; }
    constexpr const T&  get() const& { return value_; }
    constexpr T&&       get() && { return std::move(value_); }
    constexpr const T&& get() const&& { return std::move(value_); }

  public:
    constexpr optional() noexcept
        requires std::is_default_constructible_v<T>
    = default;

    constexpr optional() noexcept
        requires(!std::is_default_constructible_v<T>)
        : _(), engaged(false) {}

    ~optional()
        requires(!std::is_trivially_destructible_v<T>)
    {
        if (has_value())
            value_.~T();
    }

    ~optional()
        requires std::is_trivially_destructible_v<T>
    = default;

    constexpr optional(nullopt_t) noexcept {}

    constexpr optional(const optional& rhs)
        requires std::is_copy_constructible_v<T> && (!std::is_trivially_copy_constructible_v<T>)
    {
        if (rhs.has_value()) {
            ::new (&value_) T(rhs.value_);
            engaged = true;
        }
    }

    constexpr optional(const optional&)
        requires std::is_copy_constructible_v<T> && std::is_trivially_copy_constructible_v<T>
    = default;

    constexpr optional(optional&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T> && (!std::is_trivially_move_constructible_v<T>)
    {
        if (rhs.has_value()) {
            ::new (&value_) T(std::move(rhs.value_));
            engaged = true;
        }
    }

    constexpr optional(optional&&)
        requires std::is_move_constructible_v<T> && std::is_trivially_move_constructible_v<T>
    = default;

    /// Constructs the stored value in-place using the given arguments.
    template <class... Args>
    constexpr explicit optional(in_place_t, Args&&... args)
        requires std::is_constructible_v<T, Args...>
        : value_(std::forward<Args>(args)...), engaged(true) {}

    template <class U, class... Args>
    constexpr explicit optional(in_place_t, std::initializer_list<U> il, Args&&... args)
        requires std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>
        : value_(il, std::forward<Args>(args)...), engaged(true) {}

    /// Constructs the stored value with `u`.
    template <class U = T>
    constexpr optional(U&& u)
        requires enable_forward_value<T, U> && std::is_convertible_v<U&&, T>
        : optional(in_place, std::forward<U>(u)) {}

    template <class U = T>
    constexpr explicit optional(U&& u)
        requires enable_forward_value<T, U> && (!std::is_convertible_v<U &&, T>)
        : optional(in_place, std::forward<U>(u)) {}

    /// Converting copy constructor.
    template <class U>
    optional(const optional<U>& rhs)
        requires enable_from_other<T, U, const U&> && std::is_convertible_v<const U&, T>
    {
        if (rhs.has_value()) {
            construct(*rhs);
        }
    }

    template <class U>
    explicit optional(const optional<U>& rhs)
        requires enable_from_other<T, U, const U&> && (!std::is_convertible_v<const U&, T>)
    {
        if (rhs.has_value()) {
            construct(*rhs);
        }
    }

    /// Converting move constructor.
    template <class U>
    optional(optional<U>&& rhs)
        requires enable_from_other<T, U, U&&> && std::is_convertible_v<U&&, T>
    {
        if (rhs.has_value()) {
            construct(std::move(*rhs));
        }
    }

    template <class U>
    explicit optional(optional<U>&& rhs)
        requires enable_from_other<T, U, U&&> && (!std::is_convertible_v<U &&, T>)
    {
        if (rhs.has_value()) {
            construct(std::move(*rhs));
        }
    }

    optional& operator=(const optional& rhs)
        requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
                 (!std::is_trivially_copy_assignable_v<T>)
    {
        if (!rhs.has_value())
            reset();
        else if (has_value())
            value_ = rhs.value_;
        else
            ::new (&value_) T(rhs.value_);
        return *this;
    }

    optional& operator=(const optional&)
        requires std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T> &&
                     std::is_trivially_copy_constructible_v<T> && std::is_trivially_copy_assignable_v<T>
    = default;

    optional& operator=(optional&& rhs) noexcept(std::is_nothrow_move_constructible_v<T>)
        requires std::is_move_constructible_v<T> && std::is_move_assignable_v<T> &&
                 (!std::is_trivially_move_assignable_v<T>)
    {
        if (!rhs.has_value())
            reset();
        else if (has_value())
            value_ = std::move(rhs.value_);
        else
            ::new (&value_) T(std::move(rhs.value_));
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
    constexpr T value_or(U&& u) const&
        requires std::is_copy_constructible_v<T> && std::is_convertible_v<U&&, T>
    {
        return has_value() ? value() : static_cast<T>(std::forward<U>(u));
    }

    template <class U>
    constexpr T value_or(U&& u) &&
        requires std::is_move_constructible_v<T> && std::is_convertible_v<U&&, T>
    {
        return has_value() ? std::move(value()) : static_cast<T>(std::forward<U>(u));
    }

    template <class F>
    constexpr auto and_then(F&& f) &
        requires detail::is_optional<std::remove_cvref_t<std::invoke_result_t<F, T&>>>::value
    {
        using result = std::invoke_result_t<F, T&>;
        return has_value() ? std::invoke(std::forward<F>(f), value()) : result(nullopt);
    }

    template <class F>
    constexpr auto and_then(F&& f) &&
        requires detail::is_optional<std::remove_cvref_t<std::invoke_result_t<F, T&&>>>::value
    {
        using result = std::invoke_result_t<F, T&&>;
        return has_value() ? std::invoke(std::forward<F>(f), std::move(value())) : result(nullopt);
    }

    template <class F>
    constexpr auto and_then(F&& f) const&
        requires detail::is_optional<std::remove_cvref_t<std::invoke_result_t<F, T&>>>::value
    {
        using result = std::invoke_result_t<F, const T&>;
        return has_value() ? std::invoke(std::forward<F>(f), value()) : result(nullopt);
    }

    template <class F>
    constexpr auto and_then(F&& f) const&&
        requires detail::is_optional<std::remove_cvref_t<std::invoke_result_t<F, T&>>>::value
    {
        using result = std::invoke_result_t<F, const T&>;
        return has_value() ? std::invoke(std::forward<F>(f), value()) : result(nullopt);
    }

    /// Carries out some operation on the stored object if there is one.
    template <class F>
    constexpr auto transform(F&& f) & {
        return optional_map_impl(*this, std::forward<F>(f));
    }

    template <class F>
    constexpr auto transform(F&& f) && {
        return optional_map_impl(std::move(*this), std::forward<F>(f));
    }

    template <class F>
    constexpr auto transform(F&& f) const& {
        return optional_map_impl(*this, std::forward<F>(f));
    }

    template <class F>
    constexpr auto transform(F&& f) const&& {
        return optional_map_impl(std::move(*this), std::forward<F>(f));
    }

    /// Calls `f` if the optional is empty
    template <class F>
    constexpr optional<T> or_else(F&& f) & {
        if (has_value())
            return *this;

        std::forward<F>(f)();
        return nullopt;
    }

    template <class F>
    optional<T> or_else(F&& f) && {
        if (has_value())
            return std::move(*this);

        std::forward<F>(f)();
        return nullopt;
    }

    /// Assigns the stored value from `u`, destroying the old value if there
    /// was one.
    template <class U = T>
    optional& operator=(U&& u)
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
    optional& operator=(const optional<U>& rhs)
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
    optional& operator=(optional<U>&& rhs)
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
    T& emplace(Args&&... args)
        requires std::is_constructible<T, Args&&...>::value
    {
        *this = nullopt;
        construct(std::forward<Args>(args)...);
        return value();
    }

    template <class U, class... Args>
    T& emplace(std::initializer_list<U> il, Args&&... args)
        requires std::is_constructible_v<T, std::initializer_list<U>&, Args&&...>
    {
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
    void swap(optional& rhs) noexcept(std::is_nothrow_move_constructible<T>::value &&
                                      std::is_nothrow_swappable<T>::value) {
        using std::swap;
        if (has_value()) {
            if (rhs.has_value()) {
                swap(value(), *rhs);
            } else {
                new (std::addressof(rhs.value_)) T(std::move(value_));
                value_.T::~T();
            }
        } else if (rhs.has_value()) {
            new (std::addressof(value_)) T(std::move(rhs.value_));
            rhs.value_.T::~T();
        }
        swap(engaged, rhs.engaged);
    }

    /// Returns a pointer to the stored value
    constexpr const T* operator->() const { return std::addressof(value_); }

    constexpr T* operator->() { return std::addressof(value_); }

    /// Returns the stored value
    constexpr T& operator*() & { return value_; }

    constexpr const T& operator*() const& { return value_; }

    constexpr T&& operator*() && { return std::move(value_); }

    /// Returns whether or not the optional has a value
    constexpr bool has_value() const noexcept { return engaged; }

    constexpr explicit operator bool() const noexcept { return engaged; }

    void reset() noexcept {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            if (has_value())
                value_.~T();
        }
        engaged = false;
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

template <class T>
class optional<T&> {
  public:
    using value_type = T&;

    // [optional.ctor], constructors
    //    constexpr optional() noexcept;
    //    constexpr optional(nullopt_t) noexcept;
    //    constexpr optional(const optional&);
    //    constexpr optional(optional&&) noexcept(/* see below */);
    //    template<class U = T>
    //      constexpr explicit(/* see below */ ) optional(U&&);

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
    //    constexpr const T*  operator->() const noexcept;
    //    constexpr T*        operator->() noexcept;
    //  constexpr const T&  operator*() const& noexcept;
    //  constexpr T&        operator*() & noexcept;
    // constexpr T&&       operator*() && noexcept;
    // constexpr const T&& operator*() const&& noexcept;
    // constexpr explicit  operator bool() const noexcept;
    // constexpr bool      has_value() const noexcept;
    // constexpr const T&  value() const&;
    // constexpr T&        value() &;
    // constexpr T&&       value() &&;
    // constexpr const T&& value() const&&;
    // template <class U>
    // constexpr T value_or(U&&) const&;
    // template <class U>
    // constexpr T value_or(U&&) &&;

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
    T* value_; // exposition only

  public:
    constexpr optional() noexcept : value_(nullptr) {}

    constexpr optional(nullopt_t) noexcept : value_(nullptr) {}

    constexpr optional(const optional& rhs) noexcept = default;
    constexpr optional(optional&& rhs)               = default;

    template <class U = T>
        requires(!detail::is_optional<std::decay_t<U>>::value)
    constexpr optional(U&& u) noexcept : value_(std::addressof(u)) {
        static_assert(std::is_constructible_v<std::add_lvalue_reference_t<T>, U>, "Must be able to bind U to T&");
        static_assert(std::is_lvalue_reference<U>::value, "U must be an lvalue");
    }

    template <class U>
    constexpr explicit optional(const optional<U>& rhs) noexcept : optional(*rhs) {}

    ~optional() = default;

    optional& operator=(nullopt_t) noexcept {
        value_ = nullptr;
        return *this;
    }

    optional& operator=(const optional& rhs) = default;

    template <class U = T>
        requires(!detail::is_optional<std::decay_t<U>>::value)
    optional& operator=(U&& u) {
        static_assert(std::is_constructible_v<std::add_lvalue_reference_t<T>, U>, "Must be able to bind U to T&");
        static_assert(std::is_lvalue_reference<U>::value, "U must be an lvalue");
        value_ = std::addressof(u);
        return *this;
    }

    template <class U>
    optional& operator=(const optional<U>& rhs) noexcept {
        static_assert(std::is_constructible_v<std::add_lvalue_reference_t<T>, U>, "Must be able to bind U to T&");
        value_ = std::addressof(rhs.value());
        return *this;
    }

    template <class U = T>
        requires(!detail::is_optional<std::decay_t<U>>::value)
    optional& emplace(U&& u) noexcept {
        return *this = std::forward<U>(u);
    }

    void swap(optional& rhs) noexcept { std::swap(value_, rhs.value_); }

    constexpr const T* operator->() const noexcept { return value_; }
    constexpr T*       operator->() noexcept { return value_; }

    constexpr const T&  operator*() const& noexcept { return *value_; }
    constexpr T&        operator*() & noexcept { return *value_; }
    constexpr T&&       operator*() && noexcept { return *value_; }
    constexpr const T&& operator*() const&& noexcept { return *value_; }

    constexpr explicit operator bool() const noexcept { return value_ != nullptr; }
    constexpr bool     has_value() const noexcept { return value_ != nullptr; }

    constexpr const T& value() const& {
        if (has_value())
            return *value_;
        throw bad_optional_access();
    }
    constexpr T& value() & {
        if (has_value())
            return *value_;
        throw bad_optional_access();
    }
    constexpr T&& value() && {
        if (has_value())
            return *value_;
        throw bad_optional_access();
    }
    constexpr const T&& value() const&& {
        if (has_value())
            return *value_;
        throw bad_optional_access();
    }

    template <class U>
    constexpr T value_or(U&& u) const& {
        static_assert(std::is_copy_constructible<T>::value && std::is_convertible<U&&, T>::value,
                      "T must be copy constructible and convertible from U");
        return has_value() ? value() : static_cast<T>(std::forward<U>(u));
    }

    template <class U>
    constexpr T value_or(U&& u) && {
        static_assert(std::is_move_constructible<T>::value && std::is_convertible<U&&, T>::value,
                      "T must be move constructible and convertible from U");
        return has_value() ? value() : static_cast<T>(std::forward<U>(u));
    }

    template <class F>
    constexpr auto and_then(F&& f) & {
        using result = std::invoke_result_t<F, T&>;
        static_assert(detail::is_optional<result>::value, "F must return an optional");

        return has_value() ? std::invoke(std::forward<F>(f), value()) : result(nullopt);
    }

    template <class F>
    constexpr auto and_then(F&& f) && {
        using result = std::invoke_result_t<F, T&>;
        static_assert(detail::is_optional<result>::value, "F must return an optional");

        return has_value() ? std::invoke(std::forward<F>(f), value()) : result(nullopt);
    }

    template <class F>
    constexpr auto and_then(F&& f) const& {
        using result = std::invoke_result_t<F, const T&>;
        static_assert(detail::is_optional<result>::value, "F must return an optional");

        return has_value() ? std::invoke(std::forward<F>(f), value()) : result(nullopt);
    }

    template <class F>
    constexpr auto and_then(F&& f) const&& {
        using result = std::invoke_result_t<F, const T&>;
        static_assert(detail::is_optional<result>::value, "F must return an optional");

        return has_value() ? std::invoke(std::forward<F>(f), value()) : result(nullopt);
    }

    template <class F>
    constexpr auto transform(F&& f) & {
        return detail::optional_map_impl(*this, std::forward<F>(f));
    }

    template <class F>
    constexpr auto transform(F&& f) && {
        return detail::optional_map_impl(std::move(*this), std::forward<F>(f));
    }

    template <class F>
    constexpr auto transform(F&& f) const& {
        return detail::optional_map_impl(*this, std::forward<F>(f));
    }

    template <class F>
    constexpr auto transform(F&& f) const&& {
        return detail::optional_map_impl(std::move(*this), std::forward<F>(f));
    }
    template <class F>
    constexpr optional or_else(F&& f) && {
        if (*this) {
            return std::move(*this);
        } else {
            return std::forward<F>(f)();
        }
    }
    template <class F>
    constexpr optional or_else(F&& f) const& {
        if (*this) {
            return *this;
        } else {
            return std::forward<F>(f)();
        }
    }

    void reset() noexcept { value_ = nullptr; }
};
} // namespace smd::optional
#endif
