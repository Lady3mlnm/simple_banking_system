#ifndef _ACCOUNT_UTIL_H_
#define _ACCOUNT_UTIL_H_

#include "Accounts.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <limits>
#include <algorithm>
#include <memory>

using namespace std;

bool LoadDataFromFile(vector <unique_ptr<Basic_Account>> &accounts_vector);
bool SaveDataToFile(const vector <unique_ptr<Basic_Account>> &accounts_vector);
void ShowAllAccounts(const vector <unique_ptr<Basic_Account>> &accounts_vector);
void ShowAccount(const vector <unique_ptr<Basic_Account>> &accounts_vector);
void AddAccount(vector <unique_ptr<Basic_Account>> &accounts_vector);
void DeleteAccount(vector <unique_ptr<Basic_Account>> &accounts_vector);
void MakeDeposit(vector <unique_ptr<Basic_Account>> &accounts_vector);
void MakeWithdrawal(vector <unique_ptr<Basic_Account>> &accounts_vector);

#endif // _ACCOUNT_UTIL_H_