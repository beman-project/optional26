// ----------------------
// BASE AND DETAILS ELIDED
// ----------------------

/****************/
/* optional<T&> */
/****************/

template <class T>
class optional<T&> {
  public:
    using value_type = T;
    using iterator =
        contiguous_iterator<T,
                            optional>; // see [optionalref.iterators]
  public:
    // \ref{optionalref.ctor}, constructors

    constexpr optional() noexcept;
    constexpr optional(nullopt_t) noexcept;
    constexpr optional(const optional& rhs) noexcept = default;

    template <class Arg>
        requires(is_constructible_v<T&, remove_cv_t<Arg>> &&
                 !reference_constructs_from_temporary_v<T&, Arg>)
    constexpr explicit optional(in_place_t, Arg&& arg);

    template <class U>
        requires(is_constructible_v<T&, U> &&
                 !(is_same_v<remove_cvref_t<U>, in_place_t>) &&
                 !(is_same_v<remove_cvref_t<U>, optional>) &&
                 !reference_constructs_from_temporary_v<T&, U>)
    constexpr explicit(!is_convertible_v<U, T&>)
        optional(U&& u) noexcept(is_nothrow_constructible_v<T&, U>)
        : value_(addressof(static_cast<T&>(std::forward<U>(u)))) {}

    template <class U = T>
        requires(is_constructible_v<T&, U> &&
                 !(is_same_v<remove_cvref_t<U>, in_place_t>) &&
                 !(is_same_v<remove_cvref_t<U>, optional>) &&
                 reference_constructs_from_temporary_v<T&, U>)
    constexpr optional(U&& u) = delete;

    // The full set of 4 overloads on optional<U> by value category, doubled to
    // 8 by deleting if reference_constructs_from_temporary_v is true. This
    // allows correct constraints by propagating the value category from the
    // optional to the value within the rhs.
    template <class U>
        requires(is_constructible_v<T&, U&> &&
                 !is_same_v<remove_cv_t<T>, optional<U>> &&
                 !is_same_v<T&, U> &&
                 !reference_constructs_from_temporary_v<T&, U&>)
    constexpr explicit(!is_convertible_v<U&, T&>) optional(
        optional<U>& rhs) noexcept(is_nothrow_constructible_v<T&, U&>);

    template <class U>
        requires(is_constructible_v<T&, const U&> &&
                 !is_same_v<remove_cv_t<T>, optional<U>> &&
                 !is_same_v<T&, U> &&
                 !reference_constructs_from_temporary_v<T&, const U&>)
    constexpr explicit(!is_convertible_v<const U&, T&>)
        optional(const optional<U>& rhs) noexcept(
            is_nothrow_constructible_v<T&, const U&>);

    template <class U>
        requires(is_constructible_v<T&, U> &&
                 !is_same_v<remove_cv_t<T>, optional<U>> &&
                 !is_same_v<T&, U> &&
                 !reference_constructs_from_temporary_v<T&, U>)
    constexpr explicit(!is_convertible_v<U, T&>)
        optional(optional<U>&& rhs) noexcept(
            noexcept(is_nothrow_constructible_v<T&, U>));

    template <class U>
        requires(is_constructible_v<T&, const U> &&
                 !is_same_v<remove_cv_t<T>, optional<U>> &&
                 !is_same_v<T&, U> &&
                 !reference_constructs_from_temporary_v<T&, const U>)
    constexpr explicit(!is_convertible_v<const U, T&>)
        optional(const optional<U>&& rhs) noexcept(
            noexcept(is_nothrow_constructible_v<T&, const U>));

    template <class U>
        requires(is_constructible_v<T&, U&> &&
                 !is_same_v<remove_cv_t<T>, optional<U>> &&
                 !is_same_v<T&, U> &&
                 reference_constructs_from_temporary_v<T&, U&>)
    constexpr optional(optional<U>& rhs) = delete;

