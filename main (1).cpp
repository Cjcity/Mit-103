#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <functional>   // std::hash
using namespace std;

const int MAX_TRANSACTIONS = 100;   // Maximum records

// =========================
// Date structure
// =========================
struct Date {
    int day;
    int month;
    int year;
};

// =========================
// Transaction Class
// =========================
class Transaction {
private:
    Date date;
    string type;      // Income / Expense
    string subType;   // Fixed / Variable
    string category;  // Salary, Rent, Food, etc.
    string note;      // Short description
    float amount;     // +ve for income, -ve for expense

public:
    Transaction() {
        date = {0,0,0};
        type = "";
        subType = "";
        category = "";
        note = "";
        amount = 0.0;
    }

    Transaction(Date d, string t, string st, string c, string n, float a) {
        date = d; type = t; subType = st; category = c; note = n; amount = a;
    }

    // setters
    void setDate(Date d) { date = d; }
    void setType(string t) { type = t; }
    void setSubType(string st) { subType = st; }
    void setCategory(string c) { category = c; }
    void setNote(string n) { note = n; }
    void setAmount(float a) { amount = a; }

    // getters
    Date getDate() const { return date; }
    string getType() const { return type; }
    string getSubType() const { return subType; }
    string getCategory() const { return category; }
    string getNote() const { return note; }
    float getAmount() const { return amount; }

    // Display one transaction
    void show() const {
        cout << setw(2) << setfill('0') << date.day << "/"
             << setw(2) << setfill('0') << date.month << "/"
             << setw(4) << setfill(' ') << date.year << "  "
             << setw(8) << type
             << setw(10) << subType
             << setw(12) << category
             << setw(20) << note
             << setw(10) << fixed << setprecision(2) << amount
             << endl;
    }
};

// =========================
// Function Prototypes
// =========================
void addTransaction(Transaction arr[], int &count);
void deleteTransaction(Transaction arr[], int &count);
void listTransactions(const Transaction arr[], int count);
void sortTransactions(Transaction arr[], int count);
void searchTransactions(const Transaction arr[], int count);
bool isEarlier(Date d1, Date d2);
bool loginSystem();

// =========================
// Simple portable hash
// =========================
string simpleHash(const string &str) {
    std::hash<string> hasher;
    size_t hashed = hasher(str);
    stringstream ss;
    ss << hex << hashed;
    return ss.str();
}

