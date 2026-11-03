#include <iostream>
#include <string>
#include <limits>

struct Book {
    int id;
    std::string title;
    std::string author;
    bool isIssued;
};

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        } else {
            std::cout << "Invalid input! Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

std::string readString(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!value.empty()) {
            return value;
        }
        std::cout << "Input cannot be empty.\n";
    }
}

int main() {
    const int MAX_BOOKS = 100;
    Book library[MAX_BOOKS];
    int bookCount = 0;

    while (true) {
        std::cout << "\n--- Library Management System ---\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. Display Books\n";
        std::cout << "3. Issue Book\n";
        std::cout << "4. Return Book\n";
        std::cout << "5. Search Book\n";
        std::cout << "6. Exit\n";
        std::cout << "Enter your choice: ";

        int choice;
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input! Please enter a valid menu number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            if (bookCount >= MAX_BOOKS) {
                std::cout << "Library full!\n";
                continue;
            }

            Book newBook;
            newBook.id = readInt("Enter Book ID: ");
            newBook.title = readString("Enter Book Title: ");
            newBook.author = readString("Enter Author Name: ");
            newBook.isIssued = false;

            bool duplicate = false;
            for (int i = 0; i < bookCount; i++) {
                if (library[i].id == newBook.id) {
                    duplicate = true;
                    break;
                }
            }

            if (duplicate) {
                std::cout << "Book ID already exists.\n";
            } else {
                library[bookCount++] = newBook;
                std::cout << "Book added successfully.\n";
            }
        }
        else if (choice == 2) {
            if (bookCount == 0) {
                std::cout << "No books in library.\n";
            } else {
                std::cout << "\n--- Books in Library ---\n";
                for (int i = 0; i < bookCount; i++) {
                    std::cout << "ID: " << library[i].id
                              << ", Title: " << library[i].title
                              << ", Author: " << library[i].author
                              << ", Status: " << (library[i].isIssued ? "Issued" : "Available")
                              << std::endl;
                }
            }
        }
        else if (choice == 3) {
            int id = readInt("Enter Book ID to issue: ");
            bool found = false;
            for (int i = 0; i < bookCount; i++) {
                if (library[i].id == id) {
                    found = true;
                    if (library[i].isIssued) {
                        std::cout << "Book already issued.\n";
                    } else {
                        library[i].isIssued = true;
                        std::cout << "Book issued successfully.\n";
                    }
                    break;
                }
            }
            if (!found) {
                std::cout << "Book not found.\n";
            }
        }
        else if (choice == 4) {
            int id = readInt("Enter Book ID to return: ");
            bool found = false;
            for (int i = 0; i < bookCount; i++) {
                if (library[i].id == id) {
                    found = true;
                    if (library[i].isIssued) {
                        library[i].isIssued = false;
                        std::cout << "Book returned successfully.\n";
                    } else {
                        std::cout << "Book was not issued.\n";
                    }
                    break;
                }
            }
            if (!found) {
                std::cout << "Book not found.\n";
            }
        }
        else if (choice == 5) {
            std::cout << "Search by: 1. ID  2. Title\n";
            int searchChoice = readInt("Enter choice: ");

            if (searchChoice == 1) {
                int id = readInt("Enter Book ID: ");
                bool found = false;
                for (int i = 0; i < bookCount; i++) {
                    if (library[i].id == id) {
                        found = true;
                        std::cout << "ID: " << library[i].id
                                  << ", Title: " << library[i].title
                                  << ", Author: " << library[i].author
                                  << ", Status: " << (library[i].isIssued ? "Issued" : "Available")
                                  << std::endl;
                        break;
                    }
                }
                if (!found) {
                    std::cout << "Book not found.\n";
                }
            } else if (searchChoice == 2) {
                std::string title = readString("Enter Book Title: ");
                bool found = false;
                for (int i = 0; i < bookCount; i++) {
                    if (library[i].title == title) {
                        found = true;
                        std::cout << "ID: " << library[i].id
                                  << ", Title: " << library[i].title
                                  << ", Author: " << library[i].author
                                  << ", Status: " << (library[i].isIssued ? "Issued" : "Available")
                                  << std::endl;
                    }
                }
                if (!found) {
                    std::cout << "Book not found.\n";
                }
            } else {
                std::cout << "Invalid choice.\n";
            }
        }
        else if (choice == 6) {
            std::cout << "Exiting system...\n";
            break;
        }
        else {
            std::cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}