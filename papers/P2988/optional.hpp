// ----------------------
// BASE AND DETAILS ELIDED
// ----------------------

/****************/
/* optional<T&> */
/****************/

template <class T>
class optional<T&> {
  public:
    using value_type = T&;
    using iterator =
        detail::contiguous_iterator<T,
                                    optional>; // see [optionalref.iterators]
    using const_iterator =
        detail::contiguous_iterator<const T,
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
    constexpr explicit(!is_convertible_v<U, T>)
        optional(const optional<U>& rhs) noexcept;

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
    static_assert(is_constructible_v<add_lvalue_reference_t<T>, U>,
                  "Must be able to bind U to T&");
    static_assert(is_lvalue_reference<U>::value, "U must be an lvalue");
}

template <class T>
template <class U>
constexpr optional<T&>::optional(const optional<U>& rhs) noexcept {
    static_assert(is_constructible_v<add_lvalue_reference_t<T>, U>,
                  "Must be able to bind U to T&");
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
    static_assert(is_constructible_v<add_lvalue_reference_t<T>, U>,
                  "Must be able to bind U to T&");
    static_assert(is_lvalue_reference<U>::value, "U must be an lvalue");
    value_ = addressof(u);
    return *this;
}

template <class T>
template <class U>
constexpr optional<T&>&
optional<T&>::operator=(const optional<U>& rhs) noexcept {
    static_assert(is_constructible_v<add_lvalue_reference_t<T>, U>,
                  "Must be able to bind U to T&");
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
    static_assert(is_convertible_v<decltype(u), T>,
                  "Must be able to convert u to T");
    return has_value() ? *value_ : std::forward<U>(u);
}

//   \rSec3[optionalref.monadic]{Monadic operations}
template <class T>
template <class F>
constexpr auto optional<T&>::and_then(F&& f) const {
    using U = invoke_result_t<F, T&>;
    static_assert(detail::is_optional<U>, "F must return an optional");
    return (has_value()) ? invoke(std::forward<F>(f), *value_)
                         : remove_cvref_t<U>();
}

template <class T>
template <class F>
constexpr auto
optional<T&>::transform(F&& f) const -> optional<invoke_result_t<F, T&>> {
    using U = invoke_result_t<F, T&>;
    return (has_value()) ? optional<U>{invoke(std::forward<F>(f), *value_)}
                         : optional<U>{};
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
    using value_type = T&&;
    using iterator =
        detail::contiguous_iterator<T,
                                    optional>; // see [optionalrref.iterators]
    using const_iterator =
        detail::contiguous_iterator<const T,
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
    constexpr explicit(!is_convertible_v<U, T>)
        optional(const optional<U>& rhs) noexcept;

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
    static_assert(is_constructible_v<add_rvalue_reference_t<T>, U>,
                  "Must be able to bind U to T&&");
}

template <class T>
template <class U>
constexpr optional<T&&>::optional(const optional<U>& rhs) noexcept {
    static_assert(is_constructible_v<add_rvalue_reference_t<T>, U>,
                  "Must be able to bind U to T&&");
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
    static_assert(is_constructible_v<add_rvalue_reference_t<T>, U>,
                  "Must be able to bind U to T&&");
    value_ = addressof(u);
    return *this;
}

template <class T>
template <class U>
constexpr optional<T&&>&
optional<T&&>::operator=(const optional<U>& rhs) noexcept {
    static_assert(is_constructible_v<add_rvalue_reference_t<T>, U>,
                  "Must be able to bind U to T&");
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
    static_assert(is_convertible_v<decltype(u), T>,
                  "Must be able to bind u to T");
    return has_value() ? std::move(*value_) : std::forward<U>(u);
}

//   \rSec3[optionalrref.monadic]{Monadic operations}
template <class T>
template <class F>
constexpr auto optional<T&&>::and_then(F&& f) const {
    using U = invoke_result_t<F, T&&>;
    static_assert(detail::is_optional<U>, "F must return an optional");
    return (has_value()) ? invoke(std::forward<F>(f), std::move(*value_))
                         : remove_cvref_t<U>();
}

template <class T>
template <class F>
constexpr auto
optional<T&&>::transform(F&& f) const -> optional<invoke_result_t<F, T&&>> {
    using U = invoke_result_t<F, T&&>;
    return (has_value())
               ? optional<U>{invoke(std::forward<F>(f), std::move(*value_))}
               : optional<U>{};
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
