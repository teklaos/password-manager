#include <iostream>
#include <fmt/ranges.h>
#include <string>
#include <vector>
#include <fstream>
#include "header.h"

using namespace std;
using fmt::println;

/// \brief Searches for the passwords among the vector of entries
/// \param vec Vector of entries
void searchPasswords(const vector<string> &vec) {
    string parameter;
    println("Searching by:");
    getline(cin, parameter);
    if (cancelInput(parameter))
        return;

    for (const string &entry: vec)
        if (entry.find(parameter) != string::npos)
            cout << entry << endl;
}

/// \brief Types the sorted copy of the vector of entries
/// \param vec Vector of entries
void sortPasswords(vector<string> vec) {
    string parameter;
    println("Sorting by [N/C/N&C]:");
    for (bool invalidParameter = true; invalidParameter;) {
        invalidParameter = false;
        getline(cin, parameter);
        if (cancelInput(parameter))
            return;

        if (!strcasecmp(parameter.c_str(), "N"))
            sort(vec.begin(), vec.end());
        else if (!strcasecmp(parameter.c_str(), "C")) {
            sort(vec.begin(), vec.end(), [](const string &str1, const string &str2) -> bool {
                string separator = " - ";
                size_t first, second;
                int comp;

                first = str1.find(separator);
                second = str1.find(separator, first + separator.length());
                string category1 = str1.substr(second + separator.length(), str1.back());
                first = str2.find(separator);
                second = str2.find(separator, first + separator.length());
                string category2 = str2.substr(second + separator.length(), str2.back());
                comp = category1.compare(category2);
                if (comp >= 0)
                    return false;
                else
                    return true;
            });
        }
        else if (!strcasecmp(parameter.c_str(), "N&C")) {
            sort(vec.begin(), vec.end(), [](const string &str1, const string &str2) -> bool {
                string separator = " - ";
                size_t first, second;
                int nameComp, catComp;

                first = str1.find(separator);
                second = str1.find(separator, first + separator.length());
                string name1 = str1.substr(0, first);
                string category1 = str1.substr(second + separator.length(), str1.back());
                first = str2.find(separator);
                second = str2.find(separator, first + separator.length());
                string name2 = str1.substr(0, first);
                string category2 = str2.substr(second + separator.length(), str2.back());
                nameComp = name1.compare(name2);
                catComp = category1.compare(category2);
                if (nameComp > 0)
                    return false;
                else if (nameComp == 0) {
                    if (catComp >= 0)
                        return false;
                    else
                        return true;
                }
                else
                    return true;
            });
        }
        else {
            println("There is no such sorting parameter.");
            invalidParameter = true;
        }
    }
    for (const string &entry: vec)
        cout << entry << endl;
}

/// \brief Adds password to the source file
/// \param key Key to encode and decode the source file
/// \param filePath Path to the source file
/// \param vec Vector of entries
/// \param passwords Vector of passwords
/// \param categories Vector of categories
void addPassword(const int &key, const string &filePath, vector<string> &vec, vector<string> &passwords,
                 const vector<string> &categories) {
    string name;
    println("Enter the name of the entry:");
    getline(cin, name);
    if (cancelInput(name))
        return;

    string input, password;
    println("Would you like to make your own password? [Y/N]");
    for (bool invalidAnswer = true; invalidAnswer;) {
        invalidAnswer = false;
        getline(cin, input);
        if (cancelInput(input))
            return;

        if (!strcasecmp(input.c_str(), "Y")) {
            println("Enter the password:");
            getline(cin, password);
            if (cancelInput(password))
                return;

            for (const string &exPassword: passwords)
                if (password == exPassword)
                    println("Warning: This password is already in use!");
        }
        else if (!strcasecmp(input.c_str(), "N")) {
            int passwordLength;
            bool includeUpper, includeSpecial;
            string in;
            println("Enter length of the password [8-20]:");
            for (bool invalidInput = true, isNumber = true; invalidInput;) {
                getline(cin, in);
                if (cancelInput(in))
                    return;

                for (char c: in)
                    isNumber &= isdigit(c);
                if (isNumber) {
                    int length = stoi(in);
                    if (length >= 8 && length <= 20) {
                        passwordLength = length;
                        invalidInput = false;
                    }
                    else
                        println("Invalid length of the password.");
                }
                else {
                    println("Invalid input.");
                    isNumber = true;
                }
            }
            println("Do you want to include uppercase letters? [Y/N]");
            for (bool invalidInput = true; invalidInput;) {
                invalidInput = false;
                getline(cin, in);
                if (cancelInput(in))
                    return;

                if (!strcasecmp(in.c_str(), "Y"))
                    includeUpper = true;
                else if (!strcasecmp(in.c_str(), "N"))
                    includeUpper = false;
                else {
                    println("Invalid input.");
                    invalidInput = true;
                }
            }
            println("Do you want to include special characters? [Y/N]");
            for (bool invalidInput = true; invalidInput;) {
                invalidInput = false;
                getline(cin, in);
                if (cancelInput(in))
                    return;

                if (!strcasecmp(in.c_str(), "Y"))
                    includeSpecial = true;
                else if (!strcasecmp(in.c_str(), "N"))
                    includeSpecial = false;
                else {
                    println("Invalid input.");
                    invalidInput = true;
                }
            }
            password = getRandomPassword(passwordLength, includeUpper, includeSpecial);
            println("Created password: \"{}\".", password);
        }
        else {
            println("Invalid input.");
            invalidAnswer = true;
        }
    }
    println("Enter the category the password will be assigned to:");
    for (const string &existingCategory: categories)
        cout << existingCategory << endl;
    string category;
    for (bool noSuchCategory = true; noSuchCategory;) {
        getline(cin, category);
        if (cancelInput(category))
            return;

        for (const string &exCategory: categories)
            if (category == exCategory)
                noSuchCategory = false;
        if (noSuchCategory)
            println("There is no such category.");
    }
    passwords.push_back(password);
    sort(passwords.begin(), passwords.end());
    passwords.erase(unique(passwords.begin(), passwords.end()), passwords.end());

    string separator = " - ";
    string entry = name + separator + password + separator + category;
    cout << "Password \"" << entry << "\" was successfully added." << endl;
    vec.push_back(entry);
    string encoded_entry;
    for (char c: entry)
        encoded_entry += char(c + key);

    auto file = fstream(filePath, ios::out | ios::app);
    file << encoded_entry << endl;
    file.close();
}

