#include "account.h"
#include <iostream>

int Account::accounts = 0; 

Account::Account(std::string fname, std::string lname, long double initialBalance)
{
    firstName = fname; 
    lastName = lname; 
    balance = initialBalance; 
    id = accounts; 
}

Account::Account(const Account& account)
{
    
    id = account.id; 
    firstName = account.firstName; 
    lastName = account.lastName; 
    balance = account.balance;
}


int Account::getId()
{
    return id; 
}

void Account::setId(int number)
{
    id = number; 
}

void Account::setFirstName(std::string fname)
{
    firstName = fname; 
}

std::string Account::getFirstName()
{
    return firstName; 
}

void Account::setLastName(std::string lname)
{
    lastName = lname;
}

std::string Account::getLastName()
{
    return lastName; 
}

void Account::setBalance(long double value)
{
    balance = value; 
}

long double Account::getBalance()
{
    return balance;
}

void Account::setAccountsCounter(int value)
{
    accounts = value; 
}

int Account::getAccountsCounter()
{
    return accounts; 
}