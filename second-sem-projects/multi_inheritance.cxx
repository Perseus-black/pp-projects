#include <iostream>
#include <string>
using namespace std;

class BMW {
    public:
    string name;
    int r_year;
    string model;
    
    void C_data(string name, int r_year, string model) {
        name = "BMW X5", r_year = 2021, model = "X4";

        cout << "Car is: " << name << endl;
        cout << "Released in: " << r_year << endl;
        cout << "Model is: " << model << endl;
     }
};

class BMW_ex : public BMW {
    public:
    string feature;
    
    void C_data(string name, int r_year, string model, string feature) {
        BMW::C_data(name, r_year, model);

        cout << endl;
        cout << "Car is: " << name << endl;
        cout << "Released in: " << r_year << endl;
        cout << "Model is: " << model << endl;
        cout << "Feature: " << feature << endl;
    }
};

int main() {
    BMW_ex ex;
    
    ex.C_data("BMW X5", 2023, "X5", "Sunroof");

    return 0;
}