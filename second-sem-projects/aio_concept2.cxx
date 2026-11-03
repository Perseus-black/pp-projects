#include <iostream>
#include <string>

class Vehicle {                                       // Abstract class demonstrating Abstraction
protected:                                            // Encapsulation (protected allows derived classes access)
    std::string brand;
    int speed;

public:
    Vehicle() : brand("Unknown"), speed(0) {          // Default Constructor
        std::cout << "Vehicle created (default constructor)\n";
    }

    Vehicle(std::string b, int s) : brand(b), speed(s) {   // Parameterized Constructor
        std::cout << "Vehicle created: " << brand << ", speed: " << speed << std::endl;
    }

    Vehicle(const Vehicle &v) {                       // Copy Constructor
        brand = v.brand;
        speed = v.speed;
        std::cout << "Vehicle copied\n";
    }

    void setSpeed(int s) {                            // Method (Function)
        speed = s;
        }                                             // Encapsulation
        
    int getSpeed() {
        return speed;
        }

    virtual void start() = 0;                         // Pure virtual function
};

class Car : public Vehicle {                          // Inheritance & Polymorphism
private:
    std::string model;                                //Encapsulation

public:
    Car() : Vehicle(), model("Unknown") {             // Default Constructor
        std::cout << "Car created (default constructor)\n";
    }

    Car(std::string b, int s, std::string m) : Vehicle(b, s), model(m) {  // Parameterized Constructor
        std::cout << "Car created: " << brand << ", " << model << std::endl;
    }

    Car(const Car &c) : Vehicle(c), model(c.model) {  // Copy Constructor
        std::cout << "Car copied\n";
    }

    Car(std::string m) : Vehicle(), model(m) {        // Constructor Overloading
        std::cout << "Car created with model only: " << model << std::endl;
    }

    void display() {                                  // Method (Function)
        std::cout << "Brand: " << brand << ", Model: " << model << ", Speed: " << speed << std::endl;
    }

    void start() override {                           // Method Overriding (Polymorphism)
        std::cout << "Car started: " << brand << " " << model << std::endl;
    }
};

int main() {
    // Object creation
    Car car1;                               // Default constructor
    Car car2("Toyota", 120, "Camry");       // Parameterized constructor
    Car car3 = car2;                        // Copy constructor
    Car car4("Mustang");                    // Constructor overloading

    // Using methods
    car2.setSpeed(150);
    car2.display();

    // Polymorphism
    Vehicle* vptr = &car2;
    vptr->start();                          // Calls overridden start()

    return 0;
}