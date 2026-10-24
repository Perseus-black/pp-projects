#include<iostream>
#include<cmath>

int main() {
    float marks;
    float percentage;
    float t_marks {100.0};
    
    std::cout << "Enter your marks: ";
    std::cin >> marks;

    percentage = marks * 100.0 / t_marks; 

    if (marks >= 90.0)
    {
        std::cout << "Grade: A+\t" << percentage << "%\n";
    }
    else if (marks >= 80.0)
    {
        std::cout << "Grade: A\t" << percentage << "%\n";
    }
    else if (marks >= 70.0)
    {
        std::cout << "Grade: B\t" << percentage << "%\n";
    }
    else if (marks >= 60.0)
    {
        std::cout << "Grade: C\t" << percentage << "%\n";
    }
    else if (marks >= 50.0)
    {
        std::cout << "Grade: D\t" << percentage << "%\n";
    }
    else
    {
        std::cout << "Grade: F\n";
        std::cout << "Your percentage is below 50.";
    }
    return 0;
}