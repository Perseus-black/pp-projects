#include <iostream>
struct Object {
    int objs;
    Object* value;
};

int main() {
    Object* obj = new Object();
    
    obj->objs = 67;
    obj->value = nullptr;

    std::cout << "Address of objs: " << obj << std::endl;
    std::cout << "Value of objs: " << obj->objs << std::endl;
    std::cout << "Address of value: " << obj->value << std::endl;

    delete obj;

/*
    std::cout << "Initiate an integer object:\n";   // Prompt user to input an integer value
    std::cin >> obj;   // Read the integer value from user input

    if (obj > 0) {
        int* ptr = &obj;   // Create a pointer that holds the address of the integer object

        std::cout << "Memory saves address of the object at: " << ptr << std::endl;   //Display memory address
        std::cout << "Reverse of a pointer is: " << *ptr << std::endl;   //Display value stored at memory address
    } else {
        std::cout << "Object is not a positive, false flag." << std::endl;
    }   */
    return 0;
}