/// \brief Edits password in the source file
/// \param key Key to encode and decode the source file
/// \param filePath Path to the source file
/// \param vec Vector of entries
/// \param passwords Vector of passwords
/// \param categories Vector of categories
void editPassword(const int &key, const string &filePath, vector<string> &vec, vector<string> &passwords,
                  const vector<string> &categories) {
    string separator = " - ";
    string inPassword, toChange;
    println("Enter the password you want to change:");
    for (bool invalidPassword = true; invalidPassword;) {
        getline(cin, inPassword);
        if (cancelInput(inPassword))
            return;

        for (const string &str: vec) {
            size_t first = str.find(separator);
            size_t second = str.find(separator, first + separator.length());
            string password = str.substr(first + separator.length(), second - first - separator.length());
            if (inPassword == password)
                toChange = str;
        }
        if (!toChange.empty())
            invalidPassword = false;
        else
            println("There is no such password.");
    }
    string change;
    println("What do you want to change? [Name/Password/Category]");
    for (bool invalidInput = true; invalidInput;) {
        invalidInput = false;
        getline(cin, change);
        if (cancelInput(change))
            return;

        if (!strcasecmp(change.c_str(), "Name")) {
            string newName;
            println("Enter new name:");
            getline(cin, newName);
            if (cancelInput(newName))
                return;

            for (string &str: vec) {
                size_t first = str.find(separator);
                size_t second = str.find(separator, first + separator.length());
                string password = str.substr(first + separator.length(), second - first - separator.length());
                string category = str.substr(second + separator.length(), str.back());
                if (inPassword == password) {
                    str = newName;
                    str += separator;
                    str += password;
                    str += separator;
                    str += category;
                }
            }
            println("Name was successfully changed to \"{}\".", newName);
        }
        else if (!strcasecmp(change.c_str(), "Password")) {
            string newPassword;
            println("Enter new password:");
            getline(cin, newPassword);
            if (cancelInput(newPassword))
                return;

            for (string &str: vec) {
                size_t first = str.find(separator);
                size_t second = str.find(separator, first + separator.length());
                string name = str.substr(0, first);
                string password = str.substr(first + separator.length(), second - first - separator.length());
                string category = str.substr(second + separator.length(), str.back());
                if (inPassword == password) {
                    str = name;
                    str += separator;
                    str += newPassword;
                    str += separator;
                    str += category;
                }
            }
            replace(passwords.begin(), passwords.end(), inPassword, newPassword);
            println("Password was successfully changed to \"{}\".", newPassword);
        }
        else if (!strcasecmp(change.c_str(), "Category")) {
            println("Enter new category:");
            for (const string &existingCategory: categories)
                cout << existingCategory << endl;
            string newCategory;
            for (bool noSuchCategory = true; noSuchCategory;) {
                getline(cin, newCategory);
                if (cancelInput(newCategory))
                    return;

                for (const string &exCategory: categories)
                    if (newCategory == exCategory)
                        noSuchCategory = false;
                if (noSuchCategory)
                    println("There is no such category.");
            }
            for (string &str: vec) {
                size_t first = str.find(separator);
                size_t second = str.find(separator, first + separator.length());
                string name = str.substr(0, first);
                string password = str.substr(first + separator.length(), second - first - separator.length());
                if (inPassword == password) {
                    str = name;
                    str += separator;
                    str += password;
                    str += separator;
                    str += newCategory;
                }
            }
            cout << "Category was successfully changed to \"" << newCategory << "\"." << endl;
        }
        else {
            println("There is no such parameter.");
            invalidInput = true;
        }
    }

    auto file = fstream(filePath, ios::in);
    string firstLine, secondLine;
    getline(file, firstLine);
    getline(file, secondLine);
    file.close();

    ofstream ofs;
    ofs.open(filePath, ios::out | ios::trunc);
    ofs.close();

    file.open(filePath, ios::out | ios::app);
    file << firstLine << endl;
    file << secondLine << endl;

    for (const string &entry: vec) {
        string encoded_entry;
        for (char c: entry)
            encoded_entry += char(c + key);
        file << encoded_entry << endl;
    }
    file.close();
}

