#include<iostream>
using namespace std;
int main() {
    double f_num, s_num;
    char op;
    
    cout << "Enter a digit: ";
    cin >> f_num;
    
    cout << "Enter an operator (+, -, *, /): ";
    cin >> op;
    
    cout << "Enter another digit: ";
    cin >> s_num;
    
    switch (op) {
        case '+' :
        cout << "Result: " << f_num + s_num;
        break;
        
        case '-' :
        cout << "Result: " << f_num - s_num;
        break;
        
        case '*' :
        cout << "Result: " << f_num * s_num;
        break;
        
        case '/' :
        if (s_num != 0)
        cout << "Result: " << f_num / s_num;
        
        else
        cout << "Error: Division by zero!";
       break;
       
       default:
       cout << "Invalid Operator!";
    }
    return 0;
}