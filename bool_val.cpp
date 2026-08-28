#include<iostream>
#include<string>
using namespace std;

int main() {
    int age;
    bool nationality;
    bool religion;
    bool domicile;

    cout << "Enter your age: ";
    cin >> age;
    cout << "Are you a citizen of Pakistan? (yes/no): ";
    string temp;
    cin >> temp;
    nationality = (temp == "yes");
    cout << "Are you a Muslim? (yes/no): ";
    cin >> temp;
    religion = (temp == "yes");
    cout << "Do you have a domicile of Pakistan? (yes/no): ";
    cin >> temp;
    domicile = (temp == "yes");

    if (age >= 18 && nationality == true || religion == true && domicile == true) {
        cout << "You are eligible to vote in Pakistan." << endl;
    } else {
        cout << "You are not eligible to vote in Pakistan." << endl;
    }

    return 0;
}