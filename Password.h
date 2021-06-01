#pragma once
#include <string>
#include <vector>

class Password
{
public:
    //Password(const std::string service);

    std::string getName() const;
    std::string getPassword() const;
    int getLength() const;

    // Generate a password.
    void create(const int& length);
    
    // Print name of service and the password for it.
    void print() const;

    // Check if a password for the given service exists. It not, create an entry for it.
    // If it does exists, write the generated password to it.
    void save();
    
private:
    std::string nameOfService;
    std::string password;
    std::vector<int> key;
    std::string passwordFile = "pass.txt";
    std::string keyFile = "milkshake.txt";

    std::string characters = "abcdefghijklmopqrstuwxyzABCDEFGHIJKLMNOPQRSTUWXYZ0123456789.,-";

    // Generate a key to encrypt the password.
    void generateKey();
    
    // Encrypt the password.
    // Move generating and writing key into key file here.
    //void encryptPass();

    // Create text files to store passwords and keys.
    void createFiles();
};

/*
    To do:
    - Update save() -method to first check if an entry for a service already exists. -- In progress
        - Make the search case insensitive
        - Update output to output to given line if an entry for the service name exists.
        - Do the same for the key written in milkshake.txt.
        - Make encryption it's own method.
    - Create constructor that finds a password for a service.
      |-> mygrep -project might come in handy. 
    - 
*/