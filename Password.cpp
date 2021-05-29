#include "Password.h"
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <vector>

Password::Password()
{	
}

Password::~Password()
{
}

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

// Turn vector into string and return.
std::string Password::getKey() const
{
    std::string stringKey;
    int lengthOfKey = key.size();
    for (int i = 0; i < lengthOfKey; i++)
        stringKey.push_back(key[i]);

    return stringKey;
}


void Password::createPass(const int& length)
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

void Password::printPass() const
{
    std::cout << "Your password for " << nameOfService << " is: " << password << std::endl;
}

// Create encryption key.
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
    try {
        outfilePass.open(passwordFile);
        if (outfilePass.fail()) {
            throw 102;
        }
    }
    catch (int errNo) {
        outfilePass.clear();
        std::cerr << "Error " << errNo << ": Failed to create file. Re-trying.\n";
        createFiles();
    }
    outfilePass.write("Title\n", 6);
    outfilePass.close();
}


// Do a simple encryption and output password into txt file.
void Password::save()
{
    // Encrypt password and name of service.
    generateKey();
    std::string encryptedPass;
    int lengthOfPassword = password.size();
    for (int i = 0; i < lengthOfPassword; i++) {
        encryptedPass.push_back(password[i] += key[i]);
    }

    // Check for entries for the given service.
    std::ifstream infilePass;
    //Check if "pass.txt" can be found. If not, create files.
    label:
    try {
        infilePass.open(passwordFile);
        if (infilePass.fail())
            throw 101;
    }
    catch (int errNo) {
        infilePass.clear();
        std::cerr << "Error " << errNo << ": File(s) was not found or doesn't exist.\n";
        createFiles();
        goto label;
    }

    // Code inside this is not being run!!!
    // Search for entries for nameOfSerice.
    std::string lineFromPassFile;
    while (getline(infilePass, lineFromPassFile)) {
        std::size_t found = lineFromPassFile.find(nameOfService);
        if (found != std::string::npos) {
            // Write password to existing entry.
            // To be added.
            break;
        }
        // Write password to new entry.
        std::ofstream outfile(passwordFile, std::ios::app);
        outfile << "Service: " << nameOfService << std::endl;
        outfile << "Password: " << encryptedPass << std::endl << std::endl;
        outfile.close();
        break;
    }
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