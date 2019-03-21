#include "Account_Util.h"
#include "Accounts.h"
#include <iostream>
#include <vector>
#include <fstream>        // for work with files
#include <string>
#include <limits>         // for numeric_limits
#include <algorithm>
#include <memory>         // for unique_ptr

const string FILE_ADDRESS = "../accounts_state.txt";

using namespace std;

bool LoadDataFromFile(vector <unique_ptr<Basic_Account>> &accounts_vector) {
    ifstream in_file(FILE_ADDRESS);
    if (in_file) {
        try {
            int n_read;
            in_file >> n_read;
            Basic_Account::Set_number_accounts(n_read);
            
            while (in_file >> n_read) {
                switch (n_read) {
                    case Standard_Account::account_type:
                        accounts_vector.push_back(move(
                            unique_ptr<Basic_Account> {Standard_Account::LoadAccountFromFile(in_file)}
                        ));
                        break;
                    case Savings_Account::account_type:
                        accounts_vector.push_back(move(
                            unique_ptr<Basic_Account> {Savings_Account::LoadAccountFromFile(in_file)}
                        ));
                        break;
                    default: throw runtime_error("Unknown account code: " + n_read);
                }
            }
            
            in_file.close();
            cout << "The account data was successfully loaded." << endl;
        } catch (const exception &ex) {
            cerr << "\nexception during reading data from the file:" << endl
                 << "\"" << ex.what() << "\"" << endl
                 << "The program will be closed to avoid potencial data loss." << endl;
            return false;
        }
    } else
        cout << "File with accounts state was not found. Clear start." << endl;
    
    return true;
}


bool SaveDataToFile(const vector <unique_ptr<Basic_Account>> &accounts_vector) {
    ofstream out_file {FILE_ADDRESS};
    if (out_file) {
        out_file << Basic_Account::Get_number_accounts() << endl;
        for (auto &acc : accounts_vector)
            out_file << *acc; 
        out_file.close();
        cout << "\nThe data was saved to file." << endl;
    } else {
        cerr << "\nThe file targeted for data saving cannot be opened." << endl;
        return false;
    }
    
    return true;
}


void ShowAllAccounts(const vector <unique_ptr<Basic_Account>> &accounts_vector) {
    cout << "\nMODE: showing all accounts" << endl;
    for (auto &acc : accounts_vector)
        cout << *acc << endl; 
}


void ShowAccount(const vector <unique_ptr<Basic_Account>> &accounts_vector) {
    cout << "\nMODE: showing information on a single account" << endl;
    cout << "Enter account number: ";
    int ac_number;
    cin >> ac_number;

    bool found {false};
    for(auto &acc: accounts_vector) {
        if (*acc == ac_number) {
            cout << *acc << endl;
            found = true;
            break;
        }
    }
    if (!found)
        cout << "Account # " << ac_number << " was not found\n" << endl;
};


void AddAccount(vector <unique_ptr<Basic_Account>> &accounts_vector) {
    cout << "\nMODE: entry new account" << endl;

    cout << "Choose type of account:" << endl
         << Standard_Account::account_type << " - standard account" << endl
         << Savings_Account::account_type << " - savings account" << endl
         << "or any other number to exit from add mode: ";
    
    int ac_code;
    cin >> ac_code;
    cin.ignore(numeric_limits<streamsize>::max(),'\n');       // ignore characters remained in cin
    
    unique_ptr<Basic_Account> ac;
    switch (ac_code) {
        case Standard_Account::account_type: ac = make_unique<Standard_Account>(); break;
        case Savings_Account::account_type: ac = make_unique<Savings_Account>(); break;
        default: ac = nullptr;
    }
    
    if (ac) {
        accounts_vector.push_back(move(ac));
        cout << "Accont was successfully created" << endl;
        cout << *accounts_vector.at(accounts_vector.size()-1) << endl; 
    } else
        cout << "Cancel the add operation\n" << endl;
}


void DeleteAccount(vector <unique_ptr<Basic_Account>> &accounts_vector) {
    cout << "\nMODE: delete account" << endl;
    
    cout << "Enter account number: ";
    int ac_number;
    cin >> ac_number;

    bool found {false};
    for(size_t i{0}; i < accounts_vector.size(); i++) {
        if (*accounts_vector.at(i) == ac_number) {
            accounts_vector.erase(accounts_vector.begin()+i, accounts_vector.begin()+i+1 );
            cout << "Account was deleted.\n" << endl;
            found = true;
            break;
        }
    }
    if (!found)
        cout << "Account # " << ac_number << " was not found.\n" << endl;
}


void MakeDepositHelper(unique_ptr<Basic_Account> &acc) {
    cout << "Enter amount to deposit: ";
    int amount;
    cin >> amount;
    
    string msg;
    if ((*acc).Deposit(amount, msg)) {
        cout << "Done" << endl;
        cout << *acc << endl;
    } else {
        cout << "Problem during operation: ";
        cout << msg << "\n" << endl;
    }
}


void MakeDeposit(vector <unique_ptr<Basic_Account>> &accounts_vector) { 
    cout << "\nMODE: make deposit" << endl;
    
    cout << "Enter account number: ";
    int ac_number;
    cin >> ac_number;

    bool found {false};
    for(auto &acc: accounts_vector) {
        if (*acc == ac_number) {
            MakeDepositHelper(acc);
            found = true;
            break;
        }
    }
    if (!found)
        cout << "Account # " << ac_number << " was not found.\n" << endl;
}


void MakeWithdrawalHelper(unique_ptr<Basic_Account> &acc) {
    cout << "Enter amount to withdrawal: ";
    int amount;
    cin >> amount;
    
    string msg;
    if ((*acc).Withdrawal(amount, msg)) {
        cout << "Done" << endl;
        cout << *acc << endl;
    } else {
        cout << "Problem during operation: ";
        cout << msg << "\n" << endl;
    }
}


void MakeWithdrawal(vector <unique_ptr<Basic_Account>> &accounts_vector) { 
    cout << "\nMODE: make withdrawal" << endl;
    
    cout << "Enter account number: ";
    int ac_number;
    cin >> ac_number;

    bool found {false};
    for(auto &acc: accounts_vector) {
        if (*acc == ac_number) {
            MakeWithdrawalHelper(acc);
            found = true;
            break;
        }
    }
    if (!found)
        cout << "Account # " << ac_number << " was not found.\n" << endl;
}