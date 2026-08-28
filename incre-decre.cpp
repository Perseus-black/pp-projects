#include<iostream>
using namespace std;

int main() {
    int num;

    cout << "Enter a number: ";
    cin >> num;
    cout << "Multiplication Table of " << num << " is: " << endl; // Display multiplication table of the given number
    for(int i = 1; i <= 10; ++i) {
        cout << num << " * " << i << " = " << num * i << endl;
    }
    return 0;
}