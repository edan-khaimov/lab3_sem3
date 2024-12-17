#ifndef CONCEPTS_H
#define CONCEPTS_H
#include "TypeTraits.h"

template<typename From, typename To>
concept ConvertibleTo = requires(From from) { static_cast<To>(from); } && isConvertibleValue<From, To>;

template<typename T, typename Hash>
concept Hashable = requires(T a, Hash hash) {
    { hash(a) } -> ConvertibleTo<size_t>;
};

template<typename T>
concept EqualityComparable = requires(T a, T b) {
    { a == b } -> ConvertibleTo<bool>;
    { b == a } -> ConvertibleTo<bool>;
    { a != b } -> ConvertibleTo<bool>;
    { b != a } -> ConvertibleTo<bool>;
};

template<typename T>
concept TotallyOrdered = EqualityComparable<T> && requires(T a, T b) {
    { a < b } -> ConvertibleTo<bool>;
    { b < a } -> ConvertibleTo<bool>;
    { a <= b } -> ConvertibleTo<bool>;
    { b <= a } -> ConvertibleTo<bool>;
    { a >= b } -> ConvertibleTo<bool>;
    { b >= a } -> ConvertibleTo<bool>;
    { a > b } -> ConvertibleTo<bool>;
    { b > a } -> ConvertibleTo<bool>;
};

template<typename T>
concept Integer = isIntegralValue<T>;

template<typename T>
concept FloatingPoint = isFloatingPointValue<T>;

template<typename T>
concept Pointer = isPointerValue<T>;

template<typename T>
concept String = (sameAsValue<T, std::string> || sameAsValue<T, const char*>);

template<typename T>
concept Numerical = Integer<T> || FloatingPoint<T>;

#endif // CONCEPTS_H
