#include<iostream>
using namespace std;

class BMW {
    public:
    string name;
    int R_year;
    string model;
    
    void C_data() {
        name = "BMW M5";
        cout << "Car is: " << name << endl;
        R_year = 2023;
        cout << "Released in: " << R_year << endl;
        model = "BMW M5 CS";
        cout << "Model is: " << model << endl;
     }
};

class BMW_ex : public BMW {
    public:
    string feature;
    
    void C_data2() {
        name = "BMW M3";
        cout << "Car is: " << name << endl;
        R_year = 2017;
        cout << "Released in: " << R_year << endl;
        model = "BMW M3 CS";
        cout << "Model is: " << model << endl;
        feature = "Twin Turbo";
        cout << "It's feature is: " << feature << endl;
    }
};

int main() {
    BMW_ex Call_d;
    
    Call_d.C_data();
    Call_d.C_data2();
    return 0;
}