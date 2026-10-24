#include<iostream>
#include<cmath>

int main() {
    double f_num, s_num;
    char op;
    
    std::cout << "Perform an operation: ";
    std::cin >> f_num >> op >> s_num;
    
    switch (op) {
        case '+' :
        std::cout << "Result: " << f_num + s_num;
        break;
        
        case '-' :
        std::cout << "Result: " << f_num - s_num;
        break;
        
        case '*' :
        std::cout << "Result: " << f_num * s_num;
        break;
        
        case '/' :
        if (s_num != 0)
        std::cout << "Result: " << f_num / s_num;
        
        else
        std::cout << "Error: Division by zero!";
        break;
       
        default:
        std::cout << "Invalid Operator!";
    }
    return 0;
}