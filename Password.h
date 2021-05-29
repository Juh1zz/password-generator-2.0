#pragma once
#include <string>
#include <vector>

class Password
{
public:
    Password();
    // Constructor that retrieves a passord from pass.txt.
    //Password(const std::string& name);
    ~Password();

    std::string getName() const;
    std::string getPassword() const;
    int getLength() const;
    
    std::string getKey() const;

    // Generate a password.
    void createPass(const int& length);
    
    // Print name of service and the password for it.
    void printPass() const;

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
    // Perhaps move writing key into key file here.
    //void encryptPass();

    // Create a text file to store passwords in.
    void createFiles();
};

/*
    To do:
    - Update save() -method to first check if an entry for a service already exists. -- In progress
        - Make the search case insensitive
        - Update output to output to given line if an entry for the service name exists.
        - Do the same for the key written in milkshake.txt.
        - Make encryption it's own method.
    - Update createFiles to include key file.
    - Create constructor that finds a password for a service.
      |-> mygrep -project might come in handy. 
    - 
*/