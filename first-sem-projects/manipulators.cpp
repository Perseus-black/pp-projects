#include<iostream>
#include<iomanip>
using namespace std;

int main() {
    double num1 = 97.6825;
    cout << "Original value: " << num1 << endl;

    cout << "\nUsing setw(10) with left:\n";
    cout << left << setw(10) << num1 << endl;

    cout << "\nUsing setprecision(2) with fixed:\n";
    cout << fixed << setprecision(2) << num1 << endl;

    cout << "\nUsing setprecision(4) with scientific:\n";
    cout << scientific << setprecision(4) << num1 << endl;

    return 0;
}