#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <memory>
#include "sqlite-amalgamation-3530400/sqlite3.h"

using namespace std;

enum class Role {
    Student,
    Teacher,
    Manager
};

enum class AccountStatus {
    Active,
    Suspended
};

enum class bookstatus{
    Available,
    Issued
};

enum class LoanStatus{
    Active,
    Returned
};

void showwelcomemenu(){
    cout << "\n----------WELCOME TO SLIRMS----------\n";
    cout << "1)Login\n";
    cout << "2)Register\n";
    cout << "3)Exit\n";
}

void registrationmenu(){
    cout << "\n----------REGISTRATION MENU----------\n";
    cout << "1)Student Registration\n";
    cout << "2)Teaher Registration\n";
    cout << "3)Back\n";
}

void StudentMenu(){
    cout << "\n----------STUDENT MENU----------\n";
    cout << "1)Browse Books\n";
    cout << "2)Search Books\n";
    cout << "3)Issue Book\n";
    cout << "4)Return Book\n";
    cout << "5)My Active Loans\n";
    cout << "6)My Loan History\n";
    cout << "7)My Profile\n";
    cout << "8)Change Password\n";
    cout << "9)Logout";
}

void TeacherMenu(){
    cout << "\n----------Teacher MENU----------\n";
    cout << "1)Browse Books\n";
    cout << "2)Search Books\n";
    cout << "3)Issue Book\n";
    cout << "4)Return Book\n";
    cout << "5)My Active Loans\n";
    cout << "6)My Loan History\n";
    cout << "7)My Profile\n";
    cout << "8)Change Password\n";
    cout << "9)Logout";
}

void ManagerMenu(){
    cout << "\n----------WELCOME MISTER MANAGER----------\n";
    cout << "1)User Management\n";
    cout << "2)Book Management\n";
    cout << "3)Loan Management\n";
    cout << "4)Activity Monitoring\n";
    cout << "5)Reports\n";
    cout << "6)System Statistics\n";
    cout << "7)My Profile\n";
    cout << "8)Change Password\n";
    cout << "9)Logout\n";
}

void Usermanagement(){
    cout << "\n-----------WELCOME TO USER MANAGEMENT SECTION----------\n";
    cout << "1)View All Users\n";
    cout << "2)Find User\n";
    cout << "3)View Students\n";
    cout << "4)View Teachers\n";
    cout << "5)Register User\n";
    cout << "6)Suspend User\n";
    cout << "7)Activate User\n";
    cout << "8)Remove User\n";
    cout << "9)User Activity\n";
}

void Bookmanagement(){
    cout << "\n----------WELCOME TO BOOK MANAGEMENT SECTION----------\n";
    cout << "1)Add Book\n";
    cout << "2)Find Book\n";
    cout << "3)Search Books\n";
    cout << "4)View all Books\n";
    cout << "5)Update Book\n";
    cout << "6)Remove Book";
    cout << "7)Issued Books\n";
    cout << "8)Available Books\n";
}

void LoanManagement(){
    cout << "\n----------WELCOME TO LOAN MANAGEMENT SECTION----------\n";
    cout << "1)All Loans\n";
    cout << "2)Active Loans\n";
    cout << "3)Returned Loans\n";
    cout << "4)Overdue Loans\n";
    cout << "5)Find Loan\n";
    cout << "6)User Loans\n";

}

class User {
private:
    string Userid;
    string name;
    string email;
    string password;
    Role role;
    AccountStatus accountstatus;
    
public:
    virtual ~User() = default;

    User(string userid = "",
         string name = "",
         string email = "",
         string password = "",
         Role role = Role::Student,
         AccountStatus accountStatus = AccountStatus::Active) {

        this->Userid = userid;
        this->name = name;
        this->email = email;
        this->password = password;
        this->role = role;
        this->accountstatus = accountStatus;
    }

    string getUserId() const {
        return Userid;
    }

    string getName() const {
        return name;
    }

    string getEmail() const {
        return email;
    }

    Role getRole() const {
        return role;
    }

    AccountStatus getAccountstatus() const {
        return accountstatus;
    }

    void setName(string name) {
        this->name = name;
    }

    void setEmail(string email) {
        this->email = email;
    }

    void changePassword(string Password) {
        this->password = Password;
    }