    template <class U>
        requires(is_constructible_v<T&, const U&> &&
                 !is_same_v<remove_cv_t<T>, optional<U>> &&
                 !is_same_v<T&, U> &&
                 reference_constructs_from_temporary_v<T&, const U&>)
    constexpr optional(const optional<U>& rhs) = delete;

    template <class U>
        requires(is_constructible_v<T&, U> &&
                 !is_same_v<remove_cv_t<T>, optional<U>> &&
                 !is_same_v<T&, U> &&
                 reference_constructs_from_temporary_v<T&, U>)
    constexpr optional(optional<U>&& rhs) = delete;

    template <class U>
        requires(is_constructible_v<T&, const U> &&
                 !is_same_v<remove_cv_t<T>, optional<U>> &&
                 !is_same_v<T&, U> &&
                 reference_constructs_from_temporary_v<T&, const U>)
    constexpr optional(const optional<U>&& rhs) = delete;

    // \ref{optionalref.dtor}, destructor
    constexpr ~optional() = default;

    // \ref{optionalref.assign}, assignment
    constexpr optional& operator=(nullopt_t) noexcept;

    constexpr optional& operator=(const optional& rhs) noexcept = default;

    template <class U>
        requires(is_constructible_v<T&, U> &&
                 !reference_constructs_from_temporary_v<T&, U>);
    constexpr T& emplace(U&& u) noexcept(is_nothrow_constructible_v<T&, U>))

    // \ref{optionalref.swap}, swap
    constexpr void swap(optional& rhs) noexcept;

    // \ref{optional.iterators}, iterator support
    constexpr iterator begin() const noexcept;
    constexpr iterator end() const noexcept;

    // \ref{optionalref.observe}, observers
    constexpr T*       operator->() const noexcept;
    constexpr T&       operator*() const noexcept;
    constexpr explicit operator bool() const noexcept;
    constexpr bool     has_value() const noexcept;
    constexpr T&       value() const;
    template <class U>
    constexpr remove_cv_t<T> value_or(U&& u) const;

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
template <class Arg>
    requires(is_constructible_v<T&, remove_cv_t<Arg>> &&
             !reference_constructs_from_temporary_v<T&, Arg>)
constexpr optional<T&>::optional(in_place_t, Arg&& arg)
    : value_(addressof(static_cast<T&>(std::forward<Arg>(arg)))) {}

// Clang is unhappy with the out-of-line definition
//
// template <class T>
// template <class U>
//     requires(is_constructible_v<T&, U> && !(is_same_v<remove_cvref_t<U>,
//     in_place_t>) &&
//              !(is_same_v<remove_cvref_t<U>, optional<T&>>) &&
//              !reference_constructs_from_temporary_v<T&, U>)
// constexpr optional<T&>::optional(U&& u)
// noexcept(is_nothrow_constructible_v<T&, U>)
//     : value_(addressof(static_cast<T&>(forward<U>(u)))) {}

template <class T>
template <class U>
    requires(is_constructible_v<T&, U&> &&
             !is_same_v<remove_cv_t<T>, optional<U>> && !is_same_v<T&, U> &&
             !reference_constructs_from_temporary_v<T&, U&>)
constexpr optional<T&>::optional(optional<U>& rhs) noexcept(
    is_nothrow_constructible_v<T&, U&>) {
    if (rhs.has_value()) {
        value_ = addressof(static_cast<T&>(rhs.value()));
    } else {
        value_ = nullptr;
    }
}

template <class T>
template <class U>
    requires(is_constructible_v<T&, const U&> &&
             !is_same_v<remove_cv_t<T>, optional<U>> && !is_same_v<T&, U> &&
             !reference_constructs_from_temporary_v<T&, const U&>)
constexpr optional<T&>::optional(const optional<U>& rhs) noexcept(
    is_nothrow_constructible_v<T&, const U&>) {
    if (rhs.has_value()) {
        value_ = addressof(static_cast<T&>(rhs.value()));
    } else {
        value_ = nullptr;
    }
}

