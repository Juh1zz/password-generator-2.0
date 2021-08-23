#include <iostream>
#include <limits>
#include "Password.h"
#include "menu_options.h"

int main() {
    // Main function for Password generator 2.0

    std::cout << "Welcome to Password generator 2.0.\n";
    std::cout << "This is a tool to create and store your passwords.\n";

    while(true) {
        int function = 0;
        askAgain:
        try {
            std::cout << "Choose function (1. create password, 2. read password, 3. change password, 4. Save existing password, 5. Exit): ";
            std::cin >> function;
            if (std::cin.fail())
                throw 10;
            if (function <= 0 || function >= 6)
                throw 20;
        }
        catch (int errNo) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            switch (errNo) {
            case 10:
                std::cerr << "Input failed. Please try again.\n";
                break;
            case 20:
                std::cout << "Invalid selection. Please try again.\n";
            }
            goto askAgain;
        }

        bool randomGen;
        switch (function) {
            // Case 1: Create a new password.
        case 1:
            createPassword();
            break;
            // Case 2: Read a password from file.
        case 2:
            readPassword();
            break;
            // Case 3: Change password to a random one.
        case 3:
            randomGen = true;
            changePassword(randomGen);
            break;
            // Case 4: Change password to a user input one.
        case 4:
            randomGen = false;
            changePassword(randomGen);
            break;
        case 5:
            return 0;
        }
    }
}


/*
    Menu structure:
    Welcome. Choose function (1. create password, 2. read password, 3. change password, 4. quit).
        1. Create password -> Enter length -> Generate and display password -> Ask if user wants to save 
        password -> Save password or make new one.
        |__> Create object to generate password and save to file.
        
        2. Read password -> Ask for name of service -> Print corresponding password
        |__> Create object to retrieve password and store.
        
        3. Change password -> Ask for name of service -> Create new password for service -> Ask user for
        confirmation -> replace old password in pass.txt.
        |__> Create  object to create new password and replace old one in pass.txt.

        4. Save existing password -> Ask for name of service -> Save password in pass.txt, if an entry
        exists, modify it.
        
        5. Quit program.
*/