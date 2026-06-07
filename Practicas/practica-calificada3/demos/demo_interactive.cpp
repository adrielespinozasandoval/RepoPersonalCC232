#include "LeastElements.h"
#include "QAUtils.h"

int main() {
    using cc232::qa::demo::demo;

    std::cout << "======================================================" << std::endl << std::endl;

    // Read the custom array from the user
    std::cout << "Enter your dataset (numbers separated by spaces)" << std::endl;
    std::cout << "or type 'q' to quit:\n> ";
    
    std::string line;
    std::getline(std::cin, line);
    
    if (line == "q" || line == "Q")
        return 0;

    // Parse the string into a vector of integers
    std::istringstream iss(line);
    std::vector<int> data;
    int num;
    while (iss >> num) {
        data.push_back(num);
    }

    if (data.empty()) {
        std::cout << std::endl << "\033[1;31mNo valid numbers detected.\033[0m Exiting...";
        return 0;
    }

    // Read the parameters
    std::size_t m, k;
    std::cout << "\nEnter Window Size (m): ";
    std::cin >> m;
    std::cout << "Enter Least Elements (k): ";
    std::cin >> k;
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    try {
        demo<int>(data, m, k);
    } catch (const std::exception& e) {
        // Catches std::logic_error
        std::cout << std::endl << "\033[1;31mLogic Error:\033[0m " << e.what() << std::endl;
    }
    
    return 0;
}