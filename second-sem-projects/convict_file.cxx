#include<iostream>
using namespace std;

class Convict {
private:
    string con_crm;
    
public:
    int con_num;
    int con_room;
    string con_id;
    
    Convict(int num, int room, string id) {
        con_num = num;
        con_room = room;
        con_id = id;
        con_crm = "Unknown";
    }
     
    void Setcon_crm(string c) {
        con_crm = c;
    }
    
    void show_details() {
        cout << "Convict No: " << con_num << endl;
        cout << "Convict Room No: " << con_room << endl;
        cout << "Convict Reg ID: " << con_id << endl;
        
    }
};

int main() {
    Convict c1(4530, 137, "Diamond");
    Convict c2(5597, 112, "Silver");
    
    c1.Setcon_crm("High Risk!");
    c2.Setcon_crm("Low Risk!");
    
    c1.show_details();
    c2.show_details();
    
    return 0;
}