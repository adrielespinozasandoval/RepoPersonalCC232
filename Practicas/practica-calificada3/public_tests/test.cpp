#include <iostream>
#include <vector>
#include "LeastElements.h"

int main() {
    std::vector<int> vec{1, 2, 5, 67, 1, 4, 0, 8};
    std::size_t m = 3;
    std::size_t k = 2;

    cc232::LeastElements<int, std::vector<int>> res(vec, m, k);

    std::cout << "--- Initial Window ---" << std::endl;
    std::cout << "Window: ";
    for (auto it : res.window()) { std::cout << *it << " "; } 
    std::cout << "\nLeast:  ";
    for (auto it : res.least()) { std::cout << *it << " "; }
    std::cout << "\n----------------------\n";

    std::size_t slides = vec.size() - m;
    for (std::size_t i = 0; i < slides; ++i) {
        res.nextLeast();
        
        std::cout << "--- Slide " << i + 1 << " ---" << std::endl;
        std::cout << "Window: ";
        for (auto it : res.window()) { std::cout << *it << " "; }
        std::cout << "\nLeast:  ";
        for (auto it : res.least()) { std::cout << *it << " "; }
        std::cout << "\n----------------------\n";
    }

    return 0;
}