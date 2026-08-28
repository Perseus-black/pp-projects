#include <iostream>
using namespace std;

class Bank_acc {
private:
    string acc_holder;
    double balance;
    
public:
    Bank_acc(string name, double startBalance) {
        acc_holder = name;
        balance = startBalance;
    }
    
    void deposit(double amount) {
        balance += amount;
    }
    
    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            cout << "Insufficient Funds!" << endl;
        }
    }
    void display() {
        cout << "Account Holder: " << acc_holder << endl;
        cout << "Balance: " << balance << endl;
    }
};

int main() {
    Bank_acc acc("Ghost", 2000);
    
    acc.deposit(600);
    acc.withdraw(1200);
    acc.display();
    
    return 0;
}