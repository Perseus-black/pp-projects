#include<iostream>
using namespace std;

class Grandfather {
public:
    string grandfather_name;
    string family_name;

    Grandfather() {
        cout << "Enter Grandfather's Name: ";
        getline(cin, grandfather_name);

        cout << "Enter Family Name: ";
        getline(cin, family_name);
    }
};

class Father : public Grandfather {
public:
    string father_name;

    Father() {
        cout << "Enter Father's Name: ";
        getline(cin, father_name);
    }
};

class Son : public Father {
public:
    string son_name;

    Son() {
        cout << "Enter Son's Name: ";
        getline(cin, son_name);
    }
};

int main() {
    Son s;
    
    cout << endl;
    cout << s.grandfather_name << " " << s.family_name << endl;
    cout << s.father_name << " " << s.family_name << endl;
    cout << s.son_name << " " << s.family_name << endl;
    
    return 0;
}