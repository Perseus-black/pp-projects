#include <iostream>
using namespace std;

// Base Class
class Person {
protected:
    int id;
    string name;

public:
    Person(int i, string n) {
        id = i;
        name = n;
    }

    virtual void display() {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
    }

    virtual ~Person() {}
};

// Derived Class, Inheritance
class Doctor : public Person {
private:
    string specialization;

public:
    Doctor(int i, string n, string s)
        : Person(i, n) {

        specialization = s;
    }

    void display() override {

        cout << "\n--- Doctor Record ---\n";

        Person::display();

        cout << "Specialization: " << specialization << endl;
    }

    string getName() {
        return name;
    }
};

// Sub Class
class Patient : public Person {
private:
    int age;
    string disease;

public:
    Patient(int i, string n, int a, string d)
        : Person(i, n) {

        age = a;
        disease = d;
    }

    void display() override {

        cout << "\n--- Patient Record ---\n";

        Person::display();

        cout << "Age: " << age << endl;
        cout << "Disease: " << disease << endl;
    }

    string getName() {
        return name;
    }
};

// Base Class 02
class Appointment {
private:
    string patientName;
    string doctorName;
    string date;

public:
    Appointment(string p, string d, string dt) {

        patientName = p;
        doctorName = d;
        date = dt;
    }

    void showAppointment() {

        cout << "\n--- Appointment Details ---\n";

        cout << "Patient: " << patientName << endl;
        cout << "Doctor: " << doctorName << endl;
        cout << "Date: " << date << endl;
    }
};

// Base Class 03
class Billing {
private:
    string patientName;
    double amount;

public:
    Billing(string p, double a) {

        patientName = p;
        amount = a;
    }

    void showBill() {

        cout << "\n--- Billing Details ---\n";

        cout << "Patient: " << patientName << endl;
        cout << "Total Amount: $" << amount << endl;
    }
};

int main() {

// Patient Input
    int patientID, age;
    string patientName, disease;

    cout << "Enter Patient ID: ";
    cin >> patientID;

    cin.ignore();

    cout << "Enter Patient Name: ";
    getline(cin, patientName);

    cout << "Enter Age: ";
    cin >> age;

    cin.ignore();

    cout << "Enter Disease: ";
    getline(cin, disease);

// Doctor Input
    int doctorID;
    string doctorName, specialization;

    cout << "\nEnter Doctor ID: ";
    cin >> doctorID;

    cin.ignore();

    cout << "Enter Doctor Name: ";
    getline(cin, doctorName);

    cout << "Enter Specialization: ";
    getline(cin, specialization);

// Appointment Input
    string date;

    cout << "Enter Appointment Date: ";
    getline(cin, date);

// Billing Input
    double amount;

    cout << "Enter Bill Amount: ";
    cin >> amount;

// Objects
    Patient p1(patientID, patientName, age, disease);

    Doctor d1(doctorID, doctorName, specialization);

    Appointment a1(
        patientName,
        doctorName,
        date
    );

    Billing b1(
        patientName,
        amount
    );

// Polymorphism
    Person* personPtr;

    personPtr = &p1;
    personPtr->display();

    personPtr = &d1;
    personPtr->display();

// Appointment Details
    a1.showAppointment();

// Bill
    b1.showBill();

    return 0;
}