#include<iostream>
using namespace std;

class Phone {
    private:
    int IMEI = 0;
    
    public:
    string name;
    string model;

void phone_details() {
    
    cout << "Phone name: " << name << endl;
    cout << "Phone model: " << model << endl;
    
    }
};

int main() {
    Phone myCell;
    
    myCell.name = "iPhone X";
    myCell.model = "VX9345";
    
    myCell.phone_details();
    
    return 0;
}