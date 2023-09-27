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

template <class T>
class optional;

template <typename T>
inline constexpr bool is_optional_v = false;
template <typename T>
inline constexpr bool is_optional_v<optional<T>> = true;

template <class T>
concept is_derived_from_optional =
    requires(const T& t) { []<class U>(const optional<U>&) {}(t); };

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

namespace detail {

template <class T>
struct is_optional_impl : std::false_type {};
template <class T>
struct is_optional_impl<optional<T>> : std::true_type {};
template <class T>
using is_optional = is_optional_impl<std::decay_t<T>>;

// The storage base manages the actual storage, and correctly propagates
// trivial destruction from T. This case is for when T is not trivially
// destructible.
template <class T, bool = ::std::is_trivially_destructible<T>::value>
struct optional_storage_base {
    constexpr optional_storage_base() noexcept
        : m_dummy(), m_has_value(false) {}

    template <class... U>
    constexpr optional_storage_base(in_place_t, U&&... u)
        : m_value(std::forward<U>(u)...), m_has_value(true) {}

    ~optional_storage_base() {
        if (m_has_value) {
            m_value.~T();
            m_has_value = false;
        }
    }

    struct dummy {};
    union {
        dummy m_dummy;
        T     m_value;
    };

    bool m_has_value;
};

// This case is for when T is trivially destructible.
template <class T>
struct optional_storage_base<T, true> {
    constexpr optional_storage_base() noexcept
        : m_dummy(), m_has_value(false) {}

    template <class... U>
    constexpr optional_storage_base(in_place_t, U&&... u)
        : m_value(std::forward<U>(u)...), m_has_value(true) {}

    // No destructor, so this class is trivially destructible

    struct dummy {};
    union {
        dummy m_dummy;
        T     m_value;
    };

    bool m_has_value = false;
};

// This base class provides some handy member functions which can be used in
// further derived classes
template <class T>
struct optional_operations_base : optional_storage_base<T> {
    using optional_storage_base<T>::optional_storage_base;

    void hard_reset() noexcept {
        get().~T();
        this->m_has_value = false;
    }

    template <class... Args>
    void construct(Args&&... args) {
        new (std::addressof(this->m_value)) T(std::forward<Args>(args)...);
        this->m_has_value = true;
    }

    template <class Opt>
    void assign(Opt&& rhs) {
        if (this->has_value()) {
            if (rhs.has_value()) {
                this->m_value = std::forward<Opt>(rhs).get();
            } else {
                this->m_value.~T();
                this->m_has_value = false;
            }
        }

        else if (rhs.has_value()) {
            construct(std::forward<Opt>(rhs).get());
        }
    }

    bool has_value() const { return this->m_has_value; }

    constexpr T&        get() & { return this->m_value; }
    constexpr const T&  get() const& { return this->m_value; }
    constexpr T&&       get() && { return std::move(this->m_value); }
    constexpr const T&& get() const&& { return std::move(this->m_value); }
};

// This class manages conditionally having a trivial copy constructor
// This specialization is for when T is trivially copy constructible
template <class T, bool = std::is_trivially_copy_constructible<T>::value>
struct optional_copy_base : optional_operations_base<T> {
    using optional_operations_base<T>::optional_operations_base;
};

// This specialization is for when T is not trivially copy constructible
template <class T>
struct optional_copy_base<T, false> : optional_operations_base<T> {
    using optional_operations_base<T>::optional_operations_base;

    optional_copy_base() = default;
    optional_copy_base(const optional_copy_base& rhs)
        : optional_operations_base<T>() {
        if (rhs.has_value()) {
            this->construct(rhs.get());
        } else {
            this->m_has_value = false;
        }
    }

    optional_copy_base(optional_copy_base&& rhs)                 = default;
    optional_copy_base& operator=(const optional_copy_base& rhs) = default;
    optional_copy_base& operator=(optional_copy_base&& rhs)      = default;
};

// This class manages conditionally having a trivial move constructor
template <class T, bool = std::is_trivially_move_constructible<T>::value>
struct optional_move_base : optional_copy_base<T> {
    using optional_copy_base<T>::optional_copy_base;
};

template <class T>
struct optional_move_base<T, false> : optional_copy_base<T> {
    using optional_copy_base<T>::optional_copy_base;

    optional_move_base()                              = default;
    optional_move_base(const optional_move_base& rhs) = default;

