#ifndef TYPETRAITS_H
#define TYPETRAITS_H

template<typename...>
using voidType = void;

template<typename T>
T&& declVal() noexcept;

struct TrueType {
    static constexpr bool value = true;
    using valueType = bool;
    using type = TrueType;

    constexpr explicit operator valueType() const noexcept { return value; }
};

struct FalseType {
    static constexpr bool value = false;
    using valueType = bool;
    using type = FalseType;

    constexpr explicit operator valueType() const noexcept { return value; }
};


template<typename From, typename To, typename = void>
struct isConvertible : FalseType {};

template<typename From, typename To>
struct isConvertible<From, To, voidType<decltype(static_cast<To>(declVal<From>()))>> : TrueType {};

template<typename From, typename To>
inline constexpr bool isConvertibleValue = isConvertible<From, To>::value;

#endif // TYPETRAITS_H
