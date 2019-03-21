#include "Accounts.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>    // for string streams
#include <fstream>    // for work with files

using namespace std;

const bool SHOW_SERVICE_MESSAGE = false;      // show/hide messages about constructors' and descructors' work
int Basic_Account::number_accounts = 1000;   // number of already created in the system accounts (incl. deleted)

void ServiceMessage(string const &str) {
    if (SHOW_SERVICE_MESSAGE)
        cout << str << endl;
}



// === Methods of Basic_Account ===
Basic_Account::Basic_Account(float br) {
    cout << "Enter name: ";
    getline(cin, name);
    
    cout << "Enter initial balance: ";
    cin >> balance;

    bank_rate = br;
    number_accounts ++;
    ac_number = number_accounts;
    ServiceMessage("Basic account " + to_string(ac_number) + " is constructed");
}


Basic_Account::Basic_Account(int ac_n, string n, double b, float br)
    : ac_number{ac_n}, name{n}, balance{b}, bank_rate{br}{
    ServiceMessage("Basic account " + to_string(ac_number) + " is constructed");
}


Basic_Account::~Basic_Account() {
    ServiceMessage("Basic account " + to_string(ac_number) + " is destructed");
}


bool Basic_Account::operator==(const Basic_Account &rhs) const {
    return (this->ac_number == rhs.ac_number &&
            this->balance == rhs.balance &&
            this->bank_rate == rhs.bank_rate);
}


bool Basic_Account::operator==(int ac_n) const {
    return (this->ac_number == ac_n);
}


string Basic_Account::AccountInfo() const {
    ostringstream oss{};
    oss << "Info about account # " << ac_number << "\n"
        << "  name: " << name << "\n"
        << "  balance: " << fixed << setprecision(2) << balance << "$" << endl;
    return oss.str();
}


bool Basic_Account::Deposit(double amount, string &msg) {
    if (amount < 0) {
        msg = "Attempt to deposit negative amount";
        return false;
    }
    
    this->balance += amount;
    msg = "Done";
    return true;
}


bool Basic_Account::Withdrawal(double amount, string &msg) {
    if (amount < 0) {
        msg = "Attempt to withdrawal negative amount";
        return false;
    }
    
    if (this->balance - amount < 0) {
        msg = "Not enough money on the balance";
        return false;
    }
   
    this->balance -= amount;
    msg = "Done";
    return true;
}


ostream &operator<< (ostream &os, const Basic_Account &ac) {
    os << ac.AccountInfo();
    return os;
}


ofstream &operator<< (ofstream &ofs, const Basic_Account &ac) {
    ofs << "\n" << ac.SaveAccountToFile();
    return ofs;
}



// === Method of Standard_Account ===
Standard_Account::Standard_Account()
    : Basic_Account(default_bank_rate) {
    ServiceMessage("Standard account " + to_string(ac_number) + " is constructed");
}


Standard_Account::Standard_Account(int ac_n, string n, double b, float br)
    : Basic_Account(ac_n, n, b, br) {
    ServiceMessage("Standard account " + to_string(ac_number) + " is constructed");
}


Standard_Account::~Standard_Account() {
    ServiceMessage("Standard account " + to_string(ac_number) + " is destructed");
}


Standard_Account* Standard_Account::LoadAccountFromFile(ifstream &in_file) {
    int ac_number;
    string name;
    double balance;
    float bank_rate;

    in_file >> ac_number;
    getline(in_file >> ws, name);           // 'in_file >> ws' allows to skip spaces and similar characters
    in_file >> balance >> bank_rate;

    return new Standard_Account(ac_number, name, balance, bank_rate);
}


string Standard_Account::SaveAccountToFile() const {
    char buff[100];
    sprintf(buff, "%d\n%d\n%s\n%4.2f\n%5.3f\n",
        Standard_Account::account_type, ac_number, name.c_str(), balance, bank_rate    // c_str() is used to convert C++-string to C-string
    );
    return buff;
}


string Standard_Account::AccountInfo() const {
    string basic_info = Basic_Account::AccountInfo();

    ostringstream oss{};
    oss << "  account type: standard" << endl
        << "  bank rate: " << defaultfloat << setprecision(6) << bank_rate << "%" << endl;

    return basic_info + oss.str();
}


bool Standard_Account::Deposit(double amount, string &msg) {
    return Basic_Account::Deposit(amount, msg);
}


bool Standard_Account::Withdrawal(double amount, string &msg) {
    return Basic_Account::Withdrawal(amount, msg);
}



// === Method of Savings_Account ===
Savings_Account::Savings_Account()
    : Basic_Account(default_bank_rate), remaining_withdrawals{max_withdrawals} {
    ServiceMessage("Savings account " + to_string(ac_number) + " is constructed");
}


Savings_Account::Savings_Account(int an, string n, double b, float br, int rw)
    : Basic_Account(an, n, b, br), remaining_withdrawals{rw} {
    ServiceMessage("Savings account " + to_string(ac_number) + " is constructed");
}


Savings_Account::~Savings_Account() {
    ServiceMessage("Savings account " + to_string(ac_number) + " is destructed");
}


string Savings_Account::AccountInfo() const {
    string basic_info = Basic_Account::AccountInfo();
    
    ostringstream oss{};
    oss << "  account type: savings" << endl
        << "  bank rate: " << defaultfloat << setprecision(6) << bank_rate << "%" << endl
        << "  remaining number of withdrawals: " << remaining_withdrawals << endl;

    return basic_info + oss.str();
}


Savings_Account* Savings_Account::LoadAccountFromFile(ifstream &in_file) {
    int ac_number;
    string name;
    double balance;
    float bank_rate;
    int remaining_withdrawals;

    in_file >> ac_number;
    getline(in_file >> ws, name);
    in_file >> balance >> bank_rate >> remaining_withdrawals;

    return new Savings_Account(ac_number, name, balance, bank_rate, remaining_withdrawals);
}


string Savings_Account::SaveAccountToFile() const {
    char buff[100];
    sprintf(buff, "%d\n%d\n%s\n%4.2f\n%5.3f\n%d\n",
        Savings_Account::account_type, ac_number, name.c_str(), balance, bank_rate, remaining_withdrawals
    );
    return buff;
}


bool Savings_Account::Deposit(double amount, string &msg) {
    return Basic_Account::Deposit(amount, msg);
}


bool Savings_Account::Withdrawal(double amount, string &msg) {
    if (remaining_withdrawals <= 0) {
        msg = "The limit of withdrawals for this year is exhausted.";
        return false;
    }
    
    if (Basic_Account::Withdrawal(amount, msg)) {
        remaining_withdrawals--;
        return true;
    } else
        return false;
}