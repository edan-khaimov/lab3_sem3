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

template<typename T>
struct removeCV {
    using type = T;
};

template<typename T>
struct removeCV<const T> {
    using type = T;
};

template<typename T>
using removeCVType = typename removeCV<T>::type;

template<typename From, typename To, typename = void>
struct isConvertible : FalseType {};

template<typename From, typename To>
struct isConvertible<From, To, voidType<decltype(static_cast<To>(declVal<From>()))>> : TrueType {};

template<typename From, typename To>
inline constexpr bool isConvertibleValue = isConvertible<From, To>::value;

template<typename T>
struct isIntegral : FalseType {};

template<>
struct isIntegral<int> : TrueType {};

template<>
struct isIntegral<unsigned int> : TrueType {};

template<>
struct isIntegral<short> : TrueType {};

template<>
struct isIntegral<unsigned short> : TrueType {};

template<>
struct isIntegral<long> : TrueType {};

template<>
struct isIntegral<unsigned long> : TrueType {};

template<>
struct isIntegral<long long> : TrueType {};

template<>
struct isIntegral<unsigned long long> : TrueType {};

template<>
struct isIntegral<char> : TrueType {};

template<>
struct isIntegral<unsigned char> : TrueType {};

template<>
struct isIntegral<signed char> : TrueType {};

template<>
struct isIntegral<wchar_t> : TrueType {};

template<>
struct isIntegral<bool> : TrueType {};

template<typename T>
inline constexpr bool isIntegralValue = isIntegral<removeCVType<T>>::value;

template<typename T>
struct isFloatingPoint : FalseType {};

template<>
struct isFloatingPoint<float> : TrueType {};

template<>
struct isFloatingPoint<double> : TrueType {};

template<>
struct isFloatingPoint<long double> : TrueType {};

template<typename T>
inline constexpr bool isFloatingPointValue = isFloatingPoint<removeCVType<T>>::value;

template<typename T>
struct isPointer : FalseType {};

template<typename T>
struct isPointer<T*> : TrueType {};

template<typename T>
struct isPointer<T* const> : TrueType {};

template<typename T>
inline constexpr bool isPointerValue = isPointer<removeCVType<T>>::value;

template<typename T, typename U>
struct sameAs : FalseType {};

template<typename T>
struct sameAs<T, T> : TrueType {};

template<typename T, typename U>
inline constexpr bool sameAsValue = sameAs<T, U>::value;


#endif // TYPETRAITS_H
