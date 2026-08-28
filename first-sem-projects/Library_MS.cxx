#include <iostream>
#include <string>
using namespace std;

struct Book {
    int id;
    string title;
    string author;
    bool isIssued;
};

int main() {
    const int MAX_BOOKS = 100;
    Book library[MAX_BOOKS];
    int bookCount = 0;
    int choice;

    do {
        cout << "\n--- Library Management System ---\n";
        cout << "1. Add Book\n2. Display Books\n3. Issue Book\n4. Return Book\n5. Search Book\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // to clear the newline from buffer

        switch(choice) {
            case 1: {
                if (bookCount < MAX_BOOKS) {
                    cout << "Enter Book ID: ";
                    cin >> library[bookCount].id;
                    cin.ignore();
                    cout << "Enter Book Title: ";
                    getline(cin, library[bookCount].title);
                    cout << "Enter Author Name: ";
                    getline(cin, library[bookCount].author);
                    library[bookCount].isIssued = false;
                    bookCount++;
                    cout << "Book added successfully.\n";
                } else {
                    cout << "Library full!\n";
                }
                break;
            }

            case 2: {
                if (bookCount == 0) {
                    cout << "No books in library.\n";
                } else {
                    cout << "\n--- Books in Library ---\n";
                    for (int i = 0; i < bookCount; i++) {
                        cout << "ID: " << library[i].id << ", Title: " << library[i].title
                             << ", Author: " << library[i].author
                             << ", Status: " << (library[i].isIssued ? "Issued" : "Available") << endl;
                    }
                }
                break;
            }

            case 3: {
                int id;
                cout << "Enter Book ID to issue: ";
                cin >> id;
                bool found = false;
                for (int i = 0; i < bookCount; i++) {
                    if (library[i].id == id) {
                        found = true;
                        if (!library[i].isIssued) {
                            library[i].isIssued = true;
                            cout << "Book issued successfully.\n";
                        } else {
                            cout << "Book already issued.\n";
                        }
                        break;
                    }
                }
                if (!found) cout << "Book not found.\n";
                break;
            }

            case 4: {
                int id;
                cout << "Enter Book ID to return: ";
                cin >> id;
                bool found = false;
                for (int i = 0; i < bookCount; i++) {
                    if (library[i].id == id) {
                        found = true;
                        if (library[i].isIssued) {
                            library[i].isIssued = false;
                            cout << "Book returned successfully.\n";
                        } else {
                            cout << "Book was not issued.\n";
                        }
                        break;
                    }
                }
                if (!found) cout << "Book not found.\n";
                break;
            }

            case 5: {
                int searchChoice;
                cout << "Search by: 1. ID  2. Title\nEnter choice: ";
                cin >> searchChoice;
                cin.ignore();
                if (searchChoice == 1) {
                    int id;
                    cout << "Enter Book ID: ";
                    cin >> id;
                    bool found = false;
                    for (int i = 0; i < bookCount; i++) {
                        if (library[i].id == id) {
                            found = true;
                            cout << "ID: " << library[i].id << ", Title: " << library[i].title
                                 << ", Author: " << library[i].author
                                 << ", Status: " << (library[i].isIssued ? "Issued" : "Available") << endl;
                            break;
                        }
                    }
                    if (!found) cout << "Book not found.\n";
                } else if (searchChoice == 2) {
                    string title;
                    cout << "Enter Book Title: ";
                    getline(cin, title);
                    bool found = false;
                    for (int i = 0; i < bookCount; i++) {
                        if (library[i].title == title) {
                            found = true;
                            cout << "ID: " << library[i].id << ", Title: " << library[i].title
                                 << ", Author: " << library[i].author
                                 << ", Status: " << (library[i].isIssued ? "Issued" : "Available") << endl;
                        }
                    }
                    if (!found) cout << "Book not found.\n";
                } else {
                    cout << "Invalid choice.\n";
                }
                break;
            }

            case 6:
                cout << "Exiting system...\n";
                break;

            default:
                cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 6);

    return 0;
}