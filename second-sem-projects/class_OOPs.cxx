#include<iostream>
#include<string>
using namespace std;

class OOPs {  // base class
private:
    string core_dynamics;
public:
    string methods;
    string constructors;
    string objects;

    void set_cored(string cd1, string cd2, string cd3) {  //setter
        core_dynamics = {cd1 + "," + cd2 + "," + cd3};  //getter
    }

    OOPs(string m, string c, string o) {  // constructor
        methods = m;
        constructors = c;
        objects = o;
    }

    void getinfo() {  // method
    cout << "Core dynamics: " << core_dynamics << endl;
        cout << "Here are methods: " << methods << endl;
        cout << "Here are constructors: " << constructors << endl;
        cout << "Here are objects: " << objects << endl;
    }
};

int main() {  //main function
    OOPs obj_1("method1", "constructor1", "object1");  //object
    
        obj_1.set_cored("Inheritance","Polymorphism","Encapsulation");
    obj_1.getinfo();  //function call

    return 0;
}