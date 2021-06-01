#include "Password.h"
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <vector>
/*
Password::Password(const std::string service)
{	
    // Retrieve password from passwordFile.
}
*/
std::string Password::getName() const
{
    return nameOfService;
}

std::string Password::getPassword() const
{
    return password;
}

int Password::getLength() const
{
    return password.size();
}

void Password::create(const int& length)
{
    // Ask user what service the password is for.
    retryName:
    try {
        std::cout << "Service name: ";
        std::cin >> nameOfService;
        if (std::cin.fail())
            throw;
    }
    catch (...) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Input invalid. Please try again.\n";
        goto retryName;
    }

    // Initialize RNG.
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, characters.size() - 1);
    
    // Generate password.
    int randomNum = 0;
    for (int i = 0; i < length; i++) {
        randomNum = distrib(gen);
        password.push_back(characters[randomNum]);
    }
}

void Password::print() const
{
    std::cout << "Your password for " << nameOfService << " is: " << password << std::endl;
}

void Password::generateKey()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 9);

    int randomNum = 0;
    int lengthOfPassword = password.size();
    for (int i = 0; i < lengthOfPassword; i++) {
        randomNum = distrib(gen);
        key.push_back(randomNum);
    }
}

void Password::createFiles()
{
    // Check if a file exists, if not, create one.
    std::ofstream outfilePass;
    std::ofstream outfileKey;
    try {
        outfilePass.open(passwordFile);
        outfileKey.open(keyFile);
        if (outfilePass.fail() || outfileKey.fail()) {
            throw 102;
        }
    }
    catch (int errNo) {
        outfilePass.clear();
        std::cerr << "Error " << errNo << ": Failed to create file(s). Re-trying.\n";
        createFiles();
    }
    outfilePass.close();
    outfileKey.close();
}

void Password::save()
{
    // Encrypt password and name of service.
    generateKey();
    std::string encryptedPass;
    int lengthOfPassword = password.size();
    for (int i = 0; i < lengthOfPassword; i++) {
        encryptedPass.push_back(password[i] += key[i]);
    }

    //Check if "pass.txt" can be found. If not, create files.
    std::ifstream infilePass;
    again:
    try {
        infilePass.open(passwordFile);
        if (infilePass.fail())
            throw 101;
    }
    catch (int errNo) {
        infilePass.clear();
        std::cerr << "Error " << errNo << ": File(s) was not found or doesn't exist.\n";
        createFiles();
        goto again;
    }

    // Search for entries for nameOfSerice.
    std::string lineFromPassFile;
    do {
        std::cout << "Read line: " << lineFromPassFile << std::endl;
        std::size_t found = lineFromPassFile.find(nameOfService);
        if (found != std::string::npos) {
            // Write password to existing entry.
            // To be added.
            break;
        }
        // Write password to new entry.
        std::ofstream outfile(passwordFile, std::ios::app);
        outfile << "Service: " << nameOfService << std::endl;
        outfile << "Password: " << encryptedPass << std::endl;
        outfile.close();
        break;
    } while (getline(infilePass, lineFromPassFile));

    infilePass.close();

    // Write the key into a text file.
    // Move this to encryptPass() when I make it.
    std::ofstream keyfile(keyFile, std::ios::app);

    if (keyfile.is_open()) {
        std::string keyStr;
        for (int i = 0; i < lengthOfPassword; i++) 
            keyStr.push_back(key[i]);
        keyfile << "Service: " << nameOfService << std::endl;
        keyfile << "Key: " << keyStr << std::endl;
    }
    else {
        std::cout << "Couldn't open key file.\n";
    }
}