    void changeAccountStatus(AccountStatus status) {
        this->accountstatus = status;
    }

    virtual void displayInfo() const {
    cout << "User ID: " << Userid << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
}
};

class Student : public User{
private:
    string registrationNumber;
    string department;
    int semester;
public:
    Student(string registrationnumber = "",
        string department = "",
        int semester = 0,
        string userid = "",
        string email = "",
        string name = "",
        string password = "")
    : User(userid, name, email, password,
           Role::Student, AccountStatus::Active)
{
    this->registrationNumber = registrationnumber;
    this->department = department;
    this->semester = semester;
}

    string getregnum() const {
        return registrationNumber;
    }

    string getdepartment() const {
        return department;
    }
    
    int getsemester() const {
        return semester;
    }

    void setDepartment(string department){
        this->department = department;
    }

    void setsemester(int semester){
        this->semester = semester;
    }

    void displayInfo() const override {
    cout << "User ID: " << getUserId() << endl;
    cout << "Name: " << getName() << endl;
    cout << "Email: " << getEmail() << endl;
    cout << "Registration Number: " << registrationNumber << endl;
    cout << "Department: " << department << endl;
    cout << "Semester: " << semester << endl;
}

};

class Teacher : public User{
private:
    string facultyId;
    string department;
    string designation;
public:
    Teacher(string userid = "",
        string name = "",
        string email = "",
        string password = "",
        string facultyid = "",
        string department = "",
        string designation = "") : User(userid,name,email,password,Role::Teacher, AccountStatus::Active)
        {
            this->facultyId = facultyid;
            this->department = department;
            this->designation = designation;
        }
    
    string getDepartment() const{
        return department;
    }

    string getdesignation() const{
        return designation;
    }

    string getfacultyid() const{
        return facultyId;
    }

    void setdepartment(string department){
        this->department = department;
    }

    void setdesignation(string designation){
        this->designation = designation;
    }

    void displayInfo() const override {
        cout << "User ID: " << getUserId() << endl;
        cout << "Name: " << getName() << endl;
        cout << "Email: " << getEmail() << endl;
        cout << "Faculty ID: " << facultyId << endl;
        cout << "Department: " << department << endl;
        cout << "Designation: " << designation << endl;
    }

};

class Manager : public User{
public:
    Manager(string userid = "",
        string name = "",
        string email = "",
        string password = "")
    : User(userid, name, email, password,
           Role::Manager, AccountStatus::Active)
{
}

};

class Book{
private:
    string bookID;
    string ISBN;
    string title;
    string author;
    string category;
    int publicationyear;
    bookstatus status;

public:
    Book(string bookid = "",
        string ISBN =  "",
        string title =  "",
        string author =  "",
        string category = "",
        int publicationyear = 0,
        bookstatus status = bookstatus::Available)
        {

            this->bookID = bookid;
            this->ISBN = ISBN;
            this->title = title;
            this->author = author;
            this->category = category;
            this->publicationyear = publicationyear;
            this->status = status;

        }

    string getBookID() const {
        return bookID;
    }

    string getISBN() const {
        return ISBN;
    }

    string getTitle() const {
        return title;
    }

    string getauthor() const {
        return author;
    }

    string getcategory() const {
        return category;
    }

    int getpublicationyear() const {
        return publicationyear;
    }

    bookstatus getbook() const {
        return status;
    }

    void setcategory(string category){
        this -> category = category;
    }

    bool issueBook(){
        if(status == bookstatus::Issued){
            return false;
        }

        status = bookstatus::Issued;
        return true;
    }

    bool returnBook(){
        if(status == bookstatus::Available){
            return false;
        }

        status = bookstatus::Available;
        return true;
    }
};

class Loan{
private:
    string LoanID;
    string userID;
    string bookID;

    time_t issuedate;
    time_t duedate;
    time_t returndate;

    LoanStatus status;

public:
    Loan(string loanid, string userid, string bookid)
        : LoanID(loanid),
          userID(userid),
          bookID(bookid),
          issuedate(time(nullptr)),
          duedate(issuedate + (14 * 24 * 60 * 60)),
          returndate(0),
          status(LoanStatus::Active)
    {
    }

