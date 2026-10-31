#ifndef __BANKAPP_H__
#define __BANKAPP_H__

#include <iostream>
#include "input.h"
#include "bank.h"
#include "display.h"

class BankApp
{
    private:
enum State 
{
    SHOW_MENU, 
    OPEN_ACCOUNT, 
    CLOSE_ACCOUNT, 
    BALANCE_ENQUIRY,
    DEPOSIT, 
    WITHDRAWAL,
    SHOW_ALL_ACCOUNTS, 
    QUIT, 
    NONE
};
    Input input; 
    Bank bank;
    Display display;
    State state; 
    
    public: 
    BankApp(std::string bankName = "", std::string dataBaseFileDirectory = "");
    void run();  
};



#endif // #ifndef __BANKAPP_H__