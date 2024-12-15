#ifndef IDICTIONARY_H
#define IDICTIONARY_H
#include "../../sequences/arraySequence.h"
#include "Concepts.h"

template<typename TKey, typename TElement, typename Hasher = std::hash<TKey>>
    requires Hashable<TKey, Hasher> && EqualityComparable<TKey>
class IDictionary final {
    struct Slot {
        const TKey key;
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
        ArraySequence<Slot> oldTable = std::move(table);
        capacity *= 2;
        table = ArraySequence<Slot>(capacity);
        size = 0;

        for (auto& slot: oldTable) {
            if (slot.occupied) {
                Insert(slot.key, slot.element);
            }
        }
    }

public:
    explicit IDictionary(const size_t capacity = 16, const float maxLoadFactor = 0.9) :
        size(0), capacity(capacity), maxLoadFactor(maxLoadFactor) {}

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
        size_t distance = 0;

        Slot newSlot = {key, element, 0, true};

        while (true) {
            Slot& currentSlot = table[index];

            if (!currentSlot.occupied) {
                table[index] = std::move(newSlot);
                ++size;
                return;
            }

            if (currentSlot.key == key) {
                currentSlot.element = element;
                return;
            }

            if (currentSlot.distance < distance) {
                std::swap(newSlot, currentSlot);
            }

            ++distance;
            index = (index + 1) % capacity;
        }
    };

    TElement& Get(const TKey& key) {
        size_t index = Hash(key);
        size_t distance = 0;

        while (true) {
            Slot& currentEntry = table[index];

            if (!currentEntry.occupied || distance > currentEntry.distance) {
                throw std::runtime_error("Key not found");
            }

            if (currentEntry.key == key) {
                return currentEntry.element;
            }

            ++distance;
            index = (index + 1) % capacity;
        }
    };

    void Remove(const TKey& key) {
        size_t index = hash(key);
        size_t distance = 0;

        while (true) {
            Slot& currentSlot = table[index];

            if (!currentSlot.occupied || distance > currentSlot.distance) {
                throw std::runtime_error("Key not found");
            }

            if (currentSlot.key == key) {
                currentSlot.occupied = false;
                --size;
                break;
            }

            ++distance;
            index = (index + 1) % capacity;
        }

        size_t nextIndex = (index + 1) % capacity;
        while (table[nextIndex].occupied && table[nextIndex].distance > 0) {
            table[index] = std::move(table[nextIndex]);
            --table[index].distance;
            table[nextIndex].occupied = false;

            index = nextIndex;
            nextIndex = (index + 1) % capacity;
        }
    }

    bool Contains(const TKey& key) const {
        try {
            Get(key);
            return true;
        } catch (const std::runtime_error&) {
            return false;
        }
    }

    size_t GetCount() const { return size; }

    size_t GetCapacity() const { return capacity; }


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

        value_type operator*() const { return {current->key, current->element}; }

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
        if (this != other) {
            size = other.size;
            capacity = other.capacity;
            maxLoadFactor = other.maxLoadFactor;
            table = other.table;
        }
        return *this;
    }

    IDictionary& operator=(IDictionary&& other) noexcept {
        if (this != other) {
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

    ~IDictionary() = default;
};
#endif // IDICTIONARY_H
