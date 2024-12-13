#ifndef FNV1AHASH_H
#define FNV1AHASH_H
#include <cstdint>
#include <cstring>
#include <string>

#include "Concepts.h"

template<typename T>
struct FNV1a {
    size_t operator()(const T&) const = delete;
};

template<Integer T>
struct FNV1a<T> {
    size_t operator()(const T& key) const {
        size_t hash = 0xcbf29ce484222325UL;
        T tmp = key;
        for (size_t i = 0; i < sizeof(T); ++i) {
            hash ^= tmp & 0xFF;
            hash *= 0x100000001b3UL;
            tmp >>= 8;
        }
        return hash;
    }
};

template<FloatingPoint T>
struct FNV1a<T> {
    size_t operator()(const T& key) const {
        const auto* data = reinterpret_cast<const unsigned char*>(&key);
        size_t hash = 0xcbf29ce484222325UL;

        for (size_t i = 0; i < sizeof(T); ++i) {
            hash ^= static_cast<size_t>(data[i]);
            hash *= 0x100000001b3UL;
        }

        return hash;
    }
};

template<Pointer T>
struct FNV1a<T> {
    size_t operator()(const T& key) const { return FNV1a<std::uintptr_t>{}(reinterpret_cast<std::uintptr_t>(key)); }
};

template<String T>
struct FNV1a<T> {
    size_t operator()(const T& key) const {
        size_t hash = 0xcbf29ce484222325UL;
        const char* data = sameAsValue<T, std::string> ? key.c_str() : key;

        while (*data) {
            hash ^= static_cast<size_t>(*data++);
            hash *= 0x100000001b3UL;
        }
        return hash;
    }
};

#endif // FNV1AHASH_H
