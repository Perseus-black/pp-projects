#include <iostream>
#include <fstream>
using namespace std;

/* --- ABSTRACT CLASS --- */
class Renter {
protected:
    string name;
    string phone;

public:
    virtual void inputInfo() = 0;
    virtual void showInfo() const = 0;
};

/* --- BASE CLASS --- */
class Car {
protected:
    string carName;
    int rentPerDay;

public:
    Car(string n = "", int r = 0) {
        carName = n;
        rentPerDay = r;
    }

    virtual void showCar() const {
        cout << carName << " - $" << rentPerDay << "/day\n";
    }

    string getName() const {
        return carName;
    }

    int getRent() const {
        return rentPerDay;
    }
};

/* --- DERIVED CLASS --- */
class SportsCar : public Car {
public:
    SportsCar(string n, int r)
        : Car(n, r) {}
};

/* --- RENTAL SYSTEM --- */
class RentalSystem : public Renter {
private:
    SportsCar cars[5] = {
        SportsCar("Porsche 911 GT3 RS", 650),
        SportsCar("Nissan GT-R R34", 550),
        SportsCar("BMW M3 CS", 600),
        SportsCar("Ford Mustang GTD", 750),
        SportsCar("Audi RS6 GT", 700)
    };

    string selectedCar;
    int selectedRent;
    int days;
    int totalBill;

public:

/* --- CUSTOMER INFO --- */
    void inputInfo() override {

        cin.ignore();

        cout << "\nEnter Customer Name: ";
        getline(cin, name);

        cout << "Enter Phone Number: ";
        getline(cin, phone);
    }

    void showInfo() const override {

        cout << "\n--- Customer Information ---\n";
        cout << "Name: " << name << endl;
        cout << "Phone: " << phone << endl;
    }

/* --- BOOK CAR --- */
    void bookCar() {

        cout << "\n--- Available Cars ---\n";

        for (int i = 0; i < 5; i++) {
            cout << i + 1 << ". ";
            cars[i].showCar();
        }

        int choice;

        cout << "\nSelect Car (1-5): ";
        cin >> choice;

        if (choice < 1 || choice > 5) {
            cout << "Invalid choice.\n";
            return;
        }

        cout << "Enter Rental Days: ";
        cin >> days;

        if (days <= 0) {
            cout << "Invalid days.\n";
            return;
        }

        selectedCar = cars[choice - 1].getName();
        selectedRent = cars[choice - 1].getRent();

        totalBill = selectedRent * days;

        cout << "\nCar booked successfully.\n";
    }

/* --- BILL --- */
    void showBill() const {

        cout << "\n===== BILL =====\n";

        showInfo();

        cout << "Car: " << selectedCar << endl;
        cout << "Days: " << days << endl;
        cout << "Total Bill: $" << totalBill << endl;
    }

/* --- SAVE --- */
    void saveBooking() {

        ofstream file("bookings.txt", ios::app);

        file << "Customer Name: " << name << endl;
        file << "Phone Number: " << phone << endl;
        file << "Car: " << selectedCar << endl;
        file << "Days: " << days << endl;
        file << "Total Bill: $" << totalBill << endl;
        file << "------------------------\n";

        file.close();

        cout << "\nBooking saved successfully.\n";
    }

/* --- VIEW RECORDS --- */
    void viewRecords() {

        ifstream file("bookings.txt");

        if (!file) {
            cout << "No records found.\n";
            return;
        }

        string line;

        cout << "\n===== RECORDS =====\n";

        while (getline(file, line)) {
            cout << line << endl;
        }

        file.close();
    }

/* --- CLEAR RECORDS --- */
    void clearRecords() {

        ofstream file("bookings.txt", ios::trunc);

        file.close();

        cout << "All records cleared.\n";
    }
};

/* --- ADMIN ACCESS --- */
bool admin() {

    string pin;

    cout << "Enter Admin PIN: ";
    cin >> pin;

    return pin == "9137";
}

/* --- MAIN --- */
int main() {

    RentalSystem system;

    int choice;

    do {

        cout << "\n===== CAR RENTAL SYSTEM =====\n";
        cout << "1. New Booking\n";
        cout << "2. View Records (Admin)\n";
        cout << "3. Clear Records (Admin)\n";
        cout << "4. Exit\n";

        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            system.inputInfo();

            system.bookCar();

            system.showBill();

            system.saveBooking();

            break;

        case 2:

            if (admin())
                system.viewRecords();
            else
                cout << "Access Denied.\n";

            break;

        case 3:

            if (admin())
                system.clearRecords();
            else
                cout << "Access Denied.\n";

            break;

        case 4:

            cout << "Program Closed.\n";
            break;

        default:

            cout << "Invalid Choice.\n";
        }

    } while (choice != 4);

    return 0;
}