/// \brief Removes password(s) from the source file
/// \param key Key to encode and decode the source file
/// \param filePath Path to the source file
/// \param vec Vector of entries
/// \param passwords Vector of passwords
void removePassword(const int &key, const string &filePath, vector<string> &vec, vector<string> &passwords) {
    string inPassword;
    println("Enter the password you want to delete:");
    getline(cin, inPassword);
    if (cancelInput(inPassword))
        return;

    vector<string> toDelete;
    string separator = " - ";
    for (const string &str: vec) {
        size_t first = str.find(separator);
        size_t second = str.find(separator, first + separator.length());
        string password = str.substr(first + separator.length(), second - first - separator.length());
        if (inPassword == password)
            toDelete.push_back(password);
    }
    if (toDelete.empty()) {
        println("There is no such password to delete.");
        return;
    }
    if (toDelete.size() > 1)
        println("Warning: There are several passwords being deleted!");
    for (string pass: toDelete) {
        erase_if(vec, [&](const string &entry) -> bool {
            size_t first = entry.find(separator);
            size_t second = entry.find(separator, first + separator.length());
            string password = entry.substr(first + separator.length(), second - first - separator.length());
            return pass == password;
        });
        erase_if(passwords, [&](const string &password) -> bool { return pass == password; });
    }

    auto file = fstream(filePath, ios::in);
    string firstLine, secondLine;
    getline(file, firstLine);
    getline(file, secondLine);
    file.close();

    ofstream ofs;
    ofs.open(filePath, ios::out | ios::trunc);
    ofs.close();

    file.open(filePath, ios::out | ios::app);
    file << firstLine << endl;
    file << secondLine << endl;

    for (const string &entry: vec) {
        string encoded_entry;
        for (char c: entry)
            encoded_entry += char(c + key);
        file << encoded_entry << endl;
    }
    file.close();
    if (toDelete.size() > 1)
        println("Passwords were successfully deleted.");
    else
        println("Password was successfully deleted.");
}

/// \brief Adds category
/// \param categories Vector of categories
void addCategory(vector<string> &categories) {
    string inCategory;
    println("Enter the name of category:");
    getline(cin, inCategory);
    if (cancelInput(inCategory))
        return;

    for (const string &category: categories)
        if (inCategory == category) {
            println("This category already exists.");
            return;
        }
    categories.push_back(inCategory);
    println("Category successfully added.");
}

/// \brief Removes category along with the passwords assigned to that category
/// \param key Key to encode and decode the source file
/// \param filePath Path to the source file
/// \param vec Vector of entries
/// \param passwords Vector of passwords
/// \param categories Vector of categories
void removeCategory(const int &key, const string &filePath, vector<string> &vec, vector<string> &passwords,
                    vector<string> &categories) {
    string inCategory;
    println("Enter the name of the category you would like to delete:");
    getline(cin, inCategory);
    if (cancelInput(inCategory))
        return;

    erase_if(categories, [&](const string &category) -> bool { return inCategory == category; });
    vector<string> toDelete;
    string separator = " - ";
    erase_if(vec, [&](const string &entry) -> bool {
        size_t first = entry.find(separator);
        size_t second = entry.find(separator, first + separator.length());
        string password = entry.substr(first + separator.length(), second - first - separator.length());
        string category = entry.substr(second + separator.length(), entry.back());
        if (inCategory == category) {
            toDelete.push_back(password);
            return true;
        }
        else
            return false;
    });
    for (string pass: toDelete)
        erase_if(passwords, [&](const string &password) -> bool { return pass == password; });

    auto file = fstream(filePath, ios::in);
    string firstLine, secondLine;
    getline(file, firstLine);
    getline(file, secondLine);
    file.close();

    ofstream ofs;
    ofs.open(filePath, ios::out | ios::trunc);
    ofs.close();

    file.open(filePath, ios::out | ios::app);
    file << firstLine << endl;
    file << secondLine << endl;

    for (const string &entry: vec) {
        string encoded_entry;
        for (char c: entry)
            encoded_entry += char(c + key);
        file << encoded_entry << endl;
    }
    file.close();
    println("Category was successfully deleted.");
}