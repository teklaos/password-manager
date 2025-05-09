#include <fmt/ranges.h>
#include <iostream>
#include <string>
#include <vector>
#include "header.h"

using fmt::print, fmt::println;

int main() {
    string password;
    println("Enter the password for the file:");
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
    println("Enter the name of the source file:");
    getline(cin, filePath);

    printTime(filePath);
    vector<string> vec = getStringVector(key, filePath);
    vector<string> passwords = getPasswordsVector(vec);
    vector<string> categories = getCategoriesVector(vec);

    string input;
    println("Enter a command (enter 'help' for a list of commands):");
    for (bool isRunning = true; isRunning;) {
        print("->");
        getline(cin, input);
        if (!strcasecmp(input.c_str(), "Exit"))
            isRunning = false;

        else if (!strcasecmp(input.c_str(), "Search pass"))
            searchPasswords(vec);

        else if (!strcasecmp(input.c_str(), "Sort pass"))
            sortPasswords(vec);

        else if (!strcasecmp(input.c_str(), "Add pass"))
            addPassword(key, filePath, vec, passwords, categories);

        else if (!strcasecmp(input.c_str(), "Edit pass"))
            editPassword(key, filePath, vec, passwords, categories);

        else if (!strcasecmp(input.c_str(), "Rm pass"))
            removePassword(key, filePath, vec, passwords);

        else if (!strcasecmp(input.c_str(), "Add category"))
            addCategory(categories);

        else if (!strcasecmp(input.c_str(), "Rm category"))
            removeCategory(key, filePath, vec, passwords, categories);

        else if (!strcasecmp(input.c_str(), "Help"))
            printList();

        else if (!input.empty())
            println("Enter \'Help\' for a list of built-in commands");
    }
}