    optional_move_base(optional_move_base&& rhs) noexcept(
        std::is_nothrow_move_constructible<T>::value) {
        if (rhs.has_value()) {
            this->construct(std::move(rhs.get()));
        } else {
            this->m_has_value = false;
        }
    }
    optional_move_base& operator=(const optional_move_base& rhs) = default;
    optional_move_base& operator=(optional_move_base&& rhs)      = default;
};

// This class manages conditionally having a trivial copy assignment operator
template <class T,
          bool = std::is_trivially_copy_assignable<T>::value &&
                 std::is_trivially_copy_constructible<T>::value &&
                 std::is_trivially_destructible<T>::value>
struct optional_copy_assign_base : optional_move_base<T> {
    using optional_move_base<T>::optional_move_base;
};

template <class T>
struct optional_copy_assign_base<T, false> : optional_move_base<T> {
    using optional_move_base<T>::optional_move_base;

    optional_copy_assign_base()                                     = default;
    optional_copy_assign_base(const optional_copy_assign_base& rhs) = default;

    optional_copy_assign_base(optional_copy_assign_base&& rhs) = default;
    optional_copy_assign_base&
    operator=(const optional_copy_assign_base& rhs) {
        this->assign(rhs);
        return *this;
    }
    optional_copy_assign_base&
    operator=(optional_copy_assign_base&& rhs) = default;
};

// This class manages conditionally having a trivial move assignment operator
template <class T,
          bool = std::is_trivially_destructible<T>::value &&
                 std::is_trivially_move_constructible<T>::value &&
                 std::is_trivially_move_assignable<T>::value>
struct optional_move_assign_base : optional_copy_assign_base<T> {
    using optional_copy_assign_base<T>::optional_copy_assign_base;
};

template <class T>
struct optional_move_assign_base<T, false> : optional_copy_assign_base<T> {
    using optional_copy_assign_base<T>::optional_copy_assign_base;

    optional_move_assign_base()                                     = default;
    optional_move_assign_base(const optional_move_assign_base& rhs) = default;

    optional_move_assign_base(optional_move_assign_base&& rhs) = default;

    optional_move_assign_base&
    operator=(const optional_move_assign_base& rhs) = default;

    optional_move_assign_base&
    operator=(optional_move_assign_base&& rhs) noexcept(
        std::is_nothrow_move_constructible<T>::value &&
        std::is_nothrow_move_assignable<T>::value) {
        this->assign(std::move(rhs));
        return *this;
    }
};

// optional_delete_ctor_base will conditionally delete copy and move
// constructors depending on whether T is copy/move constructible
template <class T,
          bool EnableCopy = std::is_copy_constructible<T>::value,
          bool EnableMove = std::is_move_constructible<T>::value>
struct optional_delete_ctor_base {
    optional_delete_ctor_base()                                     = default;
    optional_delete_ctor_base(const optional_delete_ctor_base&)     = default;
    optional_delete_ctor_base(optional_delete_ctor_base&&) noexcept = default;
    optional_delete_ctor_base&
    operator=(const optional_delete_ctor_base&) = default;
    optional_delete_ctor_base&
    operator=(optional_delete_ctor_base&&) noexcept = default;
};

template <class T>
struct optional_delete_ctor_base<T, true, false> {
    optional_delete_ctor_base()                                     = default;
    optional_delete_ctor_base(const optional_delete_ctor_base&)     = default;
    optional_delete_ctor_base(optional_delete_ctor_base&&) noexcept = delete;
    optional_delete_ctor_base&
    operator=(const optional_delete_ctor_base&) = default;
    optional_delete_ctor_base&
    operator=(optional_delete_ctor_base&&) noexcept = default;
};

template <class T>
struct optional_delete_ctor_base<T, false, true> {
    optional_delete_ctor_base()                                     = default;
    optional_delete_ctor_base(const optional_delete_ctor_base&)     = delete;
    optional_delete_ctor_base(optional_delete_ctor_base&&) noexcept = default;
    optional_delete_ctor_base&
    operator=(const optional_delete_ctor_base&) = default;
    optional_delete_ctor_base&
    operator=(optional_delete_ctor_base&&) noexcept = default;
};

template <class T>
struct optional_delete_ctor_base<T, false, false> {
    optional_delete_ctor_base()                                     = default;
    optional_delete_ctor_base(const optional_delete_ctor_base&)     = delete;
    optional_delete_ctor_base(optional_delete_ctor_base&&) noexcept = delete;
    optional_delete_ctor_base&
    operator=(const optional_delete_ctor_base&) = default;
    optional_delete_ctor_base&
    operator=(optional_delete_ctor_base&&) noexcept = default;
};

// optional_delete_assign_base will conditionally delete copy and move
// constructors depending on whether T is copy/move constructible + assignable
template <class T,
          bool EnableCopy = (std::is_copy_constructible<T>::value &&
                             std::is_copy_assignable<T>::value),
          bool EnableMove = (std::is_move_constructible<T>::value &&
                             std::is_move_assignable<T>::value)>
struct optional_delete_assign_base {
    optional_delete_assign_base()                                   = default;
    optional_delete_assign_base(const optional_delete_assign_base&) = default;
    optional_delete_assign_base(optional_delete_assign_base&&) noexcept =
        default;
    optional_delete_assign_base&
    operator=(const optional_delete_assign_base&) = default;
    optional_delete_assign_base&
    operator=(optional_delete_assign_base&&) noexcept = default;
};

template <class T>
struct optional_delete_assign_base<T, true, false> {
    optional_delete_assign_base()                                   = default;
    optional_delete_assign_base(const optional_delete_assign_base&) = default;
    optional_delete_assign_base(optional_delete_assign_base&&) noexcept =
        default;
    optional_delete_assign_base&
    operator=(const optional_delete_assign_base&) = default;
    optional_delete_assign_base&
    operator=(optional_delete_assign_base&&) noexcept = delete;
};

template <class T>
struct optional_delete_assign_base<T, false, true> {
    optional_delete_assign_base()                                   = default;
    optional_delete_assign_base(const optional_delete_assign_base&) = default;
    optional_delete_assign_base(optional_delete_assign_base&&) noexcept =
        default;
    optional_delete_assign_base&
    operator=(const optional_delete_assign_base&) = delete;
    optional_delete_assign_base&
    operator=(optional_delete_assign_base&&) noexcept = default;
};

template <class T>
struct optional_delete_assign_base<T, false, false> {
    optional_delete_assign_base()                                   = default;
    optional_delete_assign_base(const optional_delete_assign_base&) = default;
    optional_delete_assign_base(optional_delete_assign_base&&) noexcept =
        default;
    optional_delete_assign_base&
    operator=(const optional_delete_assign_base&) = delete;
    optional_delete_assign_base&
    operator=(optional_delete_assign_base&&) noexcept = delete;
};

} // namespace detail

