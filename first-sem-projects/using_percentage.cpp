#include <iostream>
#include <cmath>

int main() {
    float t_marks, ob_marks;

    std::cout << "Enter the total marks: ";
    std::cin >> t_marks;

    std::cout << "Enter the obtained marks: ";
    std::cin >> ob_marks;

    std::cout << '\n';
    
    float percentage = (ob_marks / t_marks) * 100.0;
    float result = (percentage / 100.0) * t_marks;

    std::cout << "Marks obtained: " << result << '\n';
    std::cout << "Percentage: " << percentage << "%" << std::endl;

    return 0;
}