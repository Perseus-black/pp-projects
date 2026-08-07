#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    string consumerName;
    int consumerID;
    double units;
    char choice;

    do {
        // Input consumer details
        cout << "Enter Consumer Name: ";
        cin >> consumerName;
        cout << "Enter Consumer ID: ";
        cin >> consumerID;
        cout << "Enter Units Consumed: ";
        cin >> units;

        // Calculate bill
        double bill = 0.0;

        if (units <= 100)
            bill = units * 5;          // Slab 1: 0-100 units @5/unit
        else if (units <= 200)
            bill = 100 * 5 + (units - 100) * 7; // Slab 2: 101-200 units @7/unit
        else if (units <= 500)
            bill = 100 * 5 + 100 * 7 + (units - 200) * 10; // Slab 3: 201-500 units @10/unit
        else
            bill = 100 * 5 + 100 * 7 + 300 * 10 + (units - 500) * 15; // Slab 4: >500 units @15/unit

        // Display bill summary
        cout << fixed << setprecision(2);
        cout << "\n--- Electricity Bill Summary ---\n";
        cout << "Consumer Name: " << consumerName << endl;
        cout << "Consumer ID: " << consumerID << endl;
        cout << "Units Consumed: " << units << endl;
        cout << "Total Bill: Rs. " << bill << endl;

        // Ask if user wants to calculate for another consumer
        cout << "\nDo you want to calculate for another consumer? (Y/N): ";
        cin >> choice;

    } while (choice == 'Y' || choice == 'y');

    cout << "\nThank you for using the Electricity Bill Calculator!\n";
    return 0;
}