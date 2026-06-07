#include "LeastElements.h"
#include "QAUtils.h"

template <typename T>
void run_suite(std::size_t n, std::size_t m, std::size_t k) {
    using cc232::qa::bench::measure;

    std::cout << "------------------------------------------------------" << std::endl;
    std::cout << " Dataset: N = " << n << " | Window: M = " << m << " | K = " << k << std::endl;
    std::cout << "------------------------------------------------------" << std::endl;

    // Generate random dataset
    std::vector<T> data(n);
    std::mt19937 rng(42);
    std::uniform_int_distribution<T> dist(-10000, 10000);
    for (std::size_t i = 0; i < n; ++i) data[i] = dist(rng);

    // Variables to capture the output for cross-validation
    long long total_sum0 = 0;
    long long total_sum1 = 0;

    // ==========================================
    // PARTIAL SORT
    // ==========================================
    double time0 = measure([&]() {
        for (std::size_t i = 0; i <= n - m; ++i) {
            std::vector<T> window(data.begin() + i, data.begin() + i + m);
            std::partial_sort(window.begin(), window.begin() + k, window.end());
            
            long long current_sum = 0;
            for (std::size_t j = 0; j < k; ++j) current_sum += window[j];
            total_sum0 += current_sum;
        }
    });
    std::cout << " " << std::left << std::setw(25) << "Partial sort: " 
        << std::fixed << std::setprecision(2) << time0 << " ms" << std::endl;

    // ==========================================
    // SLIDING TREAP
    // ==========================================
    double time1 = measure([&]() {
        cc232::LeastElements res(data, m, k); 
        for (std::size_t i = 0; i <= n - m; ++i) {
            total_sum1 += res.sumAndSlide(); 
        }
    });
    std::cout << " " << "\033[1;32m" << std::left << std::setw(25) << "LeastElements Treap: " << "\033[0m"
        << std::fixed  << std::setprecision(2) << time1 << " ms" << std::endl;

    // ==========================================
    // VERDICT
    // ==========================================
    if (total_sum0 != total_sum1) {
        std::cout << std::endl << "\033[1;31mERROR: Sums do not match!\033[0m" << std::endl;
        return;
    }

    double speedup = time0 / time1;
    std::cout << std::endl << " \033[1;36m-> Treap is " << speedup << "x faster!\033[0m" << std::endl << std::endl;
}

int main() {
    std::cout << std::endl << "======================================================" << std::endl;
    std::cout << "  PERFORMANCE BENCHMARK" << std::endl;
    std::cout << "======================================================" << std::endl << std::endl;

    run_suite<long long>(50000, 100, 20);
    run_suite<long long>(50000, 1000, 100);
    run_suite<long long>(50000, 10000, 1000);

    return 0;
}