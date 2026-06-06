#pragma once

#include <utility>
#include <iterator>
#include <type_traits>

namespace cc232 {
    
    struct traits {
        template <typename T, typename Compare>
        struct is_valid_comp: std::integral_constant<bool, std::is_invocable_r_v<bool, Compare, T, T>> {};
        template <typename T, typename Compare>
        static constexpr bool is_valid_comp_v = is_valid_comp<T, Compare>::value;

        template <typename It, typename = void>
        struct is_random_access_iterator: std::true_type {};
        template <typename It>
        struct is_random_access_iterator<It, std::void_t<typename std::iterator_traits<It>::iterator_category>>:
            std::is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<It>::iterator_category> {};
        template <typename It>
        static constexpr bool is_random_access_iterator_v = is_random_access_iterator<It>::value;

        template <typename Container, typename = void>
        struct is_stack_like: std::false_type {};
        template <typename Container>
        struct is_stack_like<Container, std::void_t<
            decltype(std::declval<Container&>().push_back(std::declval<typename Container::value_type>())),
            decltype(std::declval<Container&>().pop_back()),
            decltype(std::declval<Container&>().clear())
        >>: std::true_type {};
        template <typename Container>
        static constexpr bool is_stack_like_v = is_stack_like<Container>::value;

        template <typename Iterator, typename = void>
        struct is_iterator: std::false_type {};
        template <typename Iterator>
        struct is_iterator<Iterator, std::void_t<
            typename std::iterator_traits<Iterator>::value_type,

            decltype(*std::declval<Iterator&>()),
            decltype(++std::declval<Iterator&>()),
            decltype(std::declval<Iterator&>() != std::declval<Iterator&>())
        >>: std::true_type {};
        template <typename Iterator>
        static constexpr bool is_iterator_v = is_iterator<Iterator>::value;

        template <typename Container, typename = void>
        struct is_iterable: std::false_type {};
        template <typename Container>
        struct is_iterable<Container, std::void_t<
            decltype(std::begin(std::declval<Container&>())),
            decltype(std::end(std::declval<Container&>()))
        >>: std::integral_constant<bool, is_iterator_v<decltype(std::begin(std::declval<Container&>()))>> {};
        template <typename Container>
        static constexpr bool is_iterable_v = is_iterable<Container>::value;

        template <typename Container, typename = void>
        struct has_random_access_iterator: std::false_type {};
        template <typename Container>
        struct has_random_access_iterator<Container, std::void_t<
            decltype(std::begin(std::declval<const Container&>())),
            decltype(std::end(std::declval<const Container&>())),
            decltype(std::cbegin(std::declval<const Container&>())),
            decltype(std::cend(std::declval<const Container&>()))
        >>: std::integral_constant<bool, is_random_access_iterator_v<decltype(std::begin(std::declval<const Container&>()))>> {};
        template <typename Container>
        static constexpr bool has_random_access_iterator_v = has_random_access_iterator<Container>::value;
    };

} // namespace cc232