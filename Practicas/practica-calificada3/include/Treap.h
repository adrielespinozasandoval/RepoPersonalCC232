#pragma once

#include <random>
#include <string>
#include <cstddef>
#include <sstream>
#include <ostream>
#include <functional>
#include <type_traits>

#include "Traits.h"

namespace cc232 {

struct Treap_traits {
    struct def_PriorityGen {
        std::mt19937_64 rng;
        std::uint64_t prio_counter = 0;

        def_PriorityGen(): rng(std::random_device{}()) {};
        explicit def_PriorityGen(std::uint64_t seed): rng(seed) {};
        ~def_PriorityGen() = default;

        std::uint64_t operator()() {
            return (rng() << 16) ^ ++prio_counter;
        }
    };
};

template <typename T, typename Compare = std::less<T>, typename PriorityGen = Treap_traits::def_PriorityGen>
class Treap {
    static_assert(traits::is_valid_comp_v<T, Compare>, "Compare must be valid comparator");
    static_assert(traits::is_generator_v<PriorityGen>, "PriorityGen must be generator");

    using gen_t = decltype(std::declval<PriorityGen&>()());
    static_assert(traits::is_less_comparable_v<gen_t>, "Generator type must be comparable with < operator");

    public: struct Node; private:

    Node *root_ = nullptr;
    Compare comp_ = {};
    PriorityGen prg_ = {};
    std::size_t size_ = 0;

    bool addNode(Node *node) {
        node->left = nullptr;
        node->right = nullptr;
        Node *parent = findM(node->value);
        if (!parent) {
            root_ = node;
            node->parent = nullptr;
            ++size_;
            return true;
        }
        if (comp_(node->value, parent->value)) {
            if (parent->left)
                return false;
            parent->left = node;
        } else if (comp_(parent->value, node->value)) {
            if (parent->right)
                return false;
            parent->right = node;
        } else
            return false;
        node->parent = parent;
        ++size_;
        percolateUp(node);
        return true;
    }
    static void destroyNode(Node *node) {
        if (!node)
            return;
        destroyNode(node->left);
        destroyNode(node->right);
        delete node;
    }

    void rotateLeft(Node *node) {
        if(!node || !node->right)
            return;
        Node *right = node->right;
        right->parent = node->parent;
        if (!node->parent)
            root_ = right;
        else if (node->isLeftChild())
            node->parent->left = right;
        else
            node->parent->right = right;
        node->right = right->left;
        if (right->left)
            right->left->parent = node;
        node->parent = right, right->left = node;
    }
    void rotateRight(Node *node) {
        if(!node || !node->left)
            return;
        Node *left = node->left;
        left->parent = node->parent;
        if (!node->parent)
            root_ = left;
        else if (node->isLeftChild())
            node->parent->left = left;
        else
            node->parent->right = left;
        node->left = left->right;
        if (left->right)
            left->right->parent = node;
        node->parent = left, left->right = node;
    }
    void percolateUp(Node *node) {
        while (node->parent && node->priority < node->parent->priority) {
            if (node->isLeftChild())
                rotateRight(node->parent);
            else
                rotateLeft(node->parent);
        }
        if (!node->parent)
            root_ = node;
    }
    void percolateDown(Node *node) {
        while (node->left || node->right) {
            if (!node->left)
                rotateLeft(node);
            else if (!node->right)
                rotateRight(node);
            else if (node->left->priority < node->right->priority)
                rotateRight(node);
            else
                rotateLeft(node);
            if (root_ == node)
                root_ = node->parent;
        }
    }

    Node *findM(const T &value) const {
        Node *curr = root_, *prev = nullptr;
        while (curr) {
            prev = curr;
            if (comp_(curr->value, value))
                curr = curr->right;
            else if (comp_(value, curr->value))
                curr = curr->left;
            else
                return curr;
        }
        return prev;
    }
    Node *findEqM(const T &value) const {
        Node *curr = root_;
        while (curr) {
            if (comp_(curr->value, value))
                curr = curr->right;
            else if (comp_(value, curr->value))
                curr = curr->left;
            else
                return curr;
        }
        return nullptr;
    }
    Node *minNode(Node *node) const {
        while (node && node->left)
            node = node->left;
        return node;
    }
    Node *maxNode(Node *node) const {
        while (node && node->right)
            node = node->right;
        return node;
    }
    Node *nextM(Node *node) const {
        if (!node)
            return nullptr;
        if (node->right)
            return minNode(node->right);
        Node *curr = node, *prev = node->parent;
        while (prev && prev->right == curr)
            curr = prev, prev = prev->parent;
        return prev;
    }
    Node *prevM(Node *node) const {
        if (!node)
            return nullptr;
        if (node->left)
            return maxNode(node->left);
        Node *curr = node, *prev = node->parent;
        while (prev && prev->left == curr)
            curr = prev, prev = prev->parent;
        return prev;
    }
    
