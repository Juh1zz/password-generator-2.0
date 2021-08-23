#include "menu_options.h"

void createPassword() {
    // Ask the user for name of service.
    std::string service;
    askAgain:
    try {
        std::cout << "Enter name of service: ";
        std::cin >> service;
        if (std::cin.fail())
            throw 10;
    }
    catch (int errNo) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Input failed. Please try again.\n";
        goto askAgain;
    }
    Password password(service);

    // Ask the user to enter length of password.
    askAgain2:
    int length = 0;
    try {
        std::cout << "Password must be atleast 8 characters long.\n";
        std::cout << "Input length of password: ";
        std::cin >> length;
        if (std::cin .fail() || length < 8)
            throw 10;
    }
    catch (int errNo) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (errNo == 10)
            std::cerr << "Input failed. Please try again.\n";
        goto askAgain2;
    }

    password.create(length);
    password.print();

    // Ask the user if they want to save their new password or create a new one.
    char shouldSave;
    char generateNew = 'n';
    askAgain3:
    try {
        std::cout << "Would you like to save your new password? (y/n): ";
        std::cin >> shouldSave;
        if (std::cin.fail())
            throw 10;
        if (shouldSave == 'n' || shouldSave == 'N') {
            std::cout << "Would you like to generate a new one? (y/n): ";
            std::cin >> generateNew;
            if (std::cin.fail())
                throw 10;
        }

    }
    catch (int errNo) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Input failed. Please try again.\n";
        goto askAgain3;
    }

    if (shouldSave == 'y' || shouldSave == 'Y') {
        password.save();
        std::cout << "Password saved.\n";
    }
    else if (generateNew == 'y' || generateNew == 'Y') {
        password.create(length);
        password.print();
        goto askAgain3;
    }
}

void readPassword() {
    std::string service;
    askAgain:
    try {
        std::cout << "Enter name of service: ";
        std::cin >> service;
        if (std::cin.fail())
            throw 10;
    }
    catch (int errNo) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Error " << errNo << ": Input failed. Please try again.\n";
        goto askAgain;
    }

    Password password(service);

    password.print();
}

void changePassword(bool& randomGen) {
    std::string service;
    askAgain:
    try {
        std::cout << "Enter name of service: ";
        std::cin >> service;
        if (std::cin.fail())
            throw 10;
    }
    catch (int errNo) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cerr << "Input failed. Please try again.\n";
        goto askAgain;
    }
    Password password(service);

    if (randomGen == false) {
        std::string newPass;
        askAgain2:
        try {
            std::cout << "Your password must be atleast 8 characters long.\n";
            std::cout << "Enter new password: ";
            std::cin >> newPass;
            if (std::cin.fail())
                throw 10;
            if (newPass.size() < 8)
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
                    std::cerr << "Password too short.\n";
                }
            goto askAgain2;
        }
        password.setPassword(newPass);
        password.save();
        return;
    }

    int length = 0;
    askAgain3:
    try {
        std::cout << "Your new password must be atleast 8 characters long.\n";
        std::cout << "Enter length of password: ";
        std::cin >> length;
        if (std::cin.fail())
            throw 10;
        if (length < 8)
            throw 20;
    }
    catch (int errNo) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (errNo) {
            case 10:
                std::cerr << "Input failed, please try again.\n";
                break;
            case 20:
                std::cerr << "Password is too short.\n";
            }
        goto askAgain3;
     }

    password.create(length);
    password.print();

    char savePass;
    askAgain4:
    try {
        std::cout << "Do you want to save your new password? (y/n): ";
        std::cin >> savePass;
        if (std::cin.fail())
            throw 10;
    }
    catch (int errNo) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (errNo == 10)
            std::cerr << "Input failed. Please try again.\n";
        goto askAgain4;
    }
    password.save();
}