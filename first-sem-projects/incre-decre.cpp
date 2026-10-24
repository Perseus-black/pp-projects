#include<iostream>
#include<cmath>

int main() {
    float num;

    std::cout << "Enter a number: ";
    std::cin >> num;

    std::cout << "Multiplication Table of " << num << " is: \n"; 

    for(int i = 1; i <= 10; ++i) {   // Display multiplication table of the given number
        std::cout << num << " * " << i << " = " << num * i << std::endl;
    }
    
    std::cout << "\n";

    for(int i = 10; i >= 0; --i) {   // Display reverse multiplication table of the given number
        std::cout << num << " * " << i << " = " << num * i << std::endl;
    }
    return 0;
}