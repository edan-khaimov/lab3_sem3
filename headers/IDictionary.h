#ifndef IDICTIONARY_H
#define IDICTIONARY_H
#include "../../sequences/arraySequence.h"
#include "Concepts.h"

template<typename TKey, typename TElement, typename Hasher = std::hash<TKey>>
    requires Hashable<TKey, Hasher> && EqualityComparable<TKey>
class IDictionary {
    struct Slot {
        TKey key;
        TElement element;
        size_t distance;
        bool occupied;

        Slot(const TKey& key, const TElement& element, const size_t distance, const bool occupied) :
            key(key), element(element), distance(distance), occupied(occupied) {}
        Slot(TKey&& key, TElement&& element, const size_t distance, const bool occupied) :
            key(std::move(key)), element(std::move(element)), distance(distance), occupied(occupied) {}
    };

    ArraySequence<Slot> table;
    size_t size;
    size_t capacity;
    float maxLoadFactor;

    size_t Hash(const TKey& key) const { return Hasher{}(key) % capacity; }

    void Rehash() {
        ArraySequence<Slot> oldTable = table; // move instead copy
        capacity *= 2;
        table = ArraySequence<Slot>(capacity);
        size = 0;

        /*
         Insert(move(old.key), move(old.value))
         */
    }

public:
    explicit IDictionary(const size_t capacity = 16, const float maxLoadFactor = 0.9) :
        size(0), capacity(capacity), maxLoadFactor(maxLoadFactor) {}

    class Iterator {
        typename ArraySequence<Slot>::Iterator current;
        typename ArraySequence<Slot>::Iterator end;

        void skipEmpty() {
            while (current != end && !current->occupied) {
                ++current;
            }
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<TKey, TElement>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(typename ArraySequence<Slot>::Iterator start, typename ArraySequence<Slot>::Iterator end) :
        current(start), end(end) {
            skipEmpty();
        }

        value_type operator*() const {
            return {current->key, current->element};
        }

        Iterator& operator++() {
            ++current;
            skipEmpty();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin() {
        return Iterator(table.begin(), table.end());
    }

    Iterator end() {
        return Iterator(table.end(), table.end());
    }
};
#endif // IDICTIONARY_H
