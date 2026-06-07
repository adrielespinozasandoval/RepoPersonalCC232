#include "LeastElements.h"
#include "QAUtils.h"

int main() {
    using cc232::qa::demo::demo;

    demo<int>({41, 56, 17, 82, 59, 76, 15}, 5, 2);
    
    return 0;
}