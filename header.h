#pragma once
#include <string>
using namespace std;

void printList();
void printTime(const string &filePath);

vector<string> getStringVector(const int &key, const string &filePath);
vector<string> getPasswordsVector(const vector<string> &vec);
vector<string> getCategoriesVector(const vector<string> &vec);

string getRandomPassword(const int &passwordLength, const bool &includeUpper, const bool &includeSpecial);
string getTimeStr();

void searchPasswords(const vector<string> &vec);
void sortPasswords(vector<string> vec);
void addPassword(const int &key, const string &filePath, vector<string> &vec, vector<string> &passwords,
                 const vector<string> &categories);
void editPassword(const int &key, const string &filePath, vector<string> &vec, vector<string> &passwords,
                  const vector<string> &categories);
void removePassword(const int &key, const string &filePath, vector<string> &vec, vector<string> &passwords);
void addCategory(vector<string> &categories);
void removeCategory(const int &key, const string &filePath, vector<string> &vec, vector<string> &passwords,
                    vector<string> &categories);
bool cancelInput(const string &str);