#include <iostream>
using namespace std;

// Abstract class demonstrating Abstraction
class Vehicle {
protected:       // Encapsulation (protected allows derived classes access)
    string brand;
    int speed;

public:
    // Default Constructor
    Vehicle() : brand("Unknown"), speed(0) {
        cout << "Vehicle created (default constructor)\n";
    }

    // Parameterized Constructor
    Vehicle(string b, int s) : brand(b), speed(s) {
        cout << "Vehicle created: " << brand << ", speed: " << speed << endl;
    }

    // Copy Constructor
    Vehicle(const Vehicle &v) {
        brand = v.brand;
        speed = v.speed;
        cout << "Vehicle copied\n";
    }

    // Method (Function)
    void setSpeed(int s) {
        speed = s;
        }   // Encapsulation
        
    int getSpeed() {
        return speed;
        }

    virtual void start() = 0;  // Pure virtual function
};

// Inheritance & Polymorphism
class Car : public Vehicle {
private:
    string model;       //Encapsulation

public:
    // Default Constructor
    Car() : Vehicle(), model("Unknown") {
        cout << "Car created (default constructor)\n";
    }

    // Parameterized Constructor
    Car(string b, int s, string m) : Vehicle(b, s), model(m) {
        cout << "Car created: " << brand << ", " << model << endl;
    }

    // Copy Constructor
    Car(const Car &c) : Vehicle(c), model(c.model) {
        cout << "Car copied\n";
    }

    // Constructor Overloading
    Car(string m) : Vehicle(), model(m) {
        cout << "Car created with model only: " << model << endl;
    }

    // Method (Function)
    void display() {
        cout << "Brand: " << brand << ", Model: " << model << ", Speed: " << speed << endl;
    }

    // Method Overriding (Polymorphism)
    void start() override {
        cout << "Car started: " << brand << " " << model << endl;
    }
};

int main() {
    // Object creation
    Car car1;     // Default constructor
    Car car2("Toyota", 120, "Camry"); // Parameterized constructor
    Car car3 = car2;     // Copy constructor
    Car car4("Mustang");    // Constructor overloading

    // Using methods
    car2.setSpeed(150);
    car2.display();

    // Polymorphism
    Vehicle* vptr = &car2;
    vptr->start();    // Calls overridden start()

    return 0;
}