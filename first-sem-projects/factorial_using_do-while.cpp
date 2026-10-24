#include<iostream>
#include<cmath>

int main() {
    int num, factorial {1};

    std::cout << "Enter a positive integer: ";
    std::cin >> num;

    if (num < 0) {
        std::cout << "Factorial is not defined for negative numbers.\n";
        return 1;
    }

    int i {1};
    do {
        factorial *= i;
        i++;
    } while (i <= num);

    std::cout << num << "!" << " = " << factorial << std::endl;
    return 0;
}