    // Restore a loan using the values loaded from the database.
    Loan(string loanid,
         string userid,
         string bookid,
         time_t issuedate,
         time_t duedate,
         time_t returndate,
         LoanStatus status)
        : LoanID(loanid),
          userID(userid),
          bookID(bookid),
          issuedate(issuedate),
          duedate(duedate),
          returndate(returndate),
          status(status)
    {
    }


    string getLoanID() const{
        return LoanID;
    }

    string getuserid() const{
        return userID;
    }

    string getBookid() const{
        return bookID;
    }

    time_t getIssueDate() const{
        return issuedate;
    }

    time_t getduedate() const{
        return duedate;
    }

    time_t getreturndate() const{
        return returndate;
    }

    LoanStatus getstatus() const{
        return status;
    }

    bool returnLoan(){
        if(status == LoanStatus::Active){
            returndate = time(nullptr);
            status = LoanStatus::Returned;
            return true;
        }
        return false;
    }

    bool isOverdue(){
        if(status == LoanStatus::Active){
            time_t current_time = time(nullptr);
            if(current_time > duedate){
                return true;
            }

            return false;
        }

        return false;
    }
};

class Library{
private:
    vector<unique_ptr<User>> users;
    vector<Book> books;
    vector<Loan> loans;

public:
    void adduser(unique_ptr<User> user){
        users.push_back(move(user));
    }

    User* finduserById(const string& userID){
        for(auto& user : users){
            if(user->getUserId() == userID){
                return user.get();
            }
        }
        return nullptr;
    }

    void addbook(const Book& book){
        books.push_back(book);
    }

    Book* findbookbyId(const string& bookid){
        for(auto& book : books){
            if(book.getBookID() == bookid){
                return &book;
            }
        }
        return nullptr;
    }
    
    vector<Book*> searchBytitle(const string& title){
        vector<Book*> results; 
        for(auto& Book : books){
            if(Book.getTitle() == title){
                results.push_back(&Book);
            }
        }
        return results;
    }

    vector<Book*> searchByISBN(const string& ISBN){
        vector<Book*> results; 
        for(auto& Book : books){
            if(Book.getISBN() == ISBN){
                results.push_back(&Book);
            }
        }
        return results;
    }

    bool issuebook(const string& userID,
                   const string& bookID,
                   const string& loanID)
    {
        User* user = finduserById(userID);
        if((user == nullptr) || (user->getAccountstatus() != AccountStatus::Active)){
            return false;
        }

        Book* book = findbookbyId(bookID);
        if(book == nullptr){
            return false;
        }
        if(!book->issueBook()){
            return false;
        }

        Loan loan(loanID, userID, bookID);

        loans.push_back(loan);

        return true;
    }

    Loan* findactiveloan(
    const string& userID,
    const string& bookID){

        for(auto& loan:loans){
            if(loan.getuserid() == userID && 
               loan.getBookid() == bookID && 
               loan.getstatus() == LoanStatus::Active){
                return &loan;
               }
        }
        return nullptr;
    }

    bool returnBook(
        const string& userID,
        const string& bookID
    ){
        Loan* loan = findactiveloan(userID,bookID);
        if(loan == nullptr){
            return false;
        }
        Book* book = findbookbyId(bookID);
        if(book == nullptr){
            return false;
        }
        if(!book->returnBook()){
            return false;
        }
        loan->returnLoan();
        return true;
    }

    void displaybooks() const{
        for(const auto& book: books){
            cout << " Book ID : " << book.getBookID() << endl;
            cout << " Title : " << book.getTitle() << endl;
            cout << "ISBN : " << book.getISBN() << endl;
            cout << "Status : ";
            if(book.getbook() == bookstatus ::Available){
                cout << "Available" << endl;
            }
            else{
                cout << "Issued" << endl;
            }

            cout << endl;
            cout << "-----------------------------------" << endl;
        }
    }
};

class Database{
private:
    sqlite3* db = nullptr;
public:
    bool open(const string& filename){
        int result = sqlite3_open(filename.c_str(), &db);
        if(result != SQLITE_OK){
            cerr << "Could not open Database: " << sqlite3_errmsg(db) << endl;
            return false;
        }
        cout << "Database opened Successfully." << endl;
        return true;
    }

