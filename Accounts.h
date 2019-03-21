#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>

using std::string;
using std::ostream;
using std::ofstream;
using std::ifstream;
using std::unique_ptr;


class Basic_Account {                // abstract class
    friend ostream &operator<< (ostream &os, const Basic_Account &ac);
    friend ofstream &operator<< (ofstream &ofs, const Basic_Account &ac);
protected:
    static int number_accounts;      // number of already created in the system accounts (incl. deleted)
    int ac_number;
    string name;
    double balance;
    float bank_rate;
public:
    int Get_ac_number() {return ac_number;}

    Basic_Account(float br = 0.0f);
    Basic_Account(int ac_n, string n, double b, float br);
    ~Basic_Account();
    
    static void Set_number_accounts(int n) {number_accounts = n; }
    static int Get_number_accounts() {return number_accounts; }
    virtual string SaveAccountToFile() const = 0;     // pure virtual function
    
    bool operator==(const Basic_Account &rhs) const;  // not used in the program, for its future expansion
    bool operator==(int ac_n) const;                  // for searching account by its number
    virtual string AccountInfo() const;               // full information about account
    virtual bool Deposit(double amount, string &msg);
    virtual bool Withdrawal(double amount, string &msg);
};


class Standard_Account: public Basic_Account {
public:
    constexpr static int account_type = 101;
    constexpr static float default_bank_rate = 4;
    
    Standard_Account();
    Standard_Account(int ac_n, string n, double b, float br);
    ~Standard_Account();
    static Standard_Account* LoadAccountFromFile(ifstream &in_file);
    virtual string SaveAccountToFile() const override;
    
    virtual string AccountInfo() const;
    virtual bool Deposit(double amount, string &msg) override;
    virtual bool Withdrawal(double amount, string &msg) override;
};


class Savings_Account: public Basic_Account {
protected:
    int remaining_withdrawals;
public:
    constexpr static int account_type = 102;
    constexpr static float default_bank_rate = 4.8;
    constexpr static int max_withdrawals = 3;
    
    Savings_Account();
    Savings_Account(int ac_n, string n, double b, float br, int rw);
    ~Savings_Account();
    static Savings_Account* LoadAccountFromFile(ifstream &in_file);
    virtual string SaveAccountToFile() const override;
    
    virtual string AccountInfo() const override;
    virtual bool Deposit(double amount, string &msg) override;
    virtual bool Withdrawal(double amount, string &msg) override;
};

#endif // _ACCOUNT_H_