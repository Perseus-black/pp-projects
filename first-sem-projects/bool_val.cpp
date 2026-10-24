#include<iostream>
#include<string>

int main() {
    int age;
    bool nationality;
    bool religion;
    bool domicile;

    std::cout << "Enter your age: ";
    std::cin >> age;

    std::cout << "Are you a citizen of Pakistan? (yes/no): ";
    std::string temp;
    std::cin >> temp;

    nationality = (temp == "yes");
    std::cout << "Are you a Muslim? (yes/no): ";
    std::cin >> temp;

    religion = (temp == "yes");
    std::cout << "Do you have a domicile of Pakistan? (yes/no): ";
    std::cin >> temp;
    
    domicile = (temp == "yes");

    if (age >= 18 && nationality == true || religion == true && domicile == true) {
        std::cout << "You are eligible to vote in Pakistan.\n";
    } else {
        std::cout << "You are not eligible to vote in Pakistan.\n";
    }

    return 0;
}