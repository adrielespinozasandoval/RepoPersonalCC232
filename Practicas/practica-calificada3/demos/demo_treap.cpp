#include "Treap.h"
#include "QAUtils.h"

template <typename T>
void treap_demo(std::initializer_list<T> data) {
    cc232::Treap<T> treap;
    for (const T &d: data) {
        treap.insert(d);
        std::cout << treap << std::endl;
    }
    std::cout << treap << std::endl;
}

int main() {
    treap_demo<int>({41, 56, 17, 82, 59, 76, 15});

    return 0;
}