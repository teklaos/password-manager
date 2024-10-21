#include <fmt/ranges.h>
#include <string>
#include <vector>
#include <fstream>
#include "header.h"

using fmt::println;

/// \brief Prints all the available commands
void printList() {
    println("Search pass - returns passwords that contain specific parameters.");
    println("Sort pass - returns a sorted list of all passwords.");
    println("Add pass - adds a new password to the source file.");
    println("Edit pass - allows editing of data in an existing password.");
    println("Rm pass - removes a specific password in the source file.");
    println("Add category - adds a new category.");
    println("Rm category - removes the category along with all the passwords assigned to that category.");
    println("Cancel - cancels current operation.");
    println("Exit - exits the program.");
}

/// \brief Prints a timestamp of an attempt to decrypt the file in the source file
/// \param filePath Path to the source file
void printTime(const string &filePath) {
    string timeStr = getTimeStr();
    string hours = timeStr.substr(0, 2);
    string minutes = timeStr.substr(2, 2);
    string seconds = timeStr.substr(4, 2);
    string currentTime = hours + ":" + minutes + ":" + seconds;
    println("{}", currentTime);

    auto file = fstream(filePath);
    file << currentTime << endl << endl;
    file.close();
}

/// \brief Gets current time
/// \return Current time string
string getTimeStr() {
    time_t currentTime;
    time(&currentTime);
    string timeStr = ctime(&currentTime);

    auto getDigits = [](char c) { return !isdigit(c); };
    timeStr.erase(remove_if(timeStr.begin(), timeStr.end(), getDigits), timeStr.end());
    timeStr = timeStr.substr(2, 6);
    return timeStr;
}

/// \brief Gets entries written in the source file
/// \param key Key to encode and decode the source file
/// \param filePath Path to the source file
/// \return Vector of written entries
vector<string> getStringVector(const int &key, const string &filePath) {
    vector<string> vec;
    int lineCounter = 0;
    auto file = fstream(filePath);
    for (string line; getline(file, line);) {
        if (++lineCounter > 2) {
            string decoded_line;
            for (char c: line)
                decoded_line += char(c - key);
            vec.push_back(decoded_line);
        }
    }
    file.close();
    return vec;
}

/// \brief Gets passwords used in the source file
/// \param vec Vector of entries
/// \return Vector of passwords used in the source file
vector<string> getPasswordsVector(const vector<string> &vec) {
    vector<string> passwords;
    string separator = " - ";
    for (const string &str: vec) {
        size_t first = str.find(separator);
        size_t second = str.find(separator, first + separator.length());
        string password = str.substr(first + separator.length(), second - first - separator.length());
        passwords.push_back(password);
    }
    sort(passwords.begin(), passwords.end());
    passwords.erase(unique(passwords.begin(), passwords.end()), passwords.end());
    return passwords;
}

/// \brief Gets categories used in the source file
/// \param vec  Vector of entries
/// \return Vector of categories used in the source file
vector<string> getCategoriesVector(const vector<string> &vec) {
    vector<string> categories;
    string separator = " - ";
    for (const string &str: vec) {
        size_t first = str.find(separator);
        size_t second = str.find(separator, first + separator.length());
        string category = str.substr(second + separator.length(), str.back());
        categories.push_back(category);
    }
    sort(categories.begin(), categories.end());
    categories.erase(unique(categories.begin(), categories.end()), categories.end());
    return categories;
}

/// \brief Creates a random password
/// \param passwordLength Length of the desired password
/// \param includeUpper Will the password include uppercase letters
/// \param includeSpecial Will the password include special characters
/// \return Randomly generated password
string getRandomPassword(const int &passwordLength, const bool &includeUpper, const bool &includeSpecial) {
    string password;
    string digits = "0123456789";
    string lower = "abcdefghijklmnopqrstuvwxyz";
    string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string special = "!@#$*_-";

    srand(time(nullptr));
    for (int i = 0; i < passwordLength; i++) {
        char character;
        switch (rand() % 4) {
            case 0:
                character = digits[rand() % digits.length()];
                break;
            case 1:
                character = lower[rand() % lower.length()];
                break;
            case 2:
                if (!includeUpper) {
                    i--;
                    continue;
                }
                character = upper[rand() % upper.length()];
                break;
            default:
                if (!includeSpecial) {
                    i--;
                    continue;
                }
                character = special[rand() % special.length()];
                break;
        }
        password += character;
    }
    return password;
}

/// \brief Helps user to cancel the current operation
/// \param str The input of the user
/// \return True if input = "Cancel"
bool cancelInput(const string &str) {
    if (!strcasecmp(str.c_str(), "Cancel")) {
        fmt::println("Action is cancelled.");
        return true;
    }
    return false;
}