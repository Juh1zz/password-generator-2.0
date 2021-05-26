#include "Password.h"
#include <iostream>
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

void Password::createPassFile()
{
    // Check if a file exists, if not, create one.
    std::fstream fs;
    fs.open(passwordFile);
    if (fs.fail()) {
        std::cout << "File doesn't exits. Creating new file.\n";
        std::ofstream outfile(passwordFile);
        if (outfile.is_open()) {
            std::cout << "File created successfully.\n";
            outfile.close();
        }
        else {
            std::cout << "Failed to create file.\n";
            
        }
    }
    fs.close();
    return;
}


// Do a simple encryption and output password into txt file.
void Password::save()
{
    generateKey();

    // Encrypt password and name of service.
    std::string encryptedPass;
    
    int lengthOfPassword = password.size();
    for (int i = 0; i < lengthOfPassword; i++)
        encryptedPass.push_back(password[i] += key[i]);
    
    // Check for entries for the given service.
    std::ifstream infile;

    //Check if "pass.txt" can be found. If not, create files.
    try {
        infile.open(passwordFile);
        if (infile.fail())
            throw 101;
    }
    catch (int errNo) {
        infile.clear();
        std::cerr << "Error " << errNo << ": File was not found or doesn't exist.\n";
        createPassFile();
    }

    // Find out if an entry for the given service already exists.
    int lengthOfServiceName = nameOfService.size();
    int lineNo = 0;
    std::string Str;

    while (std::getline(infile, Str)) {
        unsigned int lengthOfStr = Str.size();
        for (int i = 0; i < lengthOfStr; i++) {
            if (Str[i] == nameOfService[0]) {
                unsigned int hitCounter = 0;
                for (int j = 0; j < lengthOfServiceName; j++) {
                    if (Str[i + j] == nameOfService[j])
                        hitCounter++;
                }
            }
        }
        lineNo++;
    }
    infile.close();

    // If an entry for the service was fount, got to it's line.
    // If not, write a new entry to the end of the file.


    // Output encrypted password and name of service.
    std::ofstream outfile(passwordFile, std::ios::app);
    
    if (outfile.is_open()) {
        outfile << "Service: " << nameOfService << std::endl;
        outfile << "Password: " << encryptedPass << std::endl << std::endl;
        outfile.close();
    }
    else {
        createPassFile();
        save();
    }

    // Write the key into a text file.
    std::ofstream keyfile("milkshake.txt", std::ios::app);

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