template <typename T>
constexpr optional<std::decay_t<T>> make_optional(T&& __t) noexcept(
    std::is_nothrow_constructible_v<optional<std::decay_t<T>>, T>)
    requires std::is_constructible_v<std::decay_t<T>, T>
{
    return optional<std::decay_t<T>>{std::forward<T>(__t)};
}

template <typename T, typename... _Args>
constexpr optional<T> make_optional(_Args&&... __args) noexcept(
    std::is_nothrow_constructible_v<T, _Args...>)
    requires std::is_constructible_v<T, _Args...>
{
    return optional<T>{in_place, std::forward<_Args>(__args)...};
}

template <typename T, typename _Up, typename... _Args>
constexpr optional<T>
make_optional(std::initializer_list<_Up> __il, _Args&&... __args) noexcept(
    std::is_nothrow_constructible_v<T, std::initializer_list<_Up>&, _Args...>)
    requires std::is_constructible_v<T, std::initializer_list<_Up>&, _Args...>
{
    return optional<T>{in_place, __il, std::forward<_Args>(__args)...};
}

template <class T_, class U_>
concept enable_forward_value = std::is_constructible_v<T_, U_&&> &&
                               !std::is_same_v<std::decay_t<U_>, in_place_t> &&
                               !std::is_same_v<optional<T_>, std::decay_t<U_>>;

template <class T_, class U_, class Other>
concept enable_from_other =
    std::is_constructible_v<T_, Other> &&
    !std::is_constructible_v<T_, optional<U_>&> &&
    !std::is_constructible_v<T_, optional<U_>&&> &&
    !std::is_constructible_v<T_, const optional<U_>&> &&
    !std::is_constructible_v<T_, const optional<U_>&&> &&
    !std::is_convertible_v<optional<U_>&, T_> &&
    !std::is_convertible_v<optional<U_>&&, T_> &&
    !std::is_convertible_v<const optional<U_>&, T_> &&
    !std::is_convertible_v<const optional<U_>&&, T_>;

