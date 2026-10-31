#include "bankapp.h"
 
#define QUIT_CODE                                    (0)

BankApp::BankApp(std::string bankName, std::string dataBaseFileDirectoy): bank(bankName, dataBaseFileDirectoy), state(SHOW_MENU)
{
}

void BankApp::run()
{
    int id = 0; 

    switch (state)
    {
        case SHOW_MENU:
        {
            int choice = 0; 
            display.put("\n\tSelect one option below ");
            display.put("\n\t1 Open an Account");
            display.put("\n\t2 Close an Account");
            display.put("\n\t3 Balance Enquiry");
            display.put("\n\t4 Deposit");
            display.put("\n\t5 Withdrawal");
            display.put("\n\t6 Show All Accounts");
            display.put("\n\t7 Quit");
            display.put("\nEnter your choice: ");
            choice = input.get<int>();

            state = static_cast<State>(choice); 
            break;
        }
        case OPEN_ACCOUNT: 
        {
            std::string fname = ""; 
            std::string lname = ""; 
            long double initialBalance = 0.0L; 

            input.flush(); 

            display.put("Enter First Name: "); 
            fname = input.get<std::string>(); 
            
            display.put("Enter Second Name: ");
            lname = input.get<std::string>();
            
            display.put("Enter Initial Balance: ");
            initialBalance = input.get<long double>(); 
            
            try 
            {
                bank.openAccount(fname, lname, initialBalance); 
                display.put("Congradulation Account Is Created.\n");
            }
            catch (EmptyName& err)
            {
                display.put(err.what()); 
                display.put("\nAccount creation is failed.\n");
            }
            catch (InvalidBalance& err)
            {
                display.put(err.what()); 
                display.put("\nAccount creation is failed.\n");
            }
            catch (EmptyFileDirectory& err) 
            {
                display.put(err.what()); 
                display.put("\nAccount creation is failed.\n");
            }
            catch (EmptyAccounts& err)
            {
                display.put(err.what()); 
                display.put("\nAccount creation is failed.\n");
            }
            catch (OpenFileStorage& err) 
            {
                display.put(err.what()); 
                display.put("\nAccount creation is failed.\n");
            }
            catch (...)
            {
                display.put("#Error: Undefined error.");
            }
            
            state = SHOW_MENU; 
            break; 
        }
        case CLOSE_ACCOUNT:
        {
            display.put("Enter Account ID: "); 
            id = input.get<int>(); 
            
            try 
            {
                bank.closeAccount(id);
            }
            catch (InvalidAccount& err) 
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (EmptyFileDirectory& err) 
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (EmptyAccounts& err)
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (OpenFileStorage& err) 
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (...)
            {
                display.put("#Error: Undefined error.");
            }
            
            state = SHOW_MENU;
            break;
        }
        case BALANCE_ENQUIRY: 
        {
            std::string fname = ""; 
            std::string lname = ""; 
            long double balance = 0.0L; 

            display.put("Enter Account ID: "); 

            id = input.get<int>();
            
            display.put("\nYour Account Details\n");

            try 
            {
                fname = bank.getAccountFirstName(id);
                lname = bank.getAccountLastName(id); 
                balance = bank.getAccountBalance(id); 
                
                display.put("\nFirst Name: "); 
                display.put(fname); 
                
                display.put("\nLast Name: "); 
                display.put(lname); 
                
                display.put("\nBalance: "); 
                display.put(std::to_string(balance)); 

            }
            catch (InvalidAccount& err)
            {
                display.put(err.what());
                display.put("\n"); 
            }

            state = SHOW_MENU;
            break;
        }
        case DEPOSIT: 
        {
            long double amount = 0.0; 
            
            display.put("Enter Account ID: "); 
            id = input.get<int>(); 
            
            display.put("Enter Balance: ");
            amount = input.get<long double>(); 

            try 
            {
                bank.deposit(id, amount); 
            }
            catch (InvalidAccount& err)
            {
                display.put(err.what());
                display.put("\n"); 
            }
            catch (InvalidAmount& err)
            {
                display.put(err.what());
                display.put("\n"); 
            }
            catch (EmptyFileDirectory& err) 
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (EmptyAccounts& err)
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (OpenFileStorage& err) 
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (...)
            {
                display.put("#Error: Undefined error.");
            }
            
            state = SHOW_MENU; 
            break;
        }
        case WITHDRAWAL: 
        {
            long double amount = 0.0; 
            display.put("Enter Account ID: "); 
            id = input.get<int>(); 
            
            display.put("Enter Balance: ");
            amount = input.get<long double>(); 

            try 
            {
                bank.withdraw(id, amount); 
            }
            catch (InvalidAccount& err)
            {
                display.put(err.what());
                display.put("\n"); 
            }
            catch (InvalidAmount& err)
            {
                display.put(err.what());
                display.put("\n"); 
            }
            catch (InvalidWithdrawAmount& err) 
            {
                display.put(err.what());
                display.put("\n"); 
            }
            catch (EmptyFileDirectory& err) 
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (EmptyAccounts& err)
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (OpenFileStorage& err) 
            {
                display.put(err.what()); 
                display.put("\n"); 
            }
            catch (...)
            {
                display.put("#Error: Undefined error.");
            }

            state = SHOW_MENU;
            break;
        }
        case SHOW_ALL_ACCOUNTS: 
        {
            try
            {
                bank.showAllAccounts();
            }
            catch (EmptyAccounts& err)
            {
                display.put(err.what());
                display.put("\n"); 
            }
            state = SHOW_MENU; 
            break; 
        }
        case QUIT: 
        {
            std::exit(QUIT_CODE); 
            break;
        }
        default:
        {
            display.put("Plz, Enter Correct Choice!");
            state = SHOW_MENU;
            break;
        }
    }
}  