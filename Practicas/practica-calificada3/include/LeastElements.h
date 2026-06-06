#pragma once

#include <limits>
#include <memory>
#include <vector>
#include <cstddef>
#include <ostream>
#include <stdexcept>

#include <iostream>
#include <string>

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

template <typename T, typename DataContainer, typename Compare = std::greater<T>, template <typename ...> class Container = std::vector>
class LeastElements {
    static_assert(traits::is_valid_comp_v<T, Compare>, "Compare must be valid comparator");
    static_assert(traits::is_iterable_v<DataContainer>, "DataContainer must be iterable");

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
    explicit LeastElements(const DataContainer& data, std::size_t m, std::size_t k, const Compare &comp = {}) {
        if (m < k || to_size_t(std::distance(std::begin(data), std::end(data))) < m)
            throw std::logic_error("k <= m <= data.size() = n");

        using DataContainerIt = decltype(std::begin(data));
        using DCItWrapper = PrintableIt<DataContainerIt>;

        auto treap_comp = [&comp](const DCItWrapper &a, const DCItWrapper &b) {
            if (*a == *b)
                return diff(a, b) > 0;
            return comp(*a, *b);
        };
        Treap<DCItWrapper, decltype(treap_comp)> window(treap_comp);
        Treap<DCItWrapper, decltype(treap_comp)> least(treap_comp);

        for (auto it = std::begin(data); it != std::begin(data) + m; ++it) {
            window.insert(it);
        }
        for (std::size_t j = 0; j < k; ++j) {
            auto minIt = window.minNode()->value;
            window.remove(minIt);
            least.insert(minIt);
        }
    
        for (auto it = std::begin(data) + m; it != std::end(data); ++it) {
            auto expIt = it - m;
            if (!least.remove(expIt)) {
                window.remove(expIt);
            }
            window.insert(it);

            if (least.size() < k) {
                auto cIt = window.minNode()->value;
                window.remove(cIt);
                least.insert(cIt);
            }
            if (window.size() > 0 && least.size() > 0) {
                auto minIt = window.minNode()->value;
                auto maxIt = least.maxNode()->value;
                
                if(treap_comp(minIt, maxIt)) {
                    window.remove(minIt);
                    least.remove(maxIt);
                    window.insert(maxIt);
                    least.insert(minIt);
                }
            }

            std::cout << "Window: " << std::endl << window << std::endl;
            std::cout << "Least: " << std::endl << least << std::endl;
        }
    }
};

} // namespace cc232