    bool createTables(){

        if (db == nullptr) {
        cerr << "Database is not open." << endl;
        return false;
        }

        const char* sql = R"(
        CREATE TABLE IF NOT EXISTS users(
            user_id TEXT PRIMARY KEY UNIQUE,
            name TEXT NOT NULL,
            email TEXT NOT NULL UNIQUE,
            password_hash TEXT NOT NULL,
            role TEXT NOT NULL,
            account_status TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS books(
            book_id TEXT PRIMARY KEY,
            isbn TEXT NOT NULL,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            category TEXT NOT NULL,
            publication_year INTEGER NOT NULL,
            status TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS loans(
            loan_id TEXT PRIMARY KEY,
            user_id TEXT NOT NULL,
            book_id TEXT NOT NULL,
            issue_date INTEGER NOT NULL,
            due_date INTEGER NOT NULL,
            return_date INTEGER,
            status TEXT NOT NULL,
            
            FOREIGN KEY(user_id) REFERENCES users(user_id),
            FOREIGN KEY(book_id) REFERENCES books(book_id)
        );
    )";
    char *errormessage = nullptr;
    int result = sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        &errormessage
    );

    if (result != SQLITE_OK){
         cerr << "Could not create tables: "
             << errormessage << endl;

        sqlite3_free(errormessage);
        return false;
    }

    cout << "Tables are ready." << endl;
    return true;
    }
    ~Database(){
        if(db!= nullptr){
            sqlite3_close(db);
        }
    }

    bool enableForeignKeys(){
    if(db == nullptr){
    cerr << "Database is not open" << endl;
    return false;
        }
    const char* sql = "PRAGMA foreign_keys = ON;";
    char* errormessage = nullptr;
    int result = sqlite3_exec(db,
                              sql,
                              nullptr,
                              nullptr,
                              &errormessage);

    if(result != SQLITE_OK){
        cerr << "Could not enable foreign keys: " << (errormessage != nullptr ? errormessage
                                                                            : sqlite3_errmsg(db)) 
                                              << endl;
        sqlite3_free(errormessage);
        return false;
        }

    cout << "Foreign-key checking is enabled." << endl;
    return true;
    }

    bool insertbook(const Book& book){
        if(db == nullptr){
            cerr << "Database is not open." << endl;
            return false;
        }

        const char* sql = 
              "INSERT INTO books "
              "(book_id, isbn, title, author, category, publication_year, status) "
              "VALUES (?, ?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* statement = nullptr;

        int result = sqlite3_prepare_v2(
            db,
            sql,
            -1,
            &statement,
            nullptr
        );

        if(result != SQLITE_OK){
            cerr << "Could not prepare book insert: "
                 << sqlite3_errmsg(db) << endl;
            return false;
        }

        const char* statustext = 
            (book.getbook() == bookstatus::Available)?"Available" : "Issued";

        bool bindingsSucceded = 
             sqlite3_bind_text(statement, 1, book.getBookID().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK 
             &&
              sqlite3_bind_text(statement, 2, book.getISBN().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK 
              &&
            sqlite3_bind_text(statement, 3, book.getTitle().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK 
            &&
            sqlite3_bind_text(statement, 4, book.getauthor().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK 
            &&
            sqlite3_bind_text(statement, 5, book.getcategory().c_str(), -1, SQLITE_TRANSIENT) == SQLITE_OK 
            &&
            sqlite3_bind_int(statement, 6, book.getpublicationyear()) == SQLITE_OK 
            &&
            sqlite3_bind_text(statement, 7, statustext, -1, SQLITE_TRANSIENT) == SQLITE_OK;

            result = sqlite3_step(statement);

            if (result != SQLITE_DONE) {
                cerr << "Could not insert book: "
                    << sqlite3_errmsg(db) << endl;
                sqlite3_finalize(statement);
                return false;
            }

            sqlite3_finalize(statement);

            cout << "Book saved successfully." << endl;
            return true;
        }

};

// int main() {

//     return 0;
// }

int main() {
    Database database;

    if (!database.open("slims.db")) {
        return 1;
    }

    if (!database.enableForeignKeys()) {
        return 1;
    }

    if (!database.createTables()) {
        return 1;
    }

    Book sampleBook(
        "B001",
        "9780000000001",
        "Learning SQLite",
        "A. Writer",
        "Computing",
        2024,
        bookstatus::Available
    );

    if (!database.insertbook(sampleBook)) {
        return 1;
    }

    cout << "Database setup and sample book insert are complete."
         << endl;


    return 0;
}
