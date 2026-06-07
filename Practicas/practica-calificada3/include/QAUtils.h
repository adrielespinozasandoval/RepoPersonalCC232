#pragma once

#include "LeastElements.h"

#include <chrono>
#include <limits>
#include <random>
#include <vector>
#include <cassert>
#include <iomanip>
#include <numeric>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <initializer_list>

namespace cc232::qa {

    namespace shared {
        template <typename T, typename Container = std::vector<T>>
        void test(const Container &data, std::size_t m, std::size_t k, std::initializer_list<T> expected) {
            cc232::LeastElements res(data, m, k);
            for (T exp: expected) {
                assert(res.sumAndSlide() == exp && "Got a wrong sum!");
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

    namespace demo {
        template <typename T, typename Container = std::vector<T>>
        void demo(const Container &data, std::size_t m, std::size_t k) {
            std::cout << "======================================================" << std::endl << std::endl;

            std::cout << "Dataset : [ ";
            for (const auto& val : data) std::cout << val << " ";
            std::cout << "]" << std::endl;
            std::cout << "Config  : Window (m) = " << m << " | Least to sum (k) = " << k << std::endl << std::endl;

            cc232::LeastElements res(data, m, k);
            std::size_t n = std::distance(std::begin(data), std::end(data));

            for (std::size_t i = 0; i <= n - m; ++i) {
                std::cout << "Step " << i + 1 << "  : Window [" << i << " -> " << i + m - 1 << "]" << std::endl;
                std::cout << "------------------------------------------------------" << std::endl;

                // Printing with ANSI color highlighting
                std::cout << "Array   : [ ";
                for (std::size_t j = 0; j < n; ++j) {
                    if (j >= i && j < i + m) {
                        // Green text for elements currently inside the window
                        std::cout << "\033[1;32m" << data[j] << "\033[0m "; 
                    } else {
                        // Faded gray text for elements outside the window
                        std::cout << "\033[1;30m" << data[j] << "\033[0m "; 
                    }
                }
                std::cout << "]" << std::endl;

                std::cout << "Least " << k << " : { ";
                for (auto it : res.least()) {
                    std::cout << "\033[1;36m" << *it << "\033[0m "; 
                }
                
                std::cout << "}  =>  Sum: \033[1;33m" << res.sumLeast() << "\033[0m" << std::endl << std::endl;

                if (i < n - m) {
                    res.slideWindow();
                }
            }
            std::cout << "======================================================" << std::endl << std::endl;
        }
    };

    namespace bench {
        template <typename Func>
        double measure(Func&& function) {
            auto start = std::chrono::high_resolution_clock::now();
            function();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> duration = end - start;
            return duration.count();
        }
    };
}
