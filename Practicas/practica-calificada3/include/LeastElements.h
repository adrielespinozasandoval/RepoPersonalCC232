#pragma once

#include <limits>
#include <memory>
#include <vector>
#include <cstddef>
#include <ostream>
#include <stdexcept>

#include "Treap.h"
#include "Traits.h"

namespace cc232 {

template <typename Iterator>
struct PrintableIt {
    using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
    using difference_type = typename std::iterator_traits<Iterator>::difference_type;
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    using reference = typename std::iterator_traits<Iterator>::reference;
    using pointer = typename std::iterator_traits<Iterator>::pointer;

    Iterator it;

    PrintableIt() = default;
    PrintableIt(Iterator it): it(it) {}

    auto operator*() const {return *it;};
    auto operator->() const {return it.operator->();}
    operator Iterator() const {return it;}
    
    template <typename It = Iterator, std::enable_if_t<std::is_base_of_v<
        std::random_access_iterator_tag, 
        typename std::iterator_traits<It>::iterator_category>, int> = 0>
    friend difference_type operator-(const PrintableIt& a, const PrintableIt& b) {
        return a.it - b.it;
    }
    template <typename It = Iterator, std::enable_if_t<std::is_base_of_v<
        std::random_access_iterator_tag, 
        typename std::iterator_traits<It>::iterator_category>, int> = 0>
    PrintableIt operator+(difference_type n) const { 
        return PrintableIt(it + n); 
    }
    template <typename It = Iterator, std::enable_if_t<std::is_base_of_v<
        std::random_access_iterator_tag, 
        typename std::iterator_traits<It>::iterator_category>, int> = 0>
    PrintableIt operator-(difference_type n) const { 
        return PrintableIt(it - n); 
    }

    friend std::ostream& operator <<(std::ostream &os, const PrintableIt &pIt) {
        return os << *pIt.it;
    }
};

template <typename T, typename DataContainer, typename Compare = std::less<T>, template <typename ...> class Container = std::vector>
class LeastElements {
    static_assert(traits::is_valid_comp_v<T, Compare>, "Compare must be valid comparator");
    static_assert(traits::is_iterable_v<DataContainer>, "DataContainer must be iterable");

    using DataContainerIt = decltype(std::begin(std::declval<const DataContainer&>()));
    using DCItWrapper = PrintableIt<DataContainerIt>;
    struct TreapComparator {
        Compare comp;
        TreapComparator(const Compare& c = Compare{}) : comp(c) {}
        
        bool operator()(const DCItWrapper &a, const DCItWrapper &b) const {
            return (*a == *b) ? diff(a, b) > 0 : comp(*a, *b);
        }
    };

    const std::size_t m_;
    const std::size_t k_;
    const DataContainerIt end;
    DataContainerIt it;
    const TreapComparator treap_comp;
    Treap<DCItWrapper, TreapComparator> window_;
    Treap<DCItWrapper, TreapComparator> least_;

    template <typename V>
    static inline constexpr std::size_t to_size_t(V value) {return static_cast<std::size_t>(value);}

    template <class Iterator, std::enable_if_t<std::is_base_of_v<
        std::random_access_iterator_tag,
        typename std::iterator_traits<Iterator>::iterator_category>, int> = 0>
    static inline typename std::iterator_traits<Iterator>::difference_type diff(Iterator a, Iterator b) {return b - a;}
    template <class Iterator, std::enable_if_t<!std::is_base_of_v<
        std::bidirectional_iterator_tag,
        typename std::iterator_traits<Iterator>::iterator_category>, int> = 0>
    static inline typename std::iterator_traits<Iterator>::difference_type diff(Iterator a, Iterator b) {return std::distance(a, b);}

    public:
    void nextLeast() {
        if (it == end)
            return;
        
        auto expIt = it - m_;
        if (!least_.remove(expIt)) {
            window_.remove(expIt);
        }
        window_.insert(it);

        if (least_.size() < k_) {
            auto cIt = *window_.minIt();
            window_.remove(cIt);
            least_.insert(cIt);
        }
        if (window_.size() > 0 && least_.size() > 0) {
            auto minIt = *window_.minIt();
            auto maxIt = *least_.maxIt();
            
            if(treap_comp(minIt, maxIt)) {
                window_.remove(minIt);
                least_.remove(maxIt);
                window_.insert(maxIt);
                least_.insert(minIt);
            }
        }
        ++it;
    }
    
    const auto &window() const {
        return window_;
    }
    const auto &least() const {
        return least_;
    }
    
    explicit LeastElements(const DataContainer &data, std::size_t m, std::size_t k, Compare comp = {}):
        m_(m), k_(k), treap_comp(comp), window_(treap_comp), least_(treap_comp), end(std::end(data)), it(std::begin(data) + m) {
        if (m < k || to_size_t(std::distance(std::begin(data), std::end(data))) < m)
            throw std::logic_error("k <= m <= data.size() = n");

        for (auto dIt = std::begin(data); dIt != std::begin(data) + m; ++dIt)
            window_.insert(dIt);
        for (std::size_t j = 0; j < k; ++j) {
            auto minIt = *window_.minIt();
            window_.remove(minIt);
            least_.insert(minIt);
        }
    }
    ~LeastElements() = default;
};

} // namespace cc232