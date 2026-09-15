#include <iostream>
#include <string>
#include <cmath>

int main() {
    int wNum = 1999;
    double dNum = 3.14159;
    float fNum = 3.79f;
    char sChar = 'Z';                   //this is changeable to any character
    const char sChar2 = 'A';            //this is not changeable to any character
    std::string strs = "string declared.";
    bool bVal = true;   //can also be false

    std::cout << "I am here." << "\t" << std::endl; //ignoring namespace
    std::cout << "I already know this.\n";
    std::cout << "But I am here \"anyway\".\n";

    std::cout << wNum * dNum / fNum << "\n";
    std::cout << "Alphabets end with the character: " << sChar << "\n";
    std::cout << "String stores many characters such as: " << strs << "\n";

    /*
    The general rules for naming variables are:

    1. Names can contain letters, digits and underscores
    2. Names must begin with a letter or an underscore (_)
    3. Names are case-sensitive (myVar and myvar are different variables)
    4. Names cannot contain whitespaces or special characters like !, #, %, etc.
    5. Reserved words (like C++ keywords, such as int) cannot be used as names
    */

    //const is used for universal nouns or numbers.
    //const is always declared at intialization.

    return 0;
}