template <class T, class U>
concept enable_assign_forward =
    !std::is_same_v<optional<T>, std::decay_t<U>> &&
    !std::conjunction_v<std::is_scalar<T>, std::is_same<T, std::decay_t<U>>> &&
    std::is_constructible_v<T, U> && std::is_assignable_v<T&, U>;

template <class T_, class U_, class Other>
concept enable_assign_from_other =
    std::is_constructible_v<T_, Other> && std::is_assignable_v<T_&, Other> &&
    !std::is_constructible_v<T_, optional<U_>&> &&
    !std::is_constructible_v<T_, optional<U_>&&> &&
    !std::is_constructible_v<T_, const optional<U_>&> &&
    !std::is_constructible_v<T_, const optional<U_>&&> &&
    !std::is_convertible_v<optional<U_>&, T_> &&
    !std::is_convertible_v<optional<U_>&&, T_> &&
    !std::is_convertible_v<const optional<U_>&, T_> &&
    !std::is_convertible_v<const optional<U_>&&, T_> &&
    !std::is_assignable_v<T_&, optional<U_>&> &&
    !std::is_assignable_v<T_&, optional<U_>&&> &&
    !std::is_assignable_v<T_&, const optional<U_>&> &&
    !std::is_assignable_v<T_&, const optional<U_>&&>;

