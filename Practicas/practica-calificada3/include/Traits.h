#pragma once

#include <utility>
#include <iterator>
#include <type_traits>

namespace cc232 {
    
    struct traits {
        template <typename T, typename Compare>
        struct is_valid_comp: std::integral_constant<bool, std::is_invocable_r_v<bool, Compare, const T&, const T&>> {};
        template <typename T, typename Compare>
        static constexpr bool is_valid_comp_v = is_valid_comp<T, Compare>::value;

        template <typename Gen, typename = void>
        struct is_generator: std::false_type {};
        template <typename Gen>
        struct is_generator<Gen, std::void_t<
            decltype(std::declval<Gen&>()())
        >>: std::true_type {};
        template <typename Gen>
        static constexpr bool is_generator_v = is_generator<Gen>::value;

        template <typename T, typename = void>
        struct is_less_comparable: std::false_type {};
        template <typename T>
        struct is_less_comparable<T, std::void_t<
            decltype(std::declval<std::decay_t<decltype(std::declval<T&>())>>()
            < std::declval<std::decay_t<decltype(std::declval<T&>())>>()
        )>>: std::true_type {};
        template <typename T>
        static constexpr bool is_less_comparable_v = is_less_comparable<T>::value;

        template <typename T, typename = void>
        struct is_summable : std::false_type {};
        template <typename T>
        struct is_summable<T, std::void_t<
            decltype(std::declval<T>() + std::declval<T>())
        >>: std::is_same<decltype(std::declval<T>() + std::declval<T>()), T> {};
        template <typename T>
        static constexpr bool is_summable_v = is_summable<T>::value;

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
    };

} // namespace cc232