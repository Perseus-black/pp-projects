#include <iostream>
#include <string>

class LockScreen {
    public:
    void authenticate() {
        std::string expected_key = "un3x93c73d_k3y";   //expected key
        std::string key;
        bool authenticated = false;

    std::cout << "==================\n";
    std::cout << "   LOCK SCREEN\n";
    std::cout << "==================\n" << std::endl;

    std::cout << "Provide authentication to unlock the screen." << std::endl;
    std::cout << "Press 'ENTER' to continue." << std::endl;
    std::cin.get();   // Wait for user to press ENTER

    for(int i = 0; i < 3; i++) {
        std::cout << "Enter key to unlock: ";
        std::cin >> key;
        std::cout << std::endl;

    if(key == expected_key) {
        std::cout << "\nAuthentication confirmed.\n";
        std::cout << "Welcome back, Admin.\n";

        authenticated = true;
        break;
    }
    
    if (i == 0) {
        std::cout << "WARNING! ADMIN WOULD NOT DO THAT.\n";
        std::cout << "Please try again." << std::endl;
        }
        else if (i == 1) {
            std::cout << "LAST WARNING! PROCEED WITH CAUTION.\n";
            std::cout << "One attempt remaining." << std::endl; 
                }
                else {
                std::cout << "FORCED LOGIN DETECTED.\n";
                std::cout << "Admin have been notified." << std::endl;
            }
        }

        if (!authenticated) {
            std::cout << "\nSCREEN HAVE BEEN LOCKED OUT UNTIL ADMIN LOGS IN.\n";
        }
    }
};

int main() {
    LockScreen lcn;
    lcn.authenticate();
}