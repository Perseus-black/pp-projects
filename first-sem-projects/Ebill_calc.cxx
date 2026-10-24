#include <iostream>
#include <iomanip>
#include <cmath>

int main() {
    std::string consumerName;
    int consumerID;
    double units;
    char choice;

    do {
        // Input consumer details
        std::cout << "Enter Consumer Name, ID & Units Consumed:\n";
        std::cin >> consumerName >> consumerID >> units;

        // Calculate bill
        double bill {0.0};

        if (units <= 100)
            bill = units * 5;          // Slab 1: 0-100 units @5/unit
        else if (units <= 200)
            bill = 100 * 5 + (units - 100) * 7;      // Slab 2: 101-200 units @7/unit
        else if (units <= 500)
            bill = 100 * 5 + 100 * 7 + (units - 200) * 10;       // Slab 3: 201-500 units @10/unit
        else
            bill = 100 * 5 + 100 * 7 + 300 * 10 + (units - 500) * 15;       // Slab 4: >500 units @15/unit

        // Display bill summary
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "\n--- Electricity Bill Summary ---\n";
        std::cout << "Consumer Name: " << consumerName << "\nConsumer ID: " << consumerID << "\nUnits Consumed: " << units << "\nTotal Bill: Rs. " << bill << std::endl;

        // Ask if user wants to calculate for another consumer
        std::cout << "\nDo you want to calculate for another consumer? (Y/N):\n";
        std::cin >> choice;

    } while (choice == 'Y' || choice == 'y');

    std::cout << "\nThank you for using the Electricity Bill Calculator!\n";
    return 0;
}