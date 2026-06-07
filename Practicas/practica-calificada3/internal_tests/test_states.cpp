#include "LeastElements.h"
#include "QAUtils.h"

template <typename T, typename Container = std::vector<T>>
void test_states(std::size_t n, std::size_t m, std::size_t k) {
    // Generate a random dataset
    std::mt19937 rng(1337); 
    std::uniform_int_distribution<T> dist(-1000, 1000);
    
    Container data(n);
    for (std::size_t i = 0; i < n; ++i) {
        data[i] = dist(rng);
    }

    cc232::LeastElements res(data, m, k);

    // Step through every single window frame
    for (std::size_t i = 0; i <= n - m; ++i) {
        const auto& win = res.window();
        const auto& lst = res.least();

        // ==========================================
        // CHECKING TREAP STATE INVARIANTS
        // ==========================================
        
        assert(win.isTreap() && "Window Treap structural invariants broken!");
        assert(lst.isTreap() && "Least Treap structural invariants broken!");
        assert(lst.size() == k && "Least Treap size drift detected!");
        assert(win.size() == (m - k) && "Window Treap size drift detected!");

        if (lst.size() > 0 && win.size() > 0) {
            auto maxOfLeast = *lst.maxIt();
            auto minOfWindow = *win.minIt();
            assert(*maxOfLeast <= *minOfWindow && "Partition constraint violated!");
        }

        // ==========================================
        // LEAST ELEMENTS STATE
        // ==========================================
        
        // Build the Brute-Force Oracle for the current window [i, i + m)
        std::vector<T> oracle_window(data.begin() + i, data.begin() + i + m);
        std::sort(oracle_window.begin(), oracle_window.end());

        // Test if optimized sum match the brute-force sum
        T oracle_sum = 0;
        for(std::size_t j = 0; j < k; ++j) {
            oracle_sum += oracle_window[j];
        }
        assert(res.sumLeast() == oracle_sum && "Sum mismatch against Oracle!");

        // Test if the exact contents of the `least_` Treap match the Oracle
        // (This proves iterators are tracking the correct duplicate values)
        std::vector<T> least_contents;
        for (auto it : lst) {
            least_contents.push_back(*it); // Double dereference: iterator -> wrapper -> T
        }
        
        // Because the Treap iterates in-order (sorted), the arrays should be identical
        for (std::size_t j = 0; j < k; ++j) {
            assert(least_contents[j] == oracle_window[j] && "Treap contents mismatched with Oracle array!");
        }

        // Slide the window forward if it isn't at the end
        if (i < n - m) {
            res.slideWindow();
        }
    }
}

int main() {
    std::cout << "Running states validation test..." << std::endl;
    test_states<long long>(50000, 500, 100);

    std::cout << "All states validated!" << std::endl;
    return 0;
}