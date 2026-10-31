#ifndef __ACCOUNT_H__ 
#define __ACCOUNT_H__

#include <string>

class Account 
{
    private:
    static int accounts; 
    int id;
    std::string firstName; 
    std::string lastName; 
    long double balance; 
    
    public: 
    Account(std::string fname = "", std::string lname = "", long double initialBalance = 0);
    Account(const Account& account);
    int getId(); 
    void setId(int number); 
    void setFirstName(std::string fname); 
    std::string getFirstName(); 
    void setLastName(std::string lname); 
    std::string getLastName(); 
    void setBalance(long double value); 
    long double getBalance();
    static void setAccountsCounter(int value); 
    static int getAccountsCounter(); 
};

#endif // #ifndef __ACCOUNT_H__