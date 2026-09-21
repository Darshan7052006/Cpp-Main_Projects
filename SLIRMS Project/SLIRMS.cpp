#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <memory>

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
    } const 

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

    string getBookID(){
        return bookID;
    }

    string getISBN(){
        return ISBN;
    }

    string getTitle(){
        return title;
    }

    string getauthor(){
        return author;
    }

    string getcategory(){
        return category;
    }

    int getpublicationyear(){
        return publicationyear;
    }

    bookstatus getbook(){
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
    Loan(string loanid = "",
        string userid = "",
        string bookid = "",
        time_t issuedate = 0,
        time_t duedate = 0,
        time_t returndate = 0,
        LoanStatus status = LoanStatus::Active){
            this->bookID = bookid;
            this->LoanID = loanid;
            this->userID = userid;
            this->issuedate = time(nullptr);
            this->returndate = 0;
            this->duedate = this->issuedate + (14 * 24 * 60 * 60); 
            this->status = LoanStatus::Active;
        }

    string getLoanID(){
        return LoanID;
    }

    string getuserid(){
        return userID;
    }

    string getBookid(){
        return bookID;
    }

    time_t getIssueDate(){
        return issuedate;
    }

    time_t getduedate(){
        return duedate;
    }

    time_t getreturndate(){
        return returndate;
    }

    LoanStatus getstatus(){
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
};

int main() {

 return 0;
}
