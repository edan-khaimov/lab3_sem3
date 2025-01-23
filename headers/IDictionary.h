#ifndef IDICTIONARY_H
#define IDICTIONARY_H
#include "../../sequences/arraySequence.h"
#include "Concepts.h"

#include <utility>

template<typename TKey, typename TValue, typename Hasher = std::hash<TKey>>
    requires Hashable<TKey, Hasher> && EqualityComparable<TKey>
class IDictionary final {
    struct Entry {
        std::pair<TKey, TValue> keyValue;
        size_t distance = 0;
        bool occupied = false;

        Entry() = default;

        Entry(const std::pair<TKey, TValue>& keyValue, const size_t distance, const bool occupied) :
            keyValue(keyValue), distance(distance), occupied(occupied) {}

        Entry(std::pair<TKey, TValue>&& keyValue, const size_t distance, const bool occupied) :
            keyValue(std::move(keyValue)), distance(distance), occupied(occupied) {}
    };

    ArraySequence<Entry> table;
    size_t size;
    size_t capacity;
    float maxLoadFactor;

    size_t Hash(const TKey& key) const { return Hasher{}(key) % capacity; }

    void Rehash() {
        ArraySequence<Entry> oldTable = std::move(table);
        capacity *= 2;
        table = ArraySequence<Entry>(capacity);
        size = 0;

        for (auto& slot: oldTable) {
            if (slot.occupied) {
                Insert(slot.keyValue.first, slot.keyValue.second);
            }
        }
    }

public:
    explicit IDictionary(const size_t capacity = 16, const float maxLoadFactor = 0.9) :
        table(ArraySequence<Entry>(capacity)), size(0), capacity(capacity), maxLoadFactor(maxLoadFactor) {}

    IDictionary(const IDictionary& other) :
        table(other.table), size(other.size), capacity(other.capacity), maxLoadFactor(other.maxLoadFactor) {}

    IDictionary(IDictionary&& other) noexcept :
        table(std::move(other.table)), size(other.size), capacity(other.capacity), maxLoadFactor(other.maxLoadFactor) {
        other.size = 0;
        other.capacity = 0;
    }

    void Insert(const TKey& key, const TValue& value) {
        if (static_cast<float>(size) / capacity > maxLoadFactor) {
            Rehash();
        }

        size_t index = Hash(key);

        Entry newEntry = {{key, value}, 0, true};

        while (table[index].occupied) {
            if (newEntry.keyValue.first == table[index].keyValue.first) {
                table[index].keyValue.second = newEntry.keyValue.second;
                return;
            }

            if (table[index].distance < newEntry.distance) {
                std::swap(newEntry, table[index]);
            }

            ++newEntry.distance;
            index = (index + 1) % capacity;
        }

        table[index] = std::move(newEntry);
        ++size;
    }

    TValue Get(const TKey& key) const {
        size_t index = Hash(key);
        size_t distance = 0;

        while (table[index].occupied) {
            if (table[index].keyValue.first == key) {
                return table[index].keyValue.second;
            }

            if (distance > table[index].distance) {
                throw std::runtime_error("Key not found");
            }

            ++distance;
            index = (index + 1) % capacity;
        }
        throw std::runtime_error("Key not found");
    }

    void Remove(const TKey& key) {
        size_t index = Hash(key);
        size_t distance = 0;

        while (table[index].occupied) {
            if (table[index].keyValue.first == key) {
                table[index].occupied = false;
                size_t nextIndex = (index + 1) % capacity;
                while (table[nextIndex].occupied && table[nextIndex].distance > 0) {
                    std::swap(table[index], table[nextIndex]);
                    --table[index].distance;
                    ++nextIndex;
                }
                --size;
                return;
            }

            if (distance > table[index].distance) {
                throw std::runtime_error("Key not found");
            }

            ++distance;
            index = (index + 1) % capacity;
        }

        throw std::runtime_error("Key not found");
    }

    bool Contains(const TKey& key) const {
        size_t index = Hash(key);
        size_t distance = 0;

        while (table[index].occupied) {
            if (table[index].keyValue.first == key) {
                return true;
            }

            if (distance > table[index].distance) {
                return false;
            }

            ++distance;
            index = (index + 1) % capacity;
        }
        return false;
    }

    size_t GetCount() const { return size; }

    size_t GetCapacity() const { return capacity; }

    class Iterator {
        using InnerIterator = typename ArraySequence<Entry>::Iterator;
        InnerIterator current;
        InnerIterator end;

        void skipEmpty() {
            while (current != end && !current->occupied) {
                ++current;
            }
        }

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<TKey, TValue>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(InnerIterator start, InnerIterator end) :
            current(start), end(end) {
            skipEmpty();
        }

        reference operator*() const { return current->keyValue; }

        pointer operator->() const { return &current->keyValue; }

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

        bool operator==(const Iterator& other) const { return current == other.current; }

        bool operator!=(const Iterator& other) const { return current != other.current; }
    };


    Iterator begin() { return Iterator(table.begin(), table.end()); }

    Iterator end() { return Iterator(table.end(), table.end()); }

    IDictionary& operator=(const IDictionary& other) {
        if (this != &other) {
            size = other.size;
            capacity = other.capacity;
            maxLoadFactor = other.maxLoadFactor;
            table = other.table;
        }
        return *this;
    }

    IDictionary& operator=(IDictionary&& other) noexcept {
        if (this != &other) {
            size = other.size;
            capacity = other.capacity;
            maxLoadFactor = other.maxLoadFactor;
            table = std::move(other.table);
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }

    bool operator==(const IDictionary& other) const {
        return size == other.size && capacity == other.capacity && maxLoadFactor == other.maxLoadFactor &&
               table == other.table;
    }

    bool operator!=(const IDictionary& other) const {
        return size != other.size || capacity != other.capacity || maxLoadFactor != other.maxLoadFactor ||
               table != other.table;
    }

    TValue& operator[](const TKey& key) {
        size_t index = Hash(key);
        size_t distance = 0;

        while (table[index].occupied) {
            if (table[index].keyValue.first == key) {
                return table[index].keyValue.second;
            }

            if (distance > table[index].distance) {
                throw std::runtime_error("Key not found");
            }

            ++distance;
            index = (index + 1) % capacity;
        }
        throw std::runtime_error("Key not found");
    }

    ~IDictionary() = default;
};
#endif // IDICTIONARY_H
