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
    void create(const int& length);    
    void print() const;
    void save();
    
private:
    std::string serviceName;
    std::string password;
    std::vector<int> key;
    std::string passwordFile = "pass.txt";
    std::string keyFile = "milkshake.txt";
    std::string characters = "abcdefghijklmopqrstuwxyzABCDEFGHIJKLMNOPQRSTUWXYZ0123456789.,-";

    void generateKey();
    void createFiles();
    
    // Encrypt the password.
    // Move generating and writing key into key file here.
    //void encryptPass();
};

/*
    To do:
    - Update save() -method to first check if an entry for a service already exists.
        - Update output to output to given line if an entry for the service name exists.
        - Do the same for the key written in milkshake.txt.
        - Make encryption it's own method.
    - Create constructor that finds a password for a service.
      |-> mygrep -project might come in handy. 
*/