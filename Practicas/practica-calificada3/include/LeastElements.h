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

struct LeastElements_traits {
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
};


template <typename T, typename Iterator, typename Compare = std::less<T>>
class LeastElements {
    static_assert(traits::is_valid_comp_v<T, Compare>, "Compare must be valid comparator");
    static_assert(traits::is_iterator_v<Iterator>, "Iterator must be valid iterator");
    static_assert(traits::is_summable_v<T>, "Type must be summable");
    static_assert(traits::is_subtractable_v<T>, "Type must be subtractable");

    using ItWrapper = LeastElements_traits::PrintableIt<Iterator>;
    struct TreapComparator {
        Compare comp;
        TreapComparator(const Compare& c = Compare{}) : comp(c) {}
        
        bool operator()(const ItWrapper &a, const ItWrapper &b) const {
            return (*a == *b) ? diff(a, b) > 0 : comp(*a, *b);
        }
    };

    const std::size_t m_;
    const std::size_t k_;
    const Iterator end;
    Iterator it;
    const TreapComparator treap_comp;
    Treap<ItWrapper, TreapComparator> window_;
    Treap<ItWrapper, TreapComparator> least_;
    T sumLeast_{};

    template <typename V>
    static inline constexpr std::size_t to_size_t(V value) {return static_cast<std::size_t>(value);}

    template <typename It = Iterator, std::enable_if_t<std::is_base_of_v<
        std::random_access_iterator_tag,
        typename std::iterator_traits<It>::iterator_category>, int> = 0>
    static inline typename std::iterator_traits<Iterator>::difference_type diff(Iterator a, Iterator b) {return b - a;}
    template <typename It = Iterator, std::enable_if_t<!std::is_base_of_v<
        std::random_access_iterator_tag,
        typename std::iterator_traits<It>::iterator_category> && std::is_base_of_v<
        std::bidirectional_iterator_tag,
        typename std::iterator_traits<It>::iterator_category>, int> = 0>
    static inline typename std::iterator_traits<Iterator>::difference_type diff(Iterator a, Iterator b) {return std::distance(a, b);}

    public:
    bool slideWindow() {
        if (it == end)
            throw std::logic_error("End of data reached");
        
        auto expIt = std::prev(it, m_);
        if (least_.remove(expIt))
            sumLeast_ -= *expIt;
        else
            window_.remove(expIt);
        window_.insert(it);

        if (least_.size() < k_) {
            auto cIt = *window_.minIt();
            window_.remove(cIt);
            least_.insert(cIt);
            sumLeast_ += *cIt;
        }
        // Changing frontier element
        if (window_.size() > 0 && least_.size() > 0) {
            auto minIt = *window_.minIt();
            auto maxIt = *least_.maxIt();
            
            if(treap_comp(minIt, maxIt)) {
                window_.remove(minIt);
                least_.remove(maxIt);
                window_.insert(maxIt);
                least_.insert(minIt);
                sumLeast_ += *minIt - *maxIt;

                ++it;
                return true;
            }
        }
        ++it;
        return false;
    }
    T sumLeast() {
        return sumLeast_;
    }
    T sumAndSlide() {
        T sum = sumLeast_;
        if (it != end)
            slideWindow();
        return sum;
    }
    
    const auto &window() const {
        return window_;
    }
    const auto &least() const {
        return least_;
    }
    
    explicit LeastElements(Iterator first, Iterator last, std::size_t m, std::size_t k, Compare comp = {}):
        m_(m), k_(k), treap_comp(comp), window_(treap_comp), least_(treap_comp), end(last), it(std::next(first, m)) {
        if (m < k || to_size_t(diff(first, last)) < m)
            throw std::logic_error("k <= m <= data.size() = n criteria is not met");

        for (auto dIt = first; dIt != first + m; ++dIt)
            window_.insert(dIt);
        for (std::size_t j = 0; j < k; ++j) {
            auto minIt = *window_.minIt();
            window_.remove(minIt);
            least_.insert(minIt);
            sumLeast_ += *minIt;
        }
    }
    template <typename Container>
    explicit LeastElements(const Container &data, std::size_t m, std::size_t k, Compare comp = {}):
        LeastElements(std::begin(data), std::end(data), m, k, comp) {
        static_assert(traits::is_iterable_v<Container>, "Container must be iterable");
    }

    ~LeastElements() = default;
};
template <typename Container, typename Compare = std::less<typename Container::value_type>>
LeastElements(const Container&, std::size_t, std::size_t, Compare = Compare{}) 
-> LeastElements<
    typename Container::value_type, 
    decltype(std::begin(std::declval<const Container&>())), 
    Compare
>;

} // namespace cc232