#include "Password.h"
#include <iostream>
#include <string>
#include <fstream>
#include <random>
#include <vector>

Password::Password()
{
}

Password::Password(const std::string &service)
{	
    // Retrieve password from passwordFile.
    std::ifstream passFile;
    try {
        passFile.open(passwordFile);
        if (!passFile.is_open())
            throw 404;
    }
    catch (int errNo) {
        passFile.clear();
        std::cerr << "Error " << errNo << ": Could not find or open file.\n";
        std::cout << "Creating new files.\n";
        createFiles();
    }

    // Make password lower case for case insensitive search.
    std::string lowerCaseService;
    for (char c : service) {
        lowerCaseService.push_back(tolower(c));
    }

    // Search for entries for nameOfService.
    std::string passLine;
    while (getline(passFile, passLine)) {
        // Convert passLine to lower case to make search case insensitive.
        int sizeOfPassLine = passLine.size();
        for (int i = 0; i < sizeOfPassLine; i++) {
            passLine[i] = tolower(passLine[i]);
        }
        std::string serviceFromLine = passLine.substr(8, 9);
        int found = passLine.find(lowerCaseService);
        // If an entry for the service was found, get the password for it.
        if (found != std::string::npos) {
            // Retrieve password for found service.
            getline(passFile, password);
            // Extract password from line.
            password = password.substr(10, 11);
            std::cout << "Password from line: " << password << std::endl;
            break;
        }
    }
    passFile.close();
}

std::string Password::getName() const
{
    return serviceName;
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
        std::cin >> serviceName;
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
    std::cout << "Your password for " << serviceName << " is: " << password << std::endl;
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
    tryAgain:
    try {
        infilePass.open(passwordFile);
        if (infilePass.fail())
            throw 404;
    }
    catch (int errNo) {
        infilePass.clear();
        std::cerr << "Error " << errNo << ": File(s) was not found or doesn't exist.\n";
        createFiles();
        goto tryAgain;
    }

    // Make name of service lower case.
    std::string serviceLowerCase;
    for (char c : serviceName) {
        serviceLowerCase.push_back(tolower(c));
    }

    // Search for entries for nameOfService.
    std::string lineFromPassFile;
    int passFileLine = 1;

    while (getline(infilePass, lineFromPassFile)) {
        // Convert lineFromPassFile to lower case to make search case insensitive.
        int sizeOfLineFromPassFile = lineFromPassFile.size();
        for (int i = 0; i < sizeOfLineFromPassFile; i++) {
            lineFromPassFile[i] = tolower(lineFromPassFile[i]);
        }
        std::string serviceFromLine = lineFromPassFile.substr(8, 9);
        int found = lineFromPassFile.find(serviceLowerCase);
        // If an entry for the service was found, write new password to it.
        if (found != std::string::npos) {
            // Copy password file to vector and write password to existing entry.
            std::vector<std::string> passVect;
            std::string passLine;
            std::ifstream passFile;
            passFile.open(passwordFile);
            while (getline(passFile, passLine)) {
                passVect.push_back(passLine);
            }
            passVect[passFileLine] = "Password: " + password;
            passFile.close();
            
            // Rewrite passwordFile with new password.
            std::ofstream passFileOut;
            passFileOut.open(passwordFile, std::ofstream::trunc);
            for (std::string line : passVect) {
                passFileOut << line << std::endl;
            }
            passFileOut.close();
            infilePass.close();
            return;
        }
        passFileLine++;
    }
    infilePass.close();
    std::ofstream outfile(passwordFile, std::ios::app);
    outfile << "Service: " << serviceName << std::endl;
    outfile << "Password: " << encryptedPass << std::endl;
    outfile.close();

    // Write the key into a text file.
    // Move this to encryptPass() when I make it.
    // Add editing existing entries like with passwords.
    std::ofstream keyfile(keyFile, std::ios::app);

    if (keyfile.is_open()) {
        std::string keyStr;
        for (int i = 0; i < lengthOfPassword; i++)
            keyStr.push_back(key[i]);
        keyfile << "Service: " << serviceName << std::endl;
        keyfile << "Key: " << keyStr << std::endl;
    }
    else {
        std::cout << "Couldn't open key file.\n";
    }
    keyfile.close();
}