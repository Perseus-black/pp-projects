#include<iostream>
using namespace std;

int main() {
    int num;

    cout << "Enter a number: ";
    cin >> num;
    cout << "Multiplication Table of " << num << " is: " << endl; 

    for(int i = 1; i <= 10; ++i) {   // Display multiplication table of the given number
        cout << num << " * " << i << " = " << num * i << endl;
    }
    
    cout << "\n";

    for(int i = 10; i >=0; --i) {   // Display reverse multiplication table of the given number
        cout << num << " * " << i << " = " << num * i << endl;
    }
    return 0;
}