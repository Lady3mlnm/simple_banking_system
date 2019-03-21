#include "Accounts.h"
#include "Account_Util.h"
#include <iostream>
#include <vector>
#include <fstream>    // for work with files
#include <limits>     // for numeric_limits
#include <memory>     // for unique_ptr

using namespace std;
    

void ShowMenu() {
    cout << "\nPossible actions:" << endl
         << "  M - show this Menu" << endl
         << "  A - show All accounts" << endl
         << "  I - show sIngle account" << endl
         << "  N - New account" << endl
         << "  X - delete account" << endl
         << "  D - Deposit" << endl
         << "  W - Withdrawal" << endl
         << "  Q - exit with saving to file" << endl
         << "  Z - exit without saving (undo all operations)" << endl << endl;
}


int main() {
    vector <unique_ptr<Basic_Account>> accounts_vector {};
    
    // data loading
    if (!LoadDataFromFile(accounts_vector))
        return 1;
    
    ShowMenu();
    
    char user_command;
    do {
        cout << "\nSelect futher action: ";
        cin >> user_command;
        user_command = toupper(user_command);
        cin.ignore(numeric_limits<streamsize>::max(),'\n');           // ignore characters remained in cin
        
        switch(user_command) {
            case 'M': ShowMenu(); break;
            case 'A': ShowAllAccounts(accounts_vector); break;
            case 'I': ShowAccount(accounts_vector); break;
            case 'N': AddAccount(accounts_vector); break;
            case 'X': DeleteAccount(accounts_vector); break;
            case 'D': MakeDeposit(accounts_vector); break;
            case 'W': MakeWithdrawal(accounts_vector); break;
        }
    } while (user_command != 'Q' && user_command != 'Z');

    // data saving
    if (user_command == 'Q') {
        if (!SaveDataToFile(accounts_vector))
            return 1;
    }

    cout << "\nSuccessful completion of the program." << endl;
    return 0;
}