#include <iostream>            //library for input/output
#include <string>              //library for including string values
#include <cmath>               //library for math operations

                               //learned std:: instead of using namespace
    void doNothing(int&){}     //learned proper use of dummy function [user defined] exm: doNothing

    int userdef_func(int x) {                       //User-defined function declared to return value for parameter [int x]
        return x;
    }
int main()                     //learned new line spacing after main()
{
    float a {};                //NEW: learned initialization from "a = 5" [copy initialization] to a(5) [direct initialization]
                               // & a{5} [direct-list initialization]
    int b {};                  //NEW: learned WARNING! if value not used
                        
    std::cout << "Enter a number: ";                //NEW: learned [[maybe_unused]] correct use
    std::cin >> a >> b;                               

    std::cout << sizeof(int(b)) << std::endl;       //learned correct use of keyword: sizeof()

    float c (10.539);
    double d {15.5};

    std::cout << c << '\n' << d << '\n';            //learned correct use of \n instead of endl

    std::cout << 5 << '\n';                         //This is a literal value called constant       
    int x {5};                                      //This is a variable with value initialized [diff from const]   
    
    std::string str {"This is a string value"};     //A String value have been initialized
    std::cout << str << '\n';

    std::cout << a + b * c / x << std::endl;        //Mathematical operators are used & chained together
    
    //Note: Operations PRECEDENCE follow standard math rules like PEMDAS or BODMAS

    std::cout << userdef_func(20) << '\n';          //Note: User-defined functions are called explicitly using cout

    std::cout << "\n===============================================================\n";
    std::cout << "           Basic programming knowledge ends here.\n";
    std::cout << "===============================================================\n" << std::endl;


    int wNum {1999};
    double dNum {3.14159};
    float fNum {3.79f};
    char sChar {'Z'};                                      //this is changeable to any character
    const char sChar2 {'A'};                               //this is not changeable to any character
    std::string strs {"'string' declared."};
    bool bVal {true};                                      //can also be false

    std::cout << "I am here." << "\t" << std::endl;        //ignoring namespace
    std::cout << "I already know this.\n";
    std::cout << "But I am here \"anyway\".\n";

    std::cout << wNum * dNum / fNum << "\n";
    std::cout << "Alphabets end with the character: " << sChar << "\n";
    std::cout << "String stores many characters such as: " << strs << "\n";

    
    /* The general rules for naming variables are:

    1. Names can contain letters, digits and underscores
    2. Names must begin with a letter or an underscore (_)
    3. Names are case-sensitive (myVar and myvar are different variables)
    4. Names cannot contain whitespaces or special characters like !, #, %, etc.
    5. Reserved words (like C++ keywords, such as int) cannot be used as names */

    //const is used for universal nouns or numbers.
    //const is always declared at intialization.
    
    return 0;
}