#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <limits>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

// ================= Colors =================
const string RESET = "\033[0m";
const string RED = "\033[1;31m";
const string GREEN = "\033[1;32m";
const string YELLOW = "\033[1;33m";
const string BLUE = "\033[1;34m";
const string CYAN = "\033[1;36m";
const string MAGENTA = "\033[1;35m";

// ================= Book Class =================
class Book {
private:
    int id;
    string title;
    string author;
    bool isBorrowed;
    string dueDate;

public:
    Book(int id, string title, string author)
        : id(id), title(title), author(author), isBorrowed(false), dueDate("") {}

    int getId() const { return id; }
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    bool getIsBorrowed() const { return isBorrowed; }
    string getDueDate() const { return dueDate; }

    void setTitle(const string &newTitle) { title = newTitle; }
    void setAuthor(const string &newAuthor) { author = newAuthor; }

    void borrow(string due) {
        isBorrowed = true;
        dueDate = due;
    }

    void returnBook() {
        isBorrowed = false;
        dueDate = "";
    }

    bool isOverdue() const {
        if (!isBorrowed || dueDate.empty()) return false;
        time_t now = time(0);
        tm tm_due = {};
        istringstream ss(dueDate);
        ss >> get_time(&tm_due, "%Y-%m-%d");
        time_t due_time = mktime(&tm_due);
        return now > due_time;
    }

    void display() const {
        cout << CYAN << "ID: " << RESET << id << "\n";
        cout << CYAN << "Title: " << RESET << title << "\n";
        cout << CYAN << "Author: " << RESET << author << "\n";
        cout << CYAN << "Status: " << RESET
             << (isBorrowed ? (YELLOW + "Borrowed (Due: " + dueDate + ")" + RESET)
                            : (GREEN + "Available" + RESET)) << "\n";
        if (isOverdue()) {
            cout << RED << "OVERDUE!\n" << RESET;
        }
        cout << "-------------------------\n";
    }

    string serialize() const {
        return to_string(id) + "|" + title + "|" + author + "|" +
               (isBorrowed ? "1" : "0") + "|" + dueDate;
    }

    static Book deserialize(const string &data) {
        stringstream ss(data);
        string idStr, title, author, borrowedStr, due;
        getline(ss, idStr, '|');
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, borrowedStr, '|');
        getline(ss, due);

        Book b(stoi(idStr), title, author);
        if (borrowedStr == "1") b.borrow(due);
        return b;
    }
};

// ================= Transaction Class =================
class Transaction {
private:
    int bookId;
    string action;
    string date;

public:
    Transaction(int bookId, string action, string date)
        : bookId(bookId), action(action), date(date) {}

    void display() const {
        cout << MAGENTA << "Book ID: " << RESET << bookId
             << ", Action: " << action
             << ", Date: " << date << "\n";
    }

    string serialize() const {
        return to_string(bookId) + "|" + action + "|" + date;
    }

    static Transaction deserialize(const string &data) {
        stringstream ss(data);
        string idStr, action, date;
        getline(ss, idStr, '|');
        getline(ss, action, '|');
        getline(ss, date);
        return Transaction(stoi(idStr), action, date);
    }
};

// ================= Global Containers =================
vector<Book> books;
vector<Transaction> transactions;

// ================= File Names =================
const string BOOKS_FILE = "books.txt";
const string TRANSACTIONS_FILE = "transactions.txt";
const string ADMIN_FILE = "admin.txt";

// ================= Utility =================
string getCurrentDate() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-"
       << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
       << setw(2) << setfill('0') << ltm->tm_mday;
    return ss.str();
}

int getIntInput(const string &prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        } else {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << RED << "Invalid input. Enter a number.\n" << RESET;
        }
    }
}

// ================= Date Validation =================
bool isValidFutureDate(const string &date) {
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(date[i])) return false;
    }

    tm tm_due = {};
    istringstream ss(date);
    ss >> get_time(&tm_due, "%Y-%m-%d");
    if (ss.fail()) return false;

    time_t now = time(0);
    time_t due_time = mktime(&tm_due);
    if (difftime(due_time, now) <= 0) return false; // not future

    return true;
}

// ================= Save/Load =================
void saveBooks() {
    ofstream bookFile(BOOKS_FILE);
    for (const auto &b : books) bookFile << b.serialize() << "\n";
}
void saveTransactions() {
    ofstream tranFile(TRANSACTIONS_FILE);
    for (const auto &t : transactions) tranFile << t.serialize() << "\n";
}
void loadData() {
    books.clear();
    transactions.clear();

    ifstream bookFile(BOOKS_FILE);
    string line;
    while (getline(bookFile, line))
        if (!line.empty()) books.push_back(Book::deserialize(line));
    bookFile.close();

    ifstream tranFile(TRANSACTIONS_FILE);
    while (getline(tranFile, line))
        if (!line.empty()) transactions.push_back(Transaction::deserialize(line));
    tranFile.close();
}

// ================= Core Functions =================
void addBook() {
    int id = books.empty() ? 1 : books.back().getId() + 1;
    string title, author;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cout << "Enter Title: ";
    getline(cin, title);
    cout << "Enter Author: ";
    getline(cin, author);

    books.emplace_back(id, title, author);
    cout << GREEN << "Book added successfully. ID: " << id << "\n" << RESET;
    saveBooks();
}

void displayAllBooks() {
    if (books.empty()) {
        cout << YELLOW << "No books available.\n" << RESET;
        return;
    }
    for (const auto &b : books) b.display();
}

void borrowBook() {
    int id = getIntInput("Enter Book ID to borrow: ");
    for (auto &b : books) {
        if (b.getId() == id) {
            if (b.getIsBorrowed()) {
                cout << RED << "Book already borrowed.\n" << RESET;
                return;
            }

            string due;
            do {
                cout << "Enter due date (YYYY-MM-DD, must be future date): ";
                cin >> due;
                if (!isValidFutureDate(due))
                    cout << RED << "Invalid date format or past date!\n" << RESET;
            } while (!isValidFutureDate(due));

            cin.ignore();
            b.borrow(due);
            transactions.emplace_back(id, "borrow", getCurrentDate());
            cout << GREEN << "Book borrowed successfully.\n" << RESET;

            saveBooks();
            saveTransactions();
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void returnBook() {
    int id = getIntInput("Enter Book ID to return: ");
    for (auto &b : books) {
        if (b.getId() == id) {
            if (!b.getIsBorrowed()) {
                cout << YELLOW << "This book is not borrowed.\n" << RESET;
                return;
            }
            b.returnBook();
            transactions.emplace_back(id, "return", getCurrentDate());
            cout << GREEN << "Book returned successfully.\n" << RESET;
            saveBooks();
            saveTransactions();
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void checkOverdueBooks() {
    bool any = false;
    for (const auto &b : books)
        if (b.isOverdue()) {
            b.display();
            any = true;
        }
    if (!any) cout << GREEN << "No overdue books.\n" << RESET;
}

void exportOverdueReport() {
    ofstream report("overdue_report.txt");
    if (!report) { cout << RED << "Cannot create report!\n" << RESET; return; }

    report << "===== OVERDUE BOOK REPORT =====\n";
    report << "Generated on: " << getCurrentDate() << "\n\n";
    report << "ID | Title | Author | Due Date | Days Overdue\n";
    report << "------------------------------------------------------\n";

    time_t now = time(0);
    bool any = false;

    for (const auto &b : books) {
        if (b.getIsBorrowed() && !b.getDueDate().empty()) {
            tm tm_due = {};
            istringstream ss(b.getDueDate());
            ss >> get_time(&tm_due, "%Y-%m-%d");
            if (ss.fail()) continue;

            time_t due_time = mktime(&tm_due);
            double diff_seconds = difftime(now, due_time);
            int daysOverdue = (diff_seconds > 0) ? (int)(diff_seconds / (60*60*24)) : 0;
            if (daysOverdue > 0) {
                report << b.getId() << " | " << b.getTitle() << " | " 
                       << b.getAuthor() << " | " << b.getDueDate() << " | "
                       << daysOverdue << "\n";
                any = true;
            }
        }
    }

    if (any) cout << GREEN << "Overdue report saved to 'overdue_report.txt'.\n" << RESET;
    else cout << GREEN << "No overdue books to report.\n" << RESET;
    report.close();
}

// ================= Transactions =================
void viewTransactions() {
    if (transactions.empty()) {
        cout << YELLOW << "No transactions available.\n" << RESET;
        return;
    }
    cout << BLUE << "===== Transaction History =====\n" << RESET;
    for (const auto &t : transactions) t.display();
}

void clearTransactions() {
    transactions.clear();
    saveTransactions();
    cout << GREEN << "Transaction history cleared.\n" << RESET;
}

// ================= Update/Delete/Sort =================
void updateBook() {
    int id = getIntInput("Enter Book ID to update: ");
    for (auto &b : books) {
        if (b.getId() == id) {
            string newTitle, newAuthor;
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            cout << "New Title (leave empty to keep current): ";
            getline(cin, newTitle);
            cout << "New Author (leave empty to keep current): ";
            getline(cin, newAuthor);
            if (!newTitle.empty()) b.setTitle(newTitle);
            if (!newAuthor.empty()) b.setAuthor(newAuthor);
            cout << GREEN << "Book updated successfully.\n" << RESET;
            saveBooks();
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}

void deleteBook() {
    int id = getIntInput("Enter Book ID to delete: ");
    auto it = remove_if(books.begin(), books.end(), [id](const Book &b){ return b.getId() == id; });
    if (it != books.end()) {
        books.erase(it, books.end());
        cout << GREEN << "Book deleted successfully.\n" << RESET;
        saveBooks();
    } else cout << RED << "Book not found.\n" << RESET;
}

void sortBooksByTitle() {
    sort(books.begin(), books.end(), [](const Book &a, const Book &b){ return a.getTitle() < b.getTitle(); });
    cout << GREEN << "Books sorted by title.\n" << RESET;
}

void sortBooksByAuthor() {
    sort(books.begin(), books.end(), [](const Book &a, const Book &b){ return a.getAuthor() < b.getAuthor(); });
    cout << GREEN << "Books sorted by author.\n" << RESET;
}

void sortBooksByAvailability() {
    sort(books.begin(), books.end(), [](const Book &a, const Book &b){ return !a.getIsBorrowed() && b.getIsBorrowed(); });
    cout << GREEN << "Books sorted by availability.\n" << RESET;
}

// ================= Admin =================
pair<string,string> loadAdmin() {
    ifstream f(ADMIN_FILE);
    string user="admin", pass="12345";
    if(f){ getline(f,user); getline(f,pass); }
    return make_pair(user,pass);
}

void saveAdmin(const string &user,const string &pass){
    ofstream f(ADMIN_FILE);
    f<<user<<"\n"<<pass<<"\n";
}

bool adminLogin() {
    pair<string,string> creds = loadAdmin();
    string username, password;
    cout << BLUE << "=====================================\n" << RESET;
    cout << CYAN << "   Library Management System\n" << RESET;
    cout << MAGENTA << "          ADMIN LOGIN\n" << RESET;
    cout << BLUE << "=====================================\n" << RESET;
    cout << "Username: "; cin >> username;
    cout << "Password: "; cin >> password;
    if(username==creds.first && password==creds.second){
        cout << GREEN << "Login successful.\n" << RESET;
        cin.ignore();
        return true;
    }
    cout << RED << "Invalid credentials.\n" << RESET;
    return false;
}

void changePassword(){
    pair<string,string> creds = loadAdmin();
    string oldPass, newPass;
    cout<<"Enter current password: "; cin>>oldPass;
    if(oldPass!=creds.second){ cout<<RED<<"Wrong current password.\n"<<RESET; return; }
    cout<<"Enter new password: "; cin>>newPass;
    saveAdmin(creds.first,newPass);
    cout<<GREEN<<"Password changed successfully.\n"<<RESET;
}

// ================= Search =================
void searchBookByTitleOrAuthor() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    string query;
    cout << "Enter title or author to search: ";
    getline(cin, query);

    bool found = false;
    for (const auto &b : books) {
        if (b.getTitle().find(query) != string::npos ||
            b.getAuthor().find(query) != string::npos) {
            b.display();
            found = true;
        }
    }
    if (!found) cout << RED << "No matching books found.\n" << RESET;
}

void searchBookById() {
    int id = getIntInput("Enter Book ID to search: ");
    for (const auto &b : books) {
        if (b.getId() == id) {
            b.display();
            return;
        }
    }
    cout << RED << "Book not found.\n" << RESET;
}


// ================= Admin Menu =================
void adminMenu() {
    int choice;
    do {
        cout << "\n" << YELLOW << "===== ADMIN MENU =====\n" << RESET;
        cout << "1. Add Book\n2. Display All Books\n3. Search Book by Title/Author\n4. Search Book by ID\n5. Borrow Book\n6. Return Book\n7. Update Book Info\n8. Delete Book\n9. Check Overdue Books\n10. Export Overdue Report\n11. View Transactions\n12. Clear Transactions\n13. Sort Books by Title\n14. Sort Books by Author\n15. Sort Books by Availability\n16. Change Password\n17. Exit\n";
        choice = getIntInput("Enter choice: ");
        switch(choice){
            case 1:addBook(); break;
            case 2:displayAllBooks(); break;
            case 3: searchBookByTitleOrAuthor(); break;
            case 4: searchBookById(); break;
            case 5:borrowBook(); break;
            case 6:returnBook(); break;
            case 7:updateBook(); break;
            case 8:deleteBook(); break;
            case 9:checkOverdueBooks(); break;
            case 10:exportOverdueReport(); break;
            case 11:viewTransactions(); break;
            case 12:clearTransactions(); break;
            case 13:sortBooksByTitle(); break;
            case 14:sortBooksByAuthor(); break;
            case 15:sortBooksByAvailability(); break;
            case 16:changePassword(); break;
            case 17: cout<<GREEN<<"Exiting...\n"<<RESET; break;
            default: cout<<RED<<"Invalid choice.\n"<<RESET;
        }
    } while(choice!=17);
}

// ================= Main =================
int main() {
    loadData();
    if(adminLogin()){
        adminMenu();
        saveBooks();
        saveTransactions();
    }
    return 0;
}