    bool isBST(Node *node, const T *min, const T *max) const {
        if (!node)
            return true;
        if ((min && !comp_(*min, node->value)) || (max && !comp_(node->value, *max)))
            return false;
        return isBST(node->left, min, &node->value) && isBST(node->right, &node->value, max);
    }
    static bool checkParents(Node *node, Node *parent) {
        if (!node)
            return true;
        if (node->parent != parent)
            return false;
        return checkParents(node->left, node) && checkParents(node->right, node);
    }
    static bool isHeap(Node *node) {
        if (!node)
            return true;
        if (node->left && node->left->priority < node->priority || node->right && node->right->priority < node->priority)
            return false;
        return isHeap(node->left) && isHeap(node->right);
    }

    void buildStr(Node *node, const std::string &prefix, bool isTail, std::stringstream &out) const {
        if (!node)
            return;
        out << prefix << (isTail ? "└── " : "├── ") << "(" << node->value << "| p:" << node->priority << ")" << std::endl;
        std::string childPrefix = prefix + (isTail ? "    " : "│   ");

        if (node->left)
            buildStr(node->left, childPrefix, node->right == nullptr, out);
        if (node->right)
            buildStr(node->right, childPrefix, true, out);
    }

    public:

    struct Node {
        T value;
        gen_t priority;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *parent = nullptr;

        inline bool isLeftChild() const {return parent && parent->left == this;}
        inline bool isRightChild() const {return parent && parent->right == this;}

        private:
        friend class Treap;

        Node() = default;
        Node(const T &v, gen_t pr, Node *p = nullptr): value(v), priority(pr), parent(p) {}
    };
    struct const_iterator {
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using reference = const T&;
        using pointer = const T*;

        reference operator*() const {return curr->value;}
        pointer operator->() const {return &(curr->value);}
        const_iterator &operator++() {
            curr = treap->nextM(curr);
            return *this;
        }
        const_iterator &operator--() {
            curr = treap->prevM(curr);
            return *this;
        }
        bool operator ==(const const_iterator& other) const {return curr == other.curr;}
        bool operator !=(const const_iterator& other) const {return curr != other.curr;}

        private:
        friend class Treap;

        Node *curr;
        const Treap *treap;

        const_iterator(Node *curr, const Treap *treap): curr(curr), treap(treap) {}
    };
    using iterator = const_iterator;

    bool insert(const T &value, gen_t priority) {
        Node *node = new Node(value, priority);
        if (!addNode(node)) {
            delete node;
            return false;
        }
        return true;
    }
    bool insert(const T &value) {
        return insert(value, prg_());
    }
    bool remove(const T &value) {
        Node *node = findEqM(value);
        if (!node)
            return false;
        percolateDown(node);
        if (node == root_)
            root_ = nullptr;
        else if (node->isLeftChild())
            node->parent->left = nullptr;
        else
            node->parent->right = nullptr;
        delete node;
        --size_;
        return true;
    }

    void clear() {
        destroyNode(root_);
        root_ = nullptr;
        size_ = 0;
    }
    void swap(Treap &other) noexcept {
        std::swap(root_, other.root_);
        std::swap(size_, other.size_);
        std::swap(comp_, other.comp_);
        std::swap(prg_, other.prg_);
    }

    inline const Node *find(const T &value) const {
        return findM(value);
    }
    inline const Node *findEq(const T &value) const {
        return findEqM(value);
    }
    static inline iterator minIt(iterator &it) {
        return iterator(it.treap->minNode(it.curr), it.treap);
    }
    inline iterator minIt() const {
        return iterator(minNode(root_), this);
    }
    static inline iterator maxIt(iterator &it) {
        return iterator(it.treap->maxNode(it.curr), it.treap);
    }
    inline iterator maxIt() const {
        return iterator(maxNode(root_), this);
    }
    inline bool isBST() const {
        return isBST(root_, nullptr, nullptr) && checkParents(root_, nullptr);
    }
    inline bool isHeap() const {
        return isHeap(root_);
    }
    inline bool isTreap() const {
        return isBST() && isHeap();
    }

    inline const Node *root() const noexcept {
        return root_;
    }
    inline std::size_t size() const noexcept {
        return size_;
    }
    inline bool empty() const noexcept {
        return size_ == 0;
    }
    inline iterator begin() const {
        return cbegin();
    }
    inline iterator end() const {
        return cend();
    }
    inline iterator cbegin() const {
        return minIt();
    }
    inline iterator cend() const {
        return iterator(nullptr, this);
    }

    Treap() {}
    explicit Treap(Compare comp): comp_(std::move(comp)) {}
    Treap(Treap&& other) noexcept {
        swap(other);
    }
    Treap &operator =(Treap&& other) noexcept {
        if (this != &other) {
            clear();
            swap(other);
        }
        return *this;
    }
    ~Treap() {
        clear();
    }

    std::string toString() const {
        if (!root_)
            return "(empty)";

        std::stringstream out;
        std::string prefix = "";
        out << "[" << root_->value << "| p:" << root_->priority << "]" << std::endl;
        if (root_->left)
            buildStr(root_->left, prefix, root_->right == nullptr, out);
        if (root_->right)
            buildStr(root_->right, prefix, true, out);
        return out.str();
    }
    friend std::ostream& operator <<(std::ostream &os, const Treap &treap) {
        os << treap.toString();
        return os;
    }
};

} // namespace cc232