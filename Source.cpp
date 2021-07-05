#include <iostream>
#include "Password.h"


int main() {
    // Main function for Password generator 2.0

    // Placeholder function for testing purposes.
    std::string service;
    std::cout << "Enter name of service: ";
    std::cin >> service;
    Password pass(service);
    //pass.create(10);
    pass.print();
    pass.save();

    return 0;
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