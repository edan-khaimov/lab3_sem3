#ifndef MURMURHASH_H
#define MURMURHASH_H
#include <cstdint>
#include <cstring>
#include <string>
#include "Concepts.h"

template<typename T>
struct MurmurHash {
    size_t operator()(const T&) const = delete;
};

template<Integer T>
struct MurmurHash<T> {
    size_t operator()(const T& key) const {
        size_t hash = 0xc70f6907UL;
        auto k = static_cast<size_t>(key);
        k *= 0xcc9e2d51UL;
        k = k << 15 | k >> 32 - 15;
        k *= 0x1b873593UL;
        hash ^= k;
        hash = hash << 13 | hash >> 32 - 13;
        hash = hash * 5 + 0xe6546b64UL;
        return hash;
    }
};

template<FloatingPoint T>
struct MurmurHash<T> {
    size_t operator()(const T& key) const {
        const auto* data = reinterpret_cast<const unsigned char*>(&key);
        size_t hash = 0xc70f6907UL;

        for (size_t i = 0; i < sizeof(T); ++i) {
            constexpr size_t c1 = 0xcc9e2d51UL;
            constexpr size_t c2 = 0x1b873593UL;
            auto k = static_cast<size_t>(data[i]);
            k *= c1;
            k = k << 15 | k >> 32 - 15;
            k *= c2;
            hash ^= k;
            hash = hash << 13 | hash >> 32 - 13;
            hash = hash * 5 + 0xe6546b64UL;
        }

        hash ^= sizeof(T);
        hash ^= hash >> 16;
        hash *= 0x85ebca6bUL;
        hash ^= hash >> 13;
        hash *= 0xc2b2ae35UL;
        hash ^= hash >> 16;
        return hash;
    }
};

template<Pointer T>
struct MurmurHash<T> {
    size_t operator()(const T& key) { return MurmurHash<std::uintptr_t>{}(reinterpret_cast<std::uintptr_t>(key)); }
};

template<String T>
struct MurmurHash<T> {
    size_t operator()(const T& key) const {
        size_t hash = 0xc70f6907UL;
        const char* data = sameAsValue<T, std::string> ? key.c_str() : key;
        const size_t length = std::strlen(data);

        for (std::size_t i = 0; i < length; ++i) {
            constexpr std::size_t c1 = 0xcc9e2d51UL;
            constexpr std::size_t c2 = 0x1b873593UL;
            auto k = static_cast<size_t>(static_cast<unsigned char>(data[i]));
            k *= c1;
            k = k << 15 | k >> 32 - 15;
            k *= c2;
            hash ^= k;
            hash = hash << 13 | hash >> 32 - 13;
            hash = hash * 5 + 0xe6546b64UL;
        }

        hash ^= length;
        hash ^= hash >> 16;
        hash *= 0x85ebca6bUL;
        hash ^= hash >> 13;
        hash *= 0xc2b2ae35UL;
        hash ^= hash >> 16;
        return hash;
    }
};

#endif // MURMURHASH_H
