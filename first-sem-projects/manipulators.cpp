#include <iostream>
#include <iomanip>

int main() {
    double num1;

    std::cout << "Enter any number: ";
    std::cin >> num1;
    
    std::cout << "Original value: " << num1 << '\n';

    std::cout << "\nUsing setw(10) with left:\n";
    std::cout << std::left << std::setw(10) << num1 << '\n';

    std::cout << "\nUsing setprecision(2) with fixed:\n";
    std::cout << std::fixed << std::setprecision(2) << num1 << '\n';

    std::cout << "\nUsing setprecision(4) with scientific:\n";
    std::cout << std::scientific << std::setprecision(4) << num1 << '\n';

    return 0;
}