template <class T>
class optional : private detail::optional_move_assign_base<T>,
                 private detail::optional_delete_ctor_base<T>,
                 private detail::optional_delete_assign_base<T> {
    using base = detail::optional_move_assign_base<T>;

    static_assert(!std::is_same<T, in_place_t>::value,
                  "instantiation of optional with in_place_t is ill-formed");
    static_assert(!std::is_same<std::decay_t<T>, nullopt_t>::value,
                  "instantiation of optional with nullopt_t is ill-formed");

  public:
    template <class F>
    constexpr auto and_then(F&& f) & {
        using result = std::invoke_result_t<F, T&>;
        static_assert(detail::is_optional<result>::value,
                      "F must return an optional");

        return this->has_value() ? std::invoke(std::forward<F>(f), **this)
                                 : result(nullopt);
    }

    template <class F>
    constexpr auto and_then(F&& f) && {
        using result = std::invoke_result_t<F, T&&>;
        static_assert(detail::is_optional<result>::value,
                      "F must return an optional");

        return this->has_value()
                   ? std::invoke(std::forward<F>(f), std::move(**this))
                   : result(nullopt);
    }

    template <class F>
    constexpr auto and_then(F&& f) const& {
        using result = std::invoke_result_t<F, const T&>;
        static_assert(detail::is_optional<result>::value,
                      "F must return an optional");

        return this->has_value() ? std::invoke(std::forward<F>(f), **this)
                                 : result(nullopt);
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
        if (this->has_value())
            return *this;

        std::forward<F>(f)();
        return nullopt;
    }

    template <class F>
    optional<T> or_else(F&& f) && {
        if (this->has_value())
            return std::move(*this);

        std::forward<F>(f)();
        return nullopt;
    }

    /// Constructs an optional that does not contain a value.
    constexpr optional() noexcept = default;

    constexpr optional(nullopt_t) noexcept {}

    /// Copy constructor
    ///
    /// If `rhs` contains a value, the stored value is direct-initialized with
    /// it. Otherwise, the constructed optional is empty.
    constexpr optional(const optional& rhs) = default;

    /// Move constructor
    ///
    /// If `rhs` contains a value, the stored value is direct-initialized with
    /// it. Otherwise, the constructed optional is empty.
    constexpr optional(optional&& rhs) = default;

    /// Constructs the stored value in-place using the given arguments.
    template <class... Args>
    constexpr explicit optional(in_place_t, Args&&... args)
        requires std::is_constructible_v<T, Args...>
        : base(in_place, std::forward<Args>(args)...) {}

    template <class U, class... Args>
    constexpr explicit optional(in_place_t,
                                std::initializer_list<U> il,
                                Args&&... args)
        requires std::
            is_constructible_v<T, std::initializer_list<U>&, Args&&...>
    {
        this->construct(il, std::forward<Args>(args)...);
    }

    /// Constructs the stored value with `u`.
    template <class U = T>
    constexpr optional(U&& u)
        requires enable_forward_value<T, U> && std::is_convertible_v<U&&, T>
        : base(in_place, std::forward<U>(u)) {}

    template <class U = T>
    constexpr explicit optional(U&& u)
        requires enable_forward_value<T, U> &&
                 (!std::is_convertible_v<U &&, T>)
        : base(in_place, std::forward<U>(u)) {}

    /// Converting copy constructor.
    template <class U>
    optional(const optional<U>& rhs)
        requires enable_from_other<T, U, const U&> &&
                 std::is_convertible_v<const U&, T>
    {
        if (rhs.has_value()) {
            this->construct(*rhs);
        }
    }

    template <class U>
    explicit optional(const optional<U>& rhs)
        requires enable_from_other<T, U, const U&> &&
                 (!std::is_convertible_v<const U&, T>)
    {
        if (rhs.has_value()) {
            this->construct(*rhs);
        }
    }

    /// Converting move constructor.
    template <class U>
    optional(optional<U>&& rhs)
        requires enable_from_other<T, U, U&&> && std::is_convertible_v<U&&, T>
    {
        if (rhs.has_value()) {
            this->construct(std::move(*rhs));
        }
    }

    template <class U>
    explicit optional(optional<U>&& rhs)
        requires enable_from_other<T, U, U&&> &&
                 (!std::is_convertible_v<U &&, T>)
    {
        if (rhs.has_value()) {
            this->construct(std::move(*rhs));
        }
    }

    /// Destroys the stored value if there is one.
    ~optional() = default;

    /// Assignment to empty.
    ///
    /// Destroys the current value if there is one.
    optional& operator=(nullopt_t) noexcept {
        if (has_value()) {
            this->m_value.~T();
            this->m_has_value = false;
        }

        return *this;
    }

    /// Copy assignment.
    ///
    /// Copies the value from `rhs` if there is one. Otherwise resets the
    /// stored value in `*this`.
    optional& operator=(const optional& rhs) = default;

    /// Move assignment.
    ///
    /// Moves the value from `rhs` if there is one. Otherwise resets the stored
    /// value in `*this`.
    optional& operator=(optional&& rhs) = default;

    /// Assigns the stored value from `u`, destroying the old value if there
    /// was one.
    template <class U = T>
    optional& operator=(U&& u)
        requires enable_assign_forward<T, U>
    {
        if (has_value()) {
            this->m_value = std::forward<U>(u);
        } else {
            this->construct(std::forward<U>(u));
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
                this->m_value = *rhs;
            } else {
                this->hard_reset();
            }
        }

        else if (rhs.has_value()) {
            this->construct(*rhs);
        }

        return *this;
    }

    // TODO check exception guarantee
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
                this->m_value = std::move(*rhs);
            } else {
                this->hard_reset();
            }
        }

        else if (rhs.has_value()) {
            this->construct(std::move(*rhs));
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
        this->construct(std::forward<Args>(args)...);
        return value();
    }

    template <class U, class... Args>
    T& emplace(std::initializer_list<U> il, Args&&... args)
        requires std::
            is_constructible_v<T, std::initializer_list<U>&, Args&&...>
    {
        *this = nullopt;
        this->construct(il, std::forward<Args>(args)...);
        return value();
    }

    /// Swaps this optional with the other.
    ///
    /// If neither optionals have a value, nothing happens.
    /// If both have a value, the values are swapped.
    /// If one has a value, it is moved to the other and the movee is left
    /// valueless.
    void swap(optional& rhs) noexcept(
        std::is_nothrow_move_constructible<T>::value &&
        std::is_nothrow_swappable<T>::value) {
        using std::swap;
        if (has_value()) {
            if (rhs.has_value()) {
                swap(**this, *rhs);
            } else {
                new (std::addressof(rhs.m_value)) T(std::move(this->m_value));
                this->m_value.T::~T();
            }
        } else if (rhs.has_value()) {
            new (std::addressof(this->m_value)) T(std::move(rhs.m_value));
            rhs.m_value.T::~T();
        }
        swap(this->m_has_value, rhs.m_has_value);
    }

    /// Returns a pointer to the stored value
    constexpr const T* operator->() const {
        return std::addressof(this->m_value);
    }

    constexpr T* operator->() { return std::addressof(this->m_value); }

    /// Returns the stored value
    constexpr T& operator*() & { return this->m_value; }

    constexpr const T& operator*() const& { return this->m_value; }

    constexpr T&& operator*() && { return std::move(this->m_value); }

    /// Returns whether or not the optional has a value
    constexpr bool has_value() const noexcept { return this->m_has_value; }

    constexpr explicit operator bool() const noexcept {
        return this->m_has_value;
    }

    /// Returns the contained value if there is one, otherwise throws
    /// bad_optional_access
    constexpr T& value() & {
        if (has_value())
            return this->m_value;
        throw bad_optional_access();
    }
    constexpr const T& value() const& {
        if (has_value())
            return this->m_value;
        throw bad_optional_access();
    }
    constexpr T&& value() && {
        if (has_value())
            return std::move(this->m_value);
        throw bad_optional_access();
    }

    /// Returns the stored value if there is one, otherwise returns `u`
    template <class U>
    constexpr T value_or(U&& u) const&
        requires std::is_copy_constructible_v<T> &&
                 std::is_convertible_v<U&&, T>
    {
        return has_value() ? **this : static_cast<T>(std::forward<U>(u));
    }

    template <class U>
    constexpr T value_or(U&& u) &&
        requires std::is_move_constructible_v<T> &&
                 std::is_convertible_v<U&&, T>
    {
        return has_value() ? std::move(**this)
                           : static_cast<T>(std::forward<U>(u));
    }

    /// Destroys the stored value if one exists, making the optional empty
    void reset() noexcept {
        if (has_value()) {
            this->m_value.~T();
            this->m_has_value = false;
        }
    }
};

