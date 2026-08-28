#include<iostream>
#include<cstring>
#include<string>
using namespace std;

class ATM {
    private: //inaccessable by others
    double acc_num;
    int acc_bal;
    int p_code;
    public:
    string acc_hName;

    void s_data(double an, int ab, int pc) { //const for private data (show data)
        acc_num = an;
        acc_bal = ab;
        p_code = pc;
    }

    bool r_data(string acc_hName) { //const for revealing data
        int ent_pin;

        do {
            cout << "Enter PIN Code: ";
            cin >> ent_pin;

        if(ent_pin == p_code) {
            cout << "\nLogged in.\n" << endl;
            {
        cout << "Account Holder: " << acc_hName << endl;
        cout << "Account Number: " << acc_num << endl;
        cout << "Account Balance: " << acc_bal << "Rs" << endl;
        
        return true;
        }
    } else {
                cout << "Invalid PIN, Please Try Again!\n" << endl;
            }

        } while (ent_pin != p_code);

        return false;
    }
        double getBal() {
        return acc_bal;
    }

    void setBal(double balance) {
        acc_bal = balance;
    }
};

class Account : public ATM {
    public:
    int choice;
    double amount;
    int t_accNum; //Acoount To Transfer Funds
    
    void tray() {
        
        do {
        cout << "\n1. Deposit\n";
        cout << "2. Withdraw\n";
        cout << "3. Transfer\n";
        cout << "4. Exit\n " << endl;
        cout << "Select option: ";
        cin >> choice;

        switch(choice) {
        case 1:
        cout << "Enter amount to deposit: " << endl;
            cin >> amount;

            if (amount > 0) {
                setBal(getBal() + amount);
                cout << "Funds deposited successfully. " << endl;   //refreshes Balance
                cout << "New Balance: " << getBal() << " Rs.\n";
            } else {
            cout << "Invalid deposit amount.\n" << endl;
        }
        break;
        
        case 2:
        cout << "Enter amount to withdraw: " << endl;
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount.\n" << endl;
            } else if (amount > getBal()) {
                cout << "Insufficient funds to withdraw.\n" << endl;
            } else {
                setBal(getBal() - amount);

                cout << "Withdrawal successful.\n" << endl;
                cout << "New Balance: " << getBal() << " Rs.\n";
            }
            break;
            
        case 3:
        cout << "Enter account number: ";
        cin >> t_accNum;

        cout << "Enter amount to tranfer. ";
        cin >> amount;

        if (amount <= 0) {
            cout << "Invalid amount.\n";
        } else if (amount > getBal()) {
            cout << "Insufficient funds to tranfer.\n";
        } else {
            setBal(getBal() - amount);

            cout << "Funds transfered successfully.\n";
            cout << "New Balance: " << getBal() << " Rs.\n";
            }
            break;
                    
            case 4:
                cout << "Thank you for using ATM.\n";
                break;
                
                default:
                cout << "Invalid choice.\n";
            }
        }
        while (choice != 4);
    }
}; 

int main() {
    Account ac;
    ac.s_data(1, 2000, 3991);
    if(ac.r_data("Ghost")) {
    ac.tray();
    }

    return 0;
}