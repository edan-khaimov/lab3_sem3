#ifndef CONCEPTS_H
#define CONCEPTS_H
#include "TypeTraits.h"

template<typename From, typename To>
concept ConvertibleTo = requires(From from) { static_cast<To>(from); } && isConvertibleValue<From, To>;

template<typename T>
concept Hashable = requires(T a) {
    { std::hash<T>{}(a) } -> ConvertibleTo<size_t>;
};

template<typename T>
concept EqualityComparable = requires(T a, T b) {
    { a == b } -> ConvertibleTo<bool>;
    { b == a } -> ConvertibleTo<bool>;
};

#endif // CONCEPTS_H