template <typename T>
using optional_relop_t =
    std::enable_if_t<std::is_convertible<T, bool>::value, bool>;

template <typename T, typename U>
using optional_eq_t = optional_relop_t<decltype(std::declval<const T&>() ==
                                                std::declval<const U&>())>;

template <typename T, typename U>
using optional_ne_t = optional_relop_t<decltype(std::declval<const T&>() !=
                                                std::declval<const U&>())>;

template <typename T, typename U>
using optional_lt_t = optional_relop_t<decltype(std::declval<const T&>() <
                                                std::declval<const U&>())>;

template <typename T, typename U>
using optional_gt_t = optional_relop_t<decltype(std::declval<const T&>() >
                                                std::declval<const U&>())>;

template <typename T, typename U>
using optional_le_t = optional_relop_t<decltype(std::declval<const T&>() <=
                                                std::declval<const U&>())>;

template <typename T, typename U>
using optional_ge_t = optional_relop_t<decltype(std::declval<const T&>() >=
                                                std::declval<const U&>())>;

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
    return static_cast<bool>(lhs) == static_cast<bool>(rhs) &&
           (!lhs || *lhs == *rhs);
}

template <typename T, typename U>
constexpr bool operator!=(const optional<T>& lhs, const optional<U>& rhs)
    requires optional_ne_rel<T, U>
{
    return static_cast<bool>(lhs) != static_cast<bool>(rhs) ||
           (static_cast<bool>(lhs) && *lhs != *rhs);
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
constexpr std::compare_three_way_result_t<T, U>
operator<=>(const optional<T>& x, const optional<U>& y) {
    return x && y ? *x <=> *y : bool(x) <=> bool(y);
}

// Comparisons with nullopt.
template <typename T>
constexpr bool operator==(const optional<T>& lhs, nullopt_t) noexcept {
    return !lhs;
}

template <typename T>
constexpr std::strong_ordering operator<=>(const optional<T>& x,
                                           nullopt_t) noexcept {
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
    requires(!is_optional_v<U>) && std::three_way_comparable_with<T, U>
constexpr std::compare_three_way_result_t<T, U>
operator<=>(const optional<T>& x, const U& v) {
    return bool(x) ? *x <=> v : std::strong_ordering::less;
}

template <class T>
void swap(optional<T>& lhs, optional<T>& rhs) noexcept(noexcept(lhs.swap(rhs)))
    requires std::is_move_constructible_v<T> && std::is_swappable_v<T>
{
    return lhs.swap(rhs);
}

namespace detail {
template <class Opt,
          class F,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           *std::declval<Opt>()))>
constexpr auto optional_map_impl(Opt&& opt, F&& f)
    requires(!std::is_void_v<Ret>)
{
    return opt.has_value()
               ? std::invoke(std::forward<F>(f), *std::forward<Opt>(opt))
               : optional<Ret>(nullopt);
}

template <class Opt,
          class F,
          class Ret = decltype(std::invoke(std::declval<F>(),
                                           *std::declval<Opt>()))>
auto optional_map_impl(Opt&& opt, F&& f) requires std::is_void_v<Ret> {
    if (opt.has_value()) {
        std::invoke(std::forward<F>(f), *std::forward<Opt>(opt));
        return make_optional(monostate{});
    }

    return optional<monostate>(nullopt);
}
} // namespace detail

} // namespace smd::optional
#endif
