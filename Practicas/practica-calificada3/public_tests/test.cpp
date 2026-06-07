#include "LeastElements.h"
#include "QAUtils.h"

int main() {
    using cc232::qa::shared::test;

    // AtCoder example test
    test<int>({3, 1, 4, 1, 5, 9}, 4, 3, {5, 6, 10});

    // Strictly increasing data
    test<int>({10, 20, 30, 40, 50}, 3, 2, {30, 50, 70});
    test<int>({1, 2, 3, 4, 5, 6, 7}, 5, 1, {1, 2, 3});

    // Strictly decreasing data
    test<int>({50, 40, 30, 20, 10}, 3, 2, {70, 50, 30});
    test<int>({9, 8, 7, 6, 5, 4, 3, 2, 1}, 3, 3, {24, 21, 18, 15, 12, 9, 6});

    // Repeating / Identical elements
    test<int>({5, 5, 5, 5, 5}, 3, 2, {10, 10, 10});
    test<int>({0, 0, 1, 0, 0, 1}, 3, 2, {0, 0, 0, 0});

    // Oscillating / Alternating patterns
    test<int>({100, 200, 100, 200, 100, 200}, 4, 2, {200, 200, 200});
    test<int>({9, 1, 9, 1, 9, 1, 9}, 4, 2, {2, 2, 2, 2});

    // Randomly distributed numbers
    test<int>({8, 2, 6, 4, 0, 9, 3}, 4, 3, {12, 6, 10, 7});
    test<int>({1, 9, 2, 8, 3, 7, 4, 6, 5}, 5, 2, {3, 5, 5, 7, 7});
    test<int>({15, 12, 18, 14, 20, 16}, 4, 3, {41, 44, 48});
    test<int>({100, 50, 200, 150, 75, 25}, 3, 2, {150, 200, 225, 100});

    // Including negative numbers
    test<int>({-5, -1, -3, -4, -2}, 3, 2, {-8, -7, -7});
    test<int>({1, 3, -2, 5, 8, -4, 3, 2}, 5, 3, {2, -3, -3, 1});

    // Full window equals k (k = m)
    test<int>({2, 4, 6, 8, 10, 12}, 4, 4, {20, 28, 36});
    test<int>({12, 10, 8, 6, 4, 2}, 4, 4, {36, 28, 20});

    // Window size equals data size (m = n)
    test<int>({1, 4, 2, 8, 5, 7}, 6, 3, {7});

    // Late introduction of very small numbers
    test<int>({42, 42, 42, 42, 10, 10}, 4, 2, {84, 52, 20});

    // Single least element across a sliding window (k = 1)
    test<int>({11, 22, 33, 44, 55, 66, 77}, 3, 1, {11, 22, 33, 44, 55});
    test<int>({7, 6, 5, 4, 3, 2, 1}, 5, 1, {3, 2, 1});

    std::cout << "All tests passed!" << std::endl;
    return 0;
}