#include "bank.h"

Bank::Bank(std::string bankName, std::string dataBaseFileDirectory): database(dataBaseFileDirectory)
{
    name = bankName; 
    accounts = database.loadAccounts(); 
}

void Bank::openAccount(std::string fname, std::string lname, long double  initialBalance)
{
    int accountsCounter = 0; 

    if (fname == "" || lname == "")
    {
        throw EmptyName(__FILE__, __LINE__);
    }
    else if (initialBalance <= 0.0L)
    {
        throw InvalidBalance(__FILE__, __LINE__); 
    }
    else 
    {
        Account curAccount(fname, lname, initialBalance); 
        
        accountsCounter = Account::getAccountsCounter(); 
    
        curAccount.setId(accountsCounter);
    
        accounts.insert(std::pair<const int, Account&>(accountsCounter, curAccount)); 
    
        ++accountsCounter; 
    
        Account::setAccountsCounter(accountsCounter);
        
        database.update(accounts); 
    }
}

void Bank::closeAccount(int id)
{
    std::map<int, Account>::iterator itr; 

    itr = accounts.find(id);

    if (itr == accounts.end()) 
    {
        throw InvalidAccount(__FILE__, __LINE__); 
    }
    else 
    {
        accounts.erase(itr); 
    
        database.update(accounts); 
    }
}

int Bank::getTotalAccounts()
{
    int totalAccounts  = 0;

    totalAccounts = Account::getAccountsCounter(); 

    return totalAccounts; 
}

long double  Bank::getAccountBalance(int id)
{
    std::map<int, Account>::iterator itr; 
    long double balance = 0.0L; 

    itr = accounts.find(id);

    if (itr == accounts.end()) 
    {
        throw InvalidAccount(__FILE__, __LINE__); 
    }
    else 
    {
        balance = accounts[id].getBalance();
    }

    return balance; 
}

std::string Bank::getAccountLastName(int id)
{
    std::map<int, Account>::iterator itr; 
    std::string lname = ""; 
    
    itr = accounts.find(id);

    if (itr == accounts.end()) 
    {
        throw InvalidAccount(__FILE__, __LINE__); 
    }
    else 
    {
        lname = accounts[id].getLastName();
    }

    return lname; 
}

std::string Bank::getAccountFirstName(int id)
{
    std::map<int, Account>::iterator itr; 
    std::string fname = ""; 
    
    itr = accounts.find(id);

    if (itr == accounts.end())
    {
        throw InvalidAccount(__FILE__, __LINE__); 
    }
    else 
    {
        fname = accounts[id].getFirstName();
    }

    return fname; 
}

void Bank::deposit(int id, long double amount)
{
    std::map<int, Account>::iterator itr;
    long double curAmount = 0.0L; 
    
    itr = accounts.find(id); 

    if (itr == accounts.end())
    {
        throw InvalidAccount(__FILE__, __LINE__); 
    }
    else if (amount <= 0)
    {
        throw InvalidAmount(__FILE__, __LINE__); 
    }
    else 
    {
        curAmount = accounts[id].getBalance(); 

        curAmount += amount; 

        accounts[id].setBalance(curAmount); 

        database.update(accounts); 
    }
}

void Bank::withdraw(int id, long double amount)
{
    std::map<int, Account>::iterator itr; 
    long double curAmount = 0.0L; 

    itr = accounts.find(id); 

    if (itr == accounts.end())
    {
        throw InvalidAccount(__FILE__, __LINE__); 
    }
    else if (amount <=  0) 
    {
        throw InvalidAmount(__FILE__, __LINE__); 
    }
    else 
    {
        curAmount = accounts[id].getBalance(); 
        
        curAmount -= amount; 

        if (curAmount <  0)
        {
            throw InvalidWithdrawAmount(__FILE__, __LINE__); 
        }
        else 
        {
            accounts[id].setBalance(curAmount); 

            database.update(accounts); 
        }
        
    }
}

void Bank::showAllAccounts()
{
    if (true == accounts.empty()) 
    {
        throw EmptyAccounts(__FILE__, __LINE__);  
    }
    else 
    {
        for (auto account: accounts)
        {
            std::cout << "Account ID : " << account.first << std::endl; 
            std::cout << "Account first name: " <<  account.second.getFirstName() << std::endl; 
            std::cout << "Account second name: " << account.second.getLastName() << std::endl; 
            std::cout << "Account balance : " << account.second.getBalance() << std::endl;
            std::cout << "----------------" << std::endl;
        }
    }
}

