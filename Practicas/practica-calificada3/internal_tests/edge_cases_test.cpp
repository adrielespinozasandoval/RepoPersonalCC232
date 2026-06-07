#include "LeastElements.h"
#include "QAUtils.h"

int main() {
    using cc232::qa::shared::test;
    using cc232::qa::internal::test_throw;

    // ---------------------------------------------------------
    // BOUNDARY SIZES
    // ---------------------------------------------------------
    
    // Absolute minimum valid configuration: n=1, m=1, k=1
    test<int>({42}, 1, 1, {42});

    // Maximum constraint equality: n = m = k
    test<int>({1, 2, 3, 4, 5}, 5, 5, {15});

    // Full window, but only asking for the absolute minimum: n = m, k = 1
    test<int>({9, 7, 5, 3, 1}, 5, 1, {1});


    // ---------------------------------------------------------
    // TreapComparator STRESSING
    // ---------------------------------------------------------
    
    // The "Duplicate Horde": Forces the comparator to heavily rely on memory addresses
    test<int>({7, 7, 7, 7, 7, 7}, 3, 2, {14, 14, 14, 14});

    // Zeroes only: Tests summation identity and tie-breaking
    test<int>({0, 0, 0, 0, 0}, 3, 2, {0, 0, 0});

    // A single outlier moving through a sea of duplicates
    // Window 1: {5,5,5} (sum 10). Window 2: {5,5,1} (sum 6)...
    test<int>({5, 5, 5, 1, 5, 5, 5}, 3, 2, {10, 6, 6, 6, 10});

    
    // ---------------------------------------------------------
    // EXTREME VALUES
    // ---------------------------------------------------------
    
    constexpr long long LL_MAX = std::numeric_limits<long long>::max();
    constexpr long long LL_MIN = std::numeric_limits<long long>::min();
    
    test<long long>({LL_MAX, LL_MIN, 0LL, LL_MAX}, 3, 2, {
        LL_MIN + 0LL,  // Window 1: {MAX, MIN, 0} -> Least 2: MIN, 0
        LL_MIN + 0LL   // Window 2: {MIN, 0, MAX} -> Least 2: MIN, 0
    });


    // ---------------------------------------------------------
    // SHIFTING MAX
    // ---------------------------------------------------------
    
    // As the window slides, the element leaving the window is EXACTLY 
    // the maximum element inside the `least_` tree.
    test<int>({1, 2, 3, 4, 5, 6}, 3, 2, {3, 5, 7, 9});
    

    // ---------------------------------------------------------
    // EXCEPTION HANDLING
    // ---------------------------------------------------------
    
    // Window size is larger than the entire dataset (m > n)
    test_throw<int>({1, 2, 3}, 4, 2);

    // Asking for more elements than the window holds (k > m)
    test_throw<int>({1, 2, 3, 4}, 2, 3);
    
    // Both constraints violated simultaneously (k > m > n)
    test_throw<int>({1}, 5, 10);

    std::cout << "All Edge Case tests passed!" << std::endl;
    return 0;
}