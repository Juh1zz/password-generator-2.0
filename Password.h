#pragma once
#include <string>
#include <vector>

class Password
{
public:
    // Retrieve password from passwordFile.
    Password();
    Password(const std::string &service);

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
    - Create constructor that finds a password for a service.
      |-> Search used in save() should do the trick.
    - Find a way to encrypt password. Make myself or use existing library?
        - Make encryption it's own method.
*/