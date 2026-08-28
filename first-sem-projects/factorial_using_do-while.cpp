#include<iostream>
using namespace std;

int main() {
    int num, factorial = 1;
    cout << "Enter a positive integer: ";
    cin >> num;

    if (num < 0) {
        cout << "Factorial is not defined for negative numbers." << endl;
        return 1;
    }

    int i = 1;
    do {
        factorial *= i;
        i++;
    } while (i <= num);

    cout << "Factorial of " << num << " = " << factorial << endl;
    return 0;
}