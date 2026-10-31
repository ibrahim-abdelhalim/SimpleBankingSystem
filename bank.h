#ifndef __BANK_H__
#define __BANK_H__

#include <map>
#include "account.h"
#include "database.h"
#include "exception.h"

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

class InvalidAccount : public BankException
{
    public:
    InvalidAccount(const char* const File, const int Line)
        : BankException("#Error: Invalid Account.", File, Line) {}
}; 

class EmptyName : public BankException
{
    public: 
    EmptyName(const char* const File, const int Line)
        : BankException("#Error: Empty name.", File, Line) {}
}; 

class InvalidAmount : public BankException 
{
    public: 
    InvalidAmount(const char* const File, const int Line)
        : BankException("#Error: Invalid amount.", File, Line) {}
};

class InvalidWithdrawAmount : public BankException
{
    public: 
    InvalidWithdrawAmount(const char* const File, const int Line)
        : BankException("#Error: Invalid withdraw amount.", File, Line) {}
}; 


class InvalidBalance : public BankException 
{
    public: 
    InvalidBalance(const char* const File, const int Line)
        : BankException("#Error: Invalid balance.", File, Line) {}
};
#endif // #ifndef __BANK_H__