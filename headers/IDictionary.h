#ifndef IDICTIONARY_H
#define IDICTIONARY_H
#include "../../sequences/arraySequence.h"
#include "Concepts.h"

template<typename TKey, typename TElement, typename Hasher = std::hash<TKey>>
    requires Hashable<TKey, Hasher> && EqualityComparable<TKey>
class IDictionary final {
    struct Entry {
        TKey key;
        TElement element;
        size_t distance = 0;
        bool occupied = false;

        Entry() = default;

        Entry(const TKey& key, const TElement& element, const size_t distance, const bool occupied) :
            key(key), element(element), distance(distance), occupied(occupied) {}

        Entry(TKey&& key, TElement&& element, const size_t distance, const bool occupied) :
            key(std::move(key)), element(std::move(element)), distance(distance), occupied(occupied) {}
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
                Insert(slot.key, slot.element);
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

    void Insert(const TKey& key, const TElement& element) {
        if (static_cast<float>(size) / capacity > maxLoadFactor) {
            Rehash();
        }

        size_t index = Hash(key);

        Entry newEntry = {key, element, 0, true};

        while (table[index].occupied) {
            if (newEntry.key == table[index].key) {
                table[index].element = newEntry.element;
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

    TElement Get(const TKey& key) const {
        size_t index = Hash(key);
        size_t distance = 0;

        while (table[index].occupied) {
            if (table[index].key == key) {
                return table[index].element;
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
            if (table[index].key == key) {
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
            if (table[index].key == key) {
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
        using value_type = Entry;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(InnerIterator start, InnerIterator end) :
            current(start), end(end) {
            skipEmpty();
        }

        reference operator*() const { return *current; }

        pointer operator->() const { return &*current; }

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

    TElement& operator[](const TKey& key) {
        size_t index = Hash(key);
        size_t distance = 0;

        while (table[index].occupied) {
            if (table[index].key == key) {
                return table[index].element;
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
