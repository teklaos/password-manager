#include <fmt/ranges.h>
#include <iostream>
#include <string>
#include <vector>
#include "header.h"

using fmt::print, fmt::println;

int main() {
    string password;
    println("Password:");
    getline(cin, password);

    int key = 0;
    for (int i = 0; i < password.length(); i++) {
        if (i % 3 == 0)
            key += 2 * password[i];
        else if (i % 3 == 1)
            key += password[i];
        else
            key -= 2 * password[i];
    }

    string filePath;
    println("Path to source file:");
    getline(cin, filePath);

    printTime(filePath);
    vector<string> vec = getStringVector(key, filePath);
    vector<string> passwords = getPasswordsVector(vec);
    vector<string> categories = getCategoriesVector(vec);

    string input;
    println("Enter \'help\' for a list of built-in commands.");
    for (bool isRunning = true; isRunning;) {
        print("->");
        getline(cin, input);
        if (!strcasecmp(input.c_str(), "exit"))
            isRunning = false;

        else if (!strcasecmp(input.c_str(), "search pass"))
            searchPasswords(vec);

        else if (!strcasecmp(input.c_str(), "sort pass"))
            sortPasswords(vec);

        else if (!strcasecmp(input.c_str(), "add pass"))
            addPassword(key, filePath, vec, passwords, categories);

        else if (!strcasecmp(input.c_str(), "edit pass"))
            editPassword(key, filePath, vec, passwords, categories);

        else if (!strcasecmp(input.c_str(), "rm pass"))
            removePassword(key, filePath, vec, passwords);

        else if (!strcasecmp(input.c_str(), "add ctg"))
            addCategory(categories);

        else if (!strcasecmp(input.c_str(), "rm ctg"))
            removeCategory(key, filePath, vec, passwords, categories);

        else if (!strcasecmp(input.c_str(), "help"))
            printList();

        else if (!input.empty())
            println("Enter \'help\' for a list of built-in commands.");
    }
}