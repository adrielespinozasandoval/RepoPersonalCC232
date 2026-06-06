#pragma once

#include <vector>
#include <cstddef>
#include <utility>
#include <iterator>

#include "Traits.h"

namespace cc232 {

struct Heap_traits {
    template <class Container>
    struct is_heap_compatible: std::integral_constant<bool,
        traits::is_stack_like_v<Container> && traits::has_random_access_iterator_v<Container>> {};
    template <class Container>
    static constexpr bool is_heap_compatible_v = is_heap_compatible<Container>::value;
};

template <typename T, typename Compare, template <typename ...> class Container = std::vector>
class Heap {
    static_assert(traits::is_valid_comp_v<T, Compare>, "Compare must be valid comparator");
    static_assert(traits::is_stack_like_v<Container<T>>, "Container must be stack-like");
    static_assert(traits::has_random_access_iterator_v<Container<T>>, "Container must have random access iterator");

    using iterator = decltype(std::begin(std::declval<Container<T>&>()));
    using const_iterator = decltype(std::cbegin(std::declval<const Container<T>&>()));

    Container<T> data_;
    std::size_t size_;
    Compare comp_ = {};

    static inline constexpr iterator parent(Container<T> &data, iterator it) {return std::begin(data) + ((it - std::begin(data)) - 1) / 2;}
    static inline constexpr iterator leftChild(Container<T> &data, iterator it) {return std::begin(data) + (it - std::begin(data)) * 2 + 1;}
    static inline constexpr iterator rightChild(Container<T> &data, iterator it) {return std::begin(data) + (it - std::begin(data)) * 2 + 2;}
    static inline constexpr bool hasParent(Container<T> &data, iterator it) {return (it - std::begin(data)) > 0;}
    static inline constexpr bool hasLeftChild(Container<T> &data, iterator it) {
        return static_cast<std::size_t>(it - std::begin(data)) * 2 + 1 < data.size();
    }
    static inline constexpr bool hasRightChild(Container<T> &data, iterator it) {
        return static_cast<std::size_t>(it - std::begin(data)) * 2 + 2 < data.size();
    }

    static void percolateDown(Container<T> &data, iterator it, const Compare &comp) {
        while (hasLeftChild(data, it)) {
            iterator child = leftChild(data, it), right_child = rightChild(data, it);
            if (hasRightChild(data, it) && comp(*child, *right_child))
                child = right_child;
            if (!comp(*it, *child))
                break;
            std::iter_swap(it, child);
            it = child;
        }
    }
    inline void percolateDown(iterator it, const Compare &comp) {
        percolateDown(data_, it, comp);
    }
    inline void percolateDown(iterator it) {
        percolateDown(it, comp_);
    }
    void percolateUp(iterator it, const Compare &comp) {
        while (hasParent(data_, it)) {
            iterator parent_ = parent(data_, it);
            if (!comp(*parent_, *it))
                break;
            std::iter_swap(it, parent_);
            it = parent_;
        }
    }
    inline void percolateUp(iterator it) {
        percolateUp(it, comp_);
    }

    inline iterator lastLeafMit() {
        return std::begin(data_) + size_ - 1;
    }

    public:

    void insert(const T &value) {
        data_.push_back(value);
        ++size_;
        percolateUp(lastLeafMit());
    }
    template <typename It>
    void remove(It it) {
        static_assert(std::is_same_v<It, iterator> || std::is_same_v<It, const_iterator>, "Invalid iterator");

        if constexpr (std::is_same_v<It, iterator>) {
            std::iter_swap(it, lastLeafMit());
            data_.pop_back();
            --size_;
            if (it == std::end(data_))
                return;
            if (hasParent(data_, it) && comp_(*it, *parent(data_, it)))
                percolateUp(it);
            else
                percolateDown(it);
        } else
            remove(std::begin(data_) + (it - std::begin(data_)));
    }
    T removePeak() {
        iterator begin = std::begin(data_);
        T value = *begin;
        remove(begin);
        return value;
    }
    inline void clear() {
        data_.clear();
    }

    inline const Container<T>& data() const {
        return data_;
    }
    inline const_iterator peakIt() const {
        return std::begin(data_);
    }
    inline const T& peak() const {
        return *peakIt();
    }
    inline const_iterator lastLeafIt() const {
        return std::begin(data_) + size_ - 1;
    }
    inline const T& lastLeaf() const {
        return *lastLeafIt();
    }
    inline bool empty() const {
        return size_ == 0;
    }
    inline std::size_t size() const {
        return size_;
    }

    static void heapify(Container<T> &data, const Compare &comp = {}) {
        iterator begin = std::begin(data);
        auto rend = std::rend(data);
        std::size_t size_ = std::end(data) - begin;
        if (size_ < 2)
            return;
        for (auto rit = std::make_reverse_iterator(begin + size_ / 2); rit != rend; ++rit)
            percolateDown(data, std::prev(rit.base()), comp);
    }
    void heapify() {
        heapify(data_, comp_);
    }

    Heap() = default;
    explicit Heap(Container<T>&& data, const Compare &comp = {}): data_(std::move(data)), size_(data_.size()), comp_(comp) {
        heapify();
    }
    template <typename DataContainer>
    explicit Heap(const DataContainer &data, const Compare &comp = {}): size_(std::end(data) - std::begin(data)), comp_(comp) {
        static_assert(traits::is_iterable_v<DataContainer>, "DataContainer must be iterable");

        data_.clear();
        for (iterator it = std::begin(data); it != std::end(data); ++it)
            data_.push_back(*it);
        heapify();
    }
    template <typename DataIterator>
    explicit Heap(const DataIterator &begin, const DataIterator &end, const Compare &comp = {}): size_(end - begin), comp_(comp) {
        static_assert(traits::is_iterator_v<DataIterator>, "DataIterator must be random access iterator");

        data_.clear();
        for (iterator it = begin; it != end; ++it)
            data_.push_back(*it);
        heapify();
    }
    ~Heap() = default;
};

} // namespace cc232