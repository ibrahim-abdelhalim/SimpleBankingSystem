#ifndef __BANK_H__
#define __BANK_H__

#include <map>
#include "account.h"
#include "database.h"

class Bank
{
    private:
    std::string name; 
    Database database;
    std::map<int, Account> accounts;

    public: 
    Bank(std::string bankName = "", std::string dataBaseFileDirectory = "");
    void openAccount(std::string fname, std::string lname, long double initialBalance); 
    void closeAccount(int id);
    int getTotalAccounts();
    long double getAccountBalance(int id);
    std::string getAccountLastName(int id); 
    std::string getAccountFirstName(int id);
    void deposit(int id, long double amount);
    void withdraw(int id, long double amount);
    void showAllAccounts(); 
};

class InvalidAccount : public std::exception
{
    public:
    const char* what(); 
}; 

class EmptyName : public std::exception
{
    public: 
    const char* what(); 
}; 

class InvalidAmount : public std::exception 
{
    public: 
    const char* what(); 
};

class InvalidWithdrawAmount : public std::exception
{
    public: 
    const char* what(); 
}; 


class InvalidBalance : public std::exception 
{
    public: 
    const char* what(); 
};
#endif // #ifndef __BANK_H__