#ifndef SORTEDSEQUENCE_H
#define SORTEDSEQUENCE_H
#include "../../sequences/arraySequence.h"

template<typename T>
struct AVLNode {
    T value;
    size_t height;
    UniquePtr<AVLNode> left;
    UniquePtr<AVLNode> right;

    explicit AVLNode(const T& value) : value(value), height(1), left(nullptr), right(nullptr) {}
    explicit AVLNode(T&& value) : value(std::move(value)), height(1), left(nullptr), right(nullptr) {}
};

template<typename T>
class SortedSequence final {
    UniquePtr<AVLNode<T>> root;
    size_t size;

    size_t GetHeight(const UniquePtr<AVLNode<T>>& node) const { return node ? node->height : 0; }

    long long int GetBalanceFactor(const UniquePtr<AVLNode<T>>& node) const {
        return GetHeight(node->left) - GetHeight(node->right);
    }

    void UpdateHeight(UniquePtr<AVLNode<T>>& node) {
        node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));
    }

    UniquePtr<AVLNode<T>> RotateRight(UniquePtr<AVLNode<T>> y) {
        auto x = std::move(y->left);
        y->left = std::move(x->right);
        x->right = std::move(y);
        UpdateHeight(x->right);
        UpdateHeight(x);
        return x;
    }

    UniquePtr<AVLNode<T>> RotateLeft(UniquePtr<AVLNode<T>> x) {
        auto y = std::move(x->right);
        x->right = std::move(y->left);
        y->left = std::move(x);
        UpdateHeight(y->left);
        UpdateHeight(y);
        return y;
    }

    UniquePtr<AVLNode<T>> Balance(UniquePtr<AVLNode<T>> node) {
        UpdateHeight(node);

        if (GetBalanceFactor(node) > 1) {
            if (GetBalanceFactor(node->left) < 0) {
                node->left = RotateLeft(std::move(node->left));
            }
            return RotateRight(std::move(node));
        }

        if (GetBalanceFactor(node) < -1) {
            if (GetBalanceFactor(node->right) > 0) {
                node->right = RotateRight(std::move(node->right));
            }
            return RotateLeft(std::move(node));
        }

        return node;
    }

    UniquePtr<AVLNode<T>> Insert(UniquePtr<AVLNode<T>> node, const T& value) {
        if (!node) {
            ++size;
            return makeUnique<AVLNode<T>>(value);
        }

        if (value < node->value) {
            node->left = Insert(std::move(node->left), value);
        } else if (value >= node->value) {
            node->right = Insert(std::move(node->right), value);
        }

        return Balance(std::move(node));
    }

    void InOrderTraversal(const AVLNode<T>* node, ArraySequence<T>& result) const {
        if (node) {
            InOrderTraversal(node->left.get(), result);
            result.Append(node->value);
            InOrderTraversal(node->right.get(), result);
        }
    }

    UniquePtr<AVLNode<T>> Copy(const AVLNode<T>* node) {
        if (!node) {
            return UniquePtr<AVLNode<T>>(nullptr);
        }
        auto newNode = makeUnique<AVLNode<T>>(node->value);
        newNode->left = Copy(node->left.get());
        newNode->right = Copy(node->right.get());
        newNode->height = node->height;
        return newNode;
    }

public:
    SortedSequence() : root(nullptr), size(0) {}

    SortedSequence(const SortedSequence& other) : root(Copy(other.root.get())), size(other.size) {}

    SortedSequence(SortedSequence&& other) noexcept : root(std::move(other.root)), size(other.size) { other.size = 0; }

    size_t GetSize() const { return size; }

    void Add(const T& value) { root = Insert(std::move(root), value); }

    T Get(size_t index) const {
        ArraySequence<T> elements;
        InOrderTraversal(root.get(), elements);
        return elements[index];
    }

    bool IsEmpty() const { return size == 0; }

    SortedSequence& operator=(const SortedSequence& other) {
        if (this != &other) {
            root = Copy(other.root.get());
            size = other.size;
        }
        return *this;
    }

    SortedSequence& operator=(SortedSequence&& other) noexcept {
        if (this != &other) {
            root = std::move(other.root);
            size = other.size;
            other.size = 0;
        }
        return *this;
    }


    const T& operator[](size_t index) const {
        ArraySequence<T> elements;
        InOrderTraversal(root.get(), elements);
        return elements[index];
    }

    class Iterator {
        ArraySequence<T> elements;
        size_t index;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

    public:
        Iterator(const SortedSequence& seq, const size_t index) : elements(), index(index) {
            seq.InOrderTraversal(seq.root.get(), elements);
        }

        reference operator*() { return elements[index]; }

        Iterator& operator++() {
            ++index;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++index;
            return temp;
        }

        Iterator& operator--() {
            --index;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --index;
            return temp;
        }

        bool operator==(const Iterator& other) const { return index == other.index; }

        bool operator!=(const Iterator& other) const { return index != other.index; }

        bool operator<(const Iterator& other) const { return index < other.index; }

        bool operator>(const Iterator& other) const { return index > other.index; }

        bool operator<=(const Iterator& other) const { return index <= other.index; }

        bool operator>=(const Iterator& other) const { return index >= other.index; }

        Iterator& operator+=(const size_t n) {
            index += n;
            return *this;
        }

        Iterator operator+(const size_t n) const {
            Iterator temp = *this;
            temp.index += n;
            return temp;
        }

        Iterator& operator-=(const size_t n) {
            index -= n;
            return *this;
        }

        Iterator operator-(const size_t n) const {
            Iterator temp = *this;
            temp.index -= n;
            return temp;
        }

        size_t operator-(const Iterator& other) const { return index - other.index; }
    };

    Iterator begin() const { return Iterator(*this, 0); }

    Iterator end() const { return Iterator(*this, size); }

    ~SortedSequence() = default;
};

#endif // SORTEDSEQUENCE_H
