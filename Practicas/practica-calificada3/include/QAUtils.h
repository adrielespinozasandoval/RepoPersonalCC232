#pragma once

#include "LeastElements.h"

#include <limits>
#include <vector>
#include <cassert>
#include <iostream>
#include <initializer_list>

namespace cc232::qa {

    namespace shared {
        template <typename T, typename Container = std::vector<T>>
        void test(const Container &data, std::size_t m, std::size_t k, std::initializer_list<T> expected) {
            cc232::LeastElements res(data, m, k);
            for (T exp: expected) {
                assert(res.sumAndSlide() == exp);
            }
        }
    };
    namespace internal {
        template <typename T, typename Container = std::vector<T>>
        void test_throw(const Container &data, std::size_t m, std::size_t k) {
            bool thrown = false;
            try {
                cc232::LeastElements res(data, m, k);
            } catch (const std::logic_error&) {
                thrown = true;
            }
            assert(thrown && "Expected std::logic_error was not thrown!");
        }
    };
}

