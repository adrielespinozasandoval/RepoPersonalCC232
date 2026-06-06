#include "LeastElements.h"

int main() {
    std::vector<int> vec{1, 2, 5, 67, 1};
    cc232::LeastElements<int, std::vector<int>> res(vec, 3, 2);
}