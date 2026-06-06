#pragma once

#include <limits>
#include <memory>
#include <vector>
#include <cstddef>
#include <stdexcept>

#include <iostream>
#include <string>

#include "Heap.h"
#include "Traits.h"

namespace cc232 {

// Temp functions
template <typename Container>
void print(const Container &data, std::string&& desc) {
    std::cout << desc << ": ";
    for (auto it = std::begin(data); it != std::end(data); ++it)
        std::cout << **it << ", ";
    std::cout << std::endl;
}

template <typename T, typename DataContainer, typename Compare = std::greater<T>, template <typename ...> class Container = std::vector>
class LeastElements {
    static_assert(traits::is_valid_comp_v<T, Compare>, "Compare must be valid comparator");
    static_assert(Heap_traits::is_heap_compatible_v<Container<T>>, "Container must be heap-compatible");
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

    template <typename Iterator, typename Compare_, template <typename ...> class Container_>
    void removeLastDead(Heap<Iterator, Compare_, Container_> &heap, const std::size_t *window_min_pos) {
        auto it = heap.lastLeafIt(), peakIt = heap.peakIt();
        while (to_size_t(diff(peakIt, it)) < *window_min_pos) {
            heap.remove(it);
            it = heap.lastLeafIt();
        }
    }

    public:
    explicit LeastElements(const DataContainer& data, std::size_t m, std::size_t k, const Compare &comp = {}) {
        if (m < k || to_size_t(std::distance(std::begin(data), std::end(data))) < m)
            throw std::logic_error("Message");

        using DataContainerIt = decltype(std::begin(data));
        
        const DataContainerIt begin = std::begin(data);
        std::size_t s = 0;
        std::size_t* window_min_pos = &s;
        Container<DataContainerIt> its;
        for (auto it = std::begin(data); it != std::end(data) + m; ++it) {
            its.push_back(it);
        }
        auto comp_dead = [&begin, &comp, &window_min_pos](const DataContainerIt &a, const DataContainerIt &b){
            if (to_size_t(diff(a, begin)) < *window_min_pos) {
                if (to_size_t(diff(b, begin)) >= *window_min_pos) {
                    return true;
                }
            } else if (to_size_t(diff(b, begin)) < *window_min_pos) {
                return false;
            }
            if (*a == *b)
                return diff(a, b) > 0;
            return comp(*a, *b);
        };
        auto inv_comp_dead = [&begin, &comp, &window_min_pos](const DataContainerIt &a, const DataContainerIt &b){
            if (to_size_t(diff(a, begin)) < *window_min_pos) {
                if (to_size_t(diff(b, begin)) >= *window_min_pos) {
                    return true;
                }
            } else if (to_size_t(diff(b, begin)) < *window_min_pos) {
                return false;
            }
            if (*a == *b)
                return diff(a, b) > 0;
            return comp(*b, *a);
        };

        Heap<DataContainerIt, decltype(inv_comp_dead), Container> window(std::move(its), inv_comp_dead);
        Heap<DataContainerIt, decltype(comp_dead), Container> least({}, comp_dead);
        least.clear();
        {
            std::size_t curr_min_pos = std::numeric_limits<std::size_t>::max();
            for (std::size_t j = 0; j < k; ++j) {
                auto it = window.peakIt();
                std::size_t d = to_size_t(diff(begin, *it));
                if (d < curr_min_pos)
                    curr_min_pos = d;
                least.insert(*it);
                window.removePeak();
            }
            *window_min_pos = curr_min_pos;
        }
    
        std::size_t curr_pos = 1;
        bool using_min = true;
        for (auto it = std::begin(data) + m; it != std::end(data); ++it) {
            window.insert(it);
            removeLastDead(window, window_min_pos);
            removeLastDead(least, window_min_pos);
            auto rmcIt = window.peak();
            auto lpIt = least.peak();
            if (comp(*rmcIt, *lpIt)) {
                window.removePeak();
                auto lp = least.removePeak();
                least.insert(rmcIt);
                window.insert(lp);
                if (using_min && to_size_t(diff(begin, rmcIt)) == *window_min_pos) {
                    window_min_pos = &curr_pos;
                    using_min = false;
                }
            }
            print(window.data(), "Window");
            print(least.data(), "Least");
            ++curr_pos;
        }
    }
};

} // namespace cc232