template <class T>
template <class U>
    requires(is_constructible_v<T&, U> &&
             !is_same_v<remove_cv_t<T>, optional<U>> && !is_same_v<T&, U> &&
             !reference_constructs_from_temporary_v<T&, U>)
constexpr optional<T&>::optional(optional<U>&& rhs) noexcept(
    noexcept(is_nothrow_constructible_v<T&, U>)) {
    if (rhs.has_value()) {
        value_ = addressof(static_cast<T&>(std::move(rhs).value()));
    } else {
        value_ = nullptr;
    }
}

template <class T>
template <class U>
    requires(is_constructible_v<T&, const U> &&
             !is_same_v<remove_cv_t<T>, optional<U>> && !is_same_v<T&, U> &&
             !reference_constructs_from_temporary_v<T&, const U>)
constexpr optional<T&>::optional(const optional<U>&& rhs) noexcept(
    noexcept(is_nothrow_constructible_v<T&, const U>)) {
    if (rhs.has_value()) {
        value_ = addressof(static_cast<T&>(std::move(rhs).value()));
    } else {
        value_ = nullptr;
    }
}

// \rSec3[optionalref.assign]{Assignment}
template <class T>
constexpr optional<T&>& optional<T&>::operator=(nullopt_t) noexcept {
    value_ = nullptr;
    return *this;
}

template <class T>
template <class U>
constexpr T& optional<T&>::emplace(U&& u) noexcept
    requires(is_constructible_v<T&, U> &&
             !reference_constructs_from_temporary_v<T&, U>)
{
    value_ = addressof(static_cast<T&>(std::forward<U>(u)));
    return *value_;
}

//   \rSec3[optionalref.swap]{Swap}

template <class T>
constexpr void optional<T&>::swap(optional<T&>& rhs) noexcept {
    swap(value_, rhs.value_);
}

// \rSec3[optionalref.iterators]{Iterator Support}

template <class T>
constexpr optional<T&>::iterator optional<T&>::begin() const noexcept {
    return iterator(has_value() ? value_ : nullptr);
};

template <class T>
constexpr optional<T&>::iterator optional<T&>::end() const noexcept {
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
constexpr remove_cv_t<T> optional<T&>::value_or(U&& u) const {
    static_assert(is_constructible_v<remove_cv_t<T>, T&>,
                  "T must be constructible from a T&");
    static_assert(is_convertible_v<U, remove_cv_t<T>>,
                  "Must be able to convert u to T");
    if (has_value()) {
        return remove_cv_t<T>(*value_);
    } else {
        return std::forward<U>(u);
    }
}

//   \rSec3[optionalref.monadic]{Monadic operations}
template <class T>
template <class F>
constexpr auto optional<T&>::and_then(F&& f) const {
    using U = invoke_result_t<F, T&>;
    static_assert(is_optional<U>, "F must return an optional");
    if (has_value()) {
        return invoke(std::forward<F>(f), *value_);
    } else {
        return remove_cvref_t<U>();
    }
}

template <class T>
template <class F>
constexpr auto
optional<T&>::transform(F&& f) const -> optional<invoke_result_t<F, T&>> {
    using U = invoke_result_t<F, T&>;
    if (has_value()) {
        return optional<U>{invoke(std::forward<F>(f), *value_)};
    } else {
        return optional<U>{};
    }
}

template <class T>
template <class F>
constexpr optional<T&> optional<T&>::or_else(F&& f) const {
    using U = invoke_result_t<F>;
    static_assert(is_same_v<remove_cvref_t<U>, optional>);
    if (has_value()) {
        return *value_;
    } else {
        return std::forward<F>(f)();
    }
}

// \rSec3[optional.mod]{modifiers}
template <class T>
constexpr void optional<T&>::reset() noexcept {
    value_ = nullptr;
}
