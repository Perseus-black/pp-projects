#include <iostream>
#include <cstdlib>
#include <string>

int main() {
    std::string cmd;

    while (true) {
        std::cout << "myshell> ";
        getline(std::cin, cmd);

        if (cmd == "exit")
            break;

        system(cmd.c_str());
    }
}