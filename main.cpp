#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits> // for formatting

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

using namespace std;

bool confirmAction(string message);


class Book {
public:
    int id;
    string title, author, publisher, genre;
    float price;
    int quantity;

    // Input book details
    void input() {
        cout << "\nEnter Book ID: ";
        cin >> id;
        cin.ignore();
        cout << "Enter Title: ";
        getline(cin, title);
        cout << "Enter Author: ";
        getline(cin, author);
        cout << "Enter Publisher: ";
        getline(cin, publisher);
        cout << "Enter Genre: ";
        getline(cin, genre);
        cout << "Enter Price: ";
        cin >> price;
        cout << "Enter Quantity: ";
        cin >> quantity;
    }

    // Display book details
    void display() {
        cout << left << setw(6) << id << setw(25) << title << setw(20) << author
             << setw(20) << publisher << setw(10) << genre << setw(8) << price
             << setw(6) << quantity << endl;
    }
};


//Add book to file

void addBook() {
    Book b;
    b.input();

    ofstream out("books.txt", ios::app); // open in append mode
    if (out.is_open()) {
        out << b.id << "|" << b.title << "|" << b.author << "|" << b.publisher
            << "|" << b.genre << "|" << b.price << "|" << b.quantity << "\n";
        out.close();
        cout << "\n Book added successfully!\n";
    } else {
        cout << "\n Error opening file.\n";
    }
}



//View all books



void viewBooks() {
    ifstream in("books.txt");
    Book b;
    string line;

    cout << "\n All Books:\n";
    cout << left << setw(6) << "ID" << setw(25) << "Title" << setw(20) << "Author"
         << setw(20) << "Publisher" << setw(10) << "Genre" << setw(8) << "Price"
         << setw(6) << "Qty" << "\n";
    cout << "---------------------------------------------------------------------------------------\n";

    while (getline(in, line)) {
        stringstream ss(line);
string priceStr, quantityStr;

getline(ss, line, '|');
b.id = stoi(line);
getline(ss, b.title, '|');
getline(ss, b.author, '|');
getline(ss, b.publisher, '|');
getline(ss, b.genre, '|');
getline(ss, priceStr, '|');
getline(ss, quantityStr);

b.price = stof(priceStr);
b.quantity = stoi(quantityStr);

b.display();
    }

    in.close();
}



//Search for a book b ID



void searchBook() {
    int targetID;
    cout << "\nEnter Book ID to search: ";
    cin >> targetID;

    ifstream in("books.txt");
    string line;
    bool found = false;

    while (getline(in, line)) {
        stringstream ss(line);
        Book b;
        string priceStr, quantityStr;

        getline(ss, line, '\n');
        stringstream ls(line);
        getline(ls, line, '|');
        b.id = stoi(line);
        getline(ls, b.title, '|');
        getline(ls, b.author, '|');
        getline(ls, b.publisher, '|');
        getline(ls, b.genre, '|');
        getline(ls, priceStr, '|');
        getline(ls, quantityStr);

        b.price = stof(priceStr);
        b.quantity = stoi(quantityStr);

        if (b.id == targetID) {
            cout << "\n Book Found:\n";
            b.display();
            found = true;
            break;
        }
    }

    if (!found)
        cout << "\n Book ID not found.\n";

    in.close();
}



//Delete book by ID

void deleteBook() {
    int targetID;
    cout << "\nEnter Book ID to delete: ";
    cin >> targetID;
    if(!confirmAction("Are you sure you want to delete this book ?")){
        cout<<"Deletion cancelled.\n";
        return;
    }

    ifstream in("books.txt");
    ofstream out("temp.txt");

    string line;
    bool deleted = false;

    while (getline(in, line)) {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, '|');
        if (stoi(idStr) != targetID) {
            out << line << "\n";
        } else {
            deleted = true;
        }
    }

    in.close();
    out.close();

    remove("books.txt");
    rename("temp.txt", "books.txt");

    if (deleted)
        cout << "\n Book deleted successfully!\n";
    else
        cout << "\n Book not found.\n";
}

//Login function



bool login() {
    string username, password;
    string storedUser, storedPass;
    bool success = false;

    cout << "\n=====  Admin Login =====\n";
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password; // We'll add password masking later

    ifstream file("users.txt");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, storedUser, '|');
        getline(ss, storedPass, '\n');

        if (username == storedUser && password == storedPass) {
            success = true;
            break;
        }
    }

    file.close();

    if (success) {
        cout << "\n Login successful. Welcome, " << username << "!\n";
        return true;
    } else {
        cout << "\n Login failed. Incorrect username or password.\n";
        return false;
    }
}




//Defining the IssueBook function 


#include <ctime> // For date/time functions

void issueBook() {
    int bookId;
    string borrower;

    cout << "\n--- Issue Book ---\n";
    cout << "Enter Book ID to issue: ";
    cin >> bookId;
    cin.ignore(); // To consume leftover newline

    cout << "Enter Borrower Name: ";
    getline(cin, borrower);

    // Open books file to find the book and check quantity
    ifstream inFile("books.txt");
    ofstream tempFile("temp.txt");
    string line;
    bool bookFound = false;
    bool issued = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, title, author, publisher, genre, priceStr, qtyStr;
        
        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, publisher, '|');
        getline(ss, genre, '|');
        getline(ss, priceStr, '|');
        getline(ss, qtyStr, '\n');

        int id = stoi(idStr);
        int qty = stoi(qtyStr);

        if (id == bookId) {
            bookFound = true;
            if (qty > 0) {
                qty--;
                issued = true;

                // Get current date as string YYYY-MM-DD
                time_t now = time(0);
                tm *ltm = localtime(&now);
                char dateStr[11];
                sprintf(dateStr, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);

                // Append issue record to issues.txt
                ofstream issueFile("issues.txt", ios::app);
                issueFile << bookId << "|" << borrower << "|" << dateStr << "\n";
                issueFile.close();

                cout << "Book issued successfully to " << borrower << " on " << dateStr << ".\n";
            } else {
                cout << "Sorry, book is out of stock.\n";
            }
        }

        // Write updated book record (with updated qty) to temp file
        tempFile << id << "|" << title << "|" << author << "|" << publisher << "|" << genre << "|" << priceStr << "|" << qty << "\n";
    }

    inFile.close();
    tempFile.close();

    if (!bookFound) {
        cout << "Book ID not found.\n";
        remove("temp.txt"); // Clean up temp file
        return;
    }

    if (issued) {
        // Replace books.txt with temp.txt to update quantity
        remove("books.txt");
        rename("temp.txt", "books.txt");
    } else {
        remove("temp.txt"); // no change needed
    }
}



//Defining the ReturnBook function



void returnBook() {
    int bookId;
    string borrower;

    cout << "\n--- Return Book ---\n";
    cout << "Enter Book ID to return: ";
    cin >> bookId;
    cin.ignore();

    cout << "Enter Borrower Name: ";
    getline(cin, borrower);

    // Read issues.txt and remove the record of this borrower + bookId if found
    ifstream issueFile("issues.txt");
    ofstream tempIssue("temp_issues.txt");
    string line;
    bool found = false;

    while (getline(issueFile, line)) {
        stringstream ss(line);
        string idStr, name, date;

        getline(ss, idStr, '|');
        getline(ss, name, '|');
        getline(ss, date, '\n');

        int id = stoi(idStr);

        if (id == bookId && name == borrower) {
            found = true; // skip writing this line, i.e., remove record
        } else {
            tempIssue << line << "\n";
        }
    }

    issueFile.close();
    tempIssue.close();

    if (!found) {
        cout << "No record found for this book and borrower.\n";
        remove("temp_issues.txt");
        return;
    }

    // Replace issues.txt with updated file
    remove("issues.txt");
    rename("temp_issues.txt", "issues.txt");

    // Now increase quantity of the returned book in books.txt
    ifstream inFile("books.txt");
    ofstream tempFile("temp_books.txt");

    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, title, author, publisher, genre, priceStr, qtyStr;

        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, publisher, '|');
        getline(ss, genre, '|');
        getline(ss, priceStr, '|');
        getline(ss, qtyStr, '\n');

        int id = stoi(idStr);
        int qty = stoi(qtyStr);

        if (id == bookId) {
            qty++; // increase quantity
        }

        tempFile << id << "|" << title << "|" << author << "|" << publisher << "|" << genre << "|" << priceStr << "|" << qty << "\n";
    }

    inFile.close();
    tempFile.close();

    remove("books.txt");
    rename("temp_books.txt", "books.txt");

    cout << "Book returned successfully.\n";
}



//printMenu()


void printMenu() {
    cout << "\n====================================\n";
    cout << "       Library Management System       \n";
    cout << "====================================\n";
    cout << "1. Add Book\n";
    cout << "2. View All Books\n";
    cout << "3. Search Book by ID\n";
    cout << "4. Delete Book\n";
    cout << "5. Exit\n";
    cout << "6. Issue Book\n";
    cout << "7. Return Book\n";
    cout << "====================================\n";
    cout << "Enter your choice: ";
}





// getChoice()



int getChoice() {
    int choice;
    while (true) {
        if (cin >> choice) {               // input succeeded
            if (choice >= 1 && choice <= 7) {
                break;                     // valid choice
            } else {
                cout << " Please enter a valid choice (1-7): ";
            }
        } else {
            cout << " Invalid input. Please enter a number: ";
            cin.clear();                   // clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // discard bad input
        }
    }
    return choice;
}



//Reusable confirmation function



bool confirmAction(string message) {
    char ans;
    cout << message << " (y/n): ";
    cin >> ans;
    ans = tolower(ans);

    // Clear input buffer to avoid issues
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return (ans == 'y');
}


//clear screen 


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}




//Main menu


int main() {

     const int maxAttempts = 3;
    int attempts = 0;
    bool loggedIn = false;

    while (attempts < maxAttempts && !loggedIn) {
        loggedIn = login();
        if (!loggedIn) {
            attempts++;
            if (attempts < maxAttempts) {
                cout << "Try again.\n";
            } else {
                cout << "Maximum login attempts exceeded. Exiting...\n";
                return 0; // exit program
            }
        }
    }
    
    

    int choice;
    do {
        clearScreen();
        printMenu();
        choice = getChoice();

        switch (choice) {
        case 1: addBook(); break;
        case 2: viewBooks(); break;
        case 3: searchBook(); break;
        case 4: deleteBook(); break;
        case 5: cout << "\n Exiting...\n"; break;
        case 6: issueBook(); break;
        case 7: returnBook(); break;
        default: cout << "\n❗ Invalid choice. Try again.\n";
        }
        if (choice != 5){
            cout<<"\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    } while (choice != 5);

    return 0;
}

