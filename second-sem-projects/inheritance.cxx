#include<iostream>
using namespace std;

class Grandfather {
public:
    string family_name;
};

class Father : public Grandfather {
public:
    string father_name;
};

class Son : public Father {
public:
    string son_name;
};

int main() {
    Son s;
    
    cout << s.family_name << "Family Name: Syed" << endl;
    cout << s.father_name << "Father Name: Ali" << endl;
    cout << s.son_name << "Son's Name: Ghost" << endl;
    
    return 0;
}