// =========================
// Main Menu
// =========================
int main() {
    cout << "====== Personal Finance Tracker ======\n";
    if(!loginSystem()) {
        cout << "Too many failed attempts. Exiting...\n";
        return 0;
    }

    Transaction records[MAX_TRANSACTIONS];
    int total = 0;
    int choice;

    do {
        cout << "\n========== Main Menu ==========\n";
        cout << "1. Add a new transaction\n";
        cout << "2. Remove a transaction\n";
        cout << "3. Search transactions\n";
        cout << "4. Display all transactions\n";
        cout << "5. Sort transactions\n";
        cout << "6. Exit program\n";
        cout << "Select an option: ";
        cin >> choice;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Try again.\n";
            continue;
        }

        switch(choice) {
            case 1: addTransaction(records, total); break;
            case 2: deleteTransaction(records, total); break;
            case 3: searchTransactions(records, total); break;
            case 4: listTransactions(records, total); break;
            case 5: sortTransactions(records, total); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while(choice != 6);

    return 0;
}

// =========================
// Login System
// =========================
bool loginSystem() {
    // Predefined username/password (hashed)
    const string USERNAME = "admin";
    const string PASSWORD_HASH = simpleHash("pass123"); // change password here

    string u, p;
    int attempts = 3;

    while(attempts--) {
        cout << "Username: ";
        cin >> u;
        cout << "Password: ";
        cin >> p;

        if(u == USERNAME && simpleHash(p) == PASSWORD_HASH) {
            cout << "Login successful!\n";
            return true;
        } else {
            cout << "Invalid credentials. Attempts left: " << attempts << "\n";
        }
    }
    return false;
}

// =========================
// Function Definitions
// =========================

// Add new transaction
void addTransaction(Transaction arr[], int &count) {
    if(count >= MAX_TRANSACTIONS) {
        cout << "Transaction list full!\n";
        return;
    }

    string dateStr;
    Date d;
    string t, st, c, n;
    float a;

    cout << "Enter date (DD/MM/YYYY): ";
    cin >> dateStr;
    d.day   = stoi(dateStr.substr(0,2));
    d.month = stoi(dateStr.substr(3,2));
    d.year  = stoi(dateStr.substr(6,4));

    int typeChoice;
    cout << "Type: 1. Income  2. Expense: ";
    cin >> typeChoice;
    t = (typeChoice == 1 ? "Income" : "Expense");

    int stChoice;
    cout << "Subtype: 1. Fixed  2. Variable: ";
    cin >> stChoice;
    st = (stChoice == 1 ? "Fixed" : "Variable");

    cout << "Enter category: ";
    cin >> c;

    cout << "Enter description: ";
    cin.ignore();
    getline(cin, n);

    cout << "Enter amount (+ for income, - for expense): ";
    cin >> a;

    arr[count] = Transaction(d, t, st, c, n, a);
    count++;

    cout << "Transaction added!\n";
}

// Delete transaction
void deleteTransaction(Transaction arr[], int &count) {
    if(count == 0) {
        cout << "No transactions available.\n";
        return;
    }

    int index;
    cout << "Enter transaction number to delete (1-" << count << "): ";
    cin >> index;

    if(index < 1 || index > count) {
        cout << "Invalid index.\n";
        return;
    }

    for(int i = index-1; i < count-1; i++) {
        arr[i] = arr[i+1];
    }
    count--;

    cout << "Transaction deleted.\n";
}

// Display transactions
void listTransactions(const Transaction arr[], int count) {
    if(count == 0) {
        cout << "No transactions found.\n";
        return;
    }

    cout << "Date       Type     SubType   Category    Description          Amount\n";
    cout << "-------------------------------------------------------------------------------\n";

    for(int i = 0; i < count; i++) {
        arr[i].show();
    }
}

// Compare two dates
bool isEarlier(Date d1, Date d2) {
    if(d1.year != d2.year) return d1.year < d2.year;
    if(d1.month != d2.month) return d1.month < d2.month;
    return d1.day < d2.day;
}

// Sort transactions
void sortTransactions(Transaction arr[], int count) {
    if(count == 0) {
        cout << "No transactions to sort.\n";
        return;
    }

    int option;
    cout << "Sort by:\n";
    cout << "1. Date\n";
    cout << "2. Amount\n";
    cout << "3. Type (Income/Expense)\n";
    cout << "4. SubType (Fixed/Variable)\n";
    cout << "5. Category\n";
    cout << "Enter choice: ";
    cin >> option;

    for(int i = 0; i < count-1; i++) {
        for(int j = 0; j < count-i-1; j++) {
            bool swapNeeded = false;

            if(option == 1 && !isEarlier(arr[j].getDate(), arr[j+1].getDate()))
                swapNeeded = true;
            else if(option == 2 && arr[j].getAmount() > arr[j+1].getAmount())
                swapNeeded = true;
            else if(option == 3 && arr[j].getType() > arr[j+1].getType())
                swapNeeded = true;
            else if(option == 4 && arr[j].getSubType() > arr[j+1].getSubType())
                swapNeeded = true;
            else if(option == 5 && arr[j].getCategory() > arr[j+1].getCategory())
                swapNeeded = true;

            if(swapNeeded) {
                Transaction temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    cout << "Sorting completed.\n";
}

// Search transactions
void searchTransactions(const Transaction arr[], int count) {
    if(count == 0) {
        cout << "No transactions to search.\n";
        return;
    }

    int option;
    cout << "Search by:\n";
    cout << "1. Date\n";
    cout << "2. Category\n";
    cout << "3. Type\n";
    cout << "4. SubType\n";
    cout << "Enter choice: ";
    cin >> option;

    bool found = false;

    if(option == 1) {
        string dateStr;
        Date d;
        cout << "Enter date (DD/MM/YYYY): ";
        cin >> dateStr;
        d.day   = stoi(dateStr.substr(0,2));
        d.month = stoi(dateStr.substr(3,2));
        d.year  = stoi(dateStr.substr(6,4));

        for(int i = 0; i < count; i++) {
            Date temp = arr[i].getDate();
            if(temp.day == d.day && temp.month == d.month && temp.year == d.year) {
                arr[i].show();
                found = true;
            }
        }
    }
    else {
        string key;
        cout << "Enter search keyword: ";
        cin >> key;
        for(int i = 0; i < count; i++) {
            if((option == 2 && arr[i].getCategory() == key) ||
               (option == 3 && arr[i].getType() == key) ||
               (option == 4 && arr[i].getSubType() == key)) {
                arr[i].show();
                found = true;
            }
        }
    }

    if(!found) cout << "No results found.\n";
}
