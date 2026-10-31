#include "database.h"

Database::Database(std::string fileDirectory)
{    
    directory = fileDirectory; 
}

void Database::setDirectory(std::string fileDirectory)
{
    if ("" == fileDirectory) 
    {
        throw EmptyFileDirectory(__FILE__, __LINE__); 
    }
    else 
    {
        directory = fileDirectory;  
    }
}

void Database::saveAccounts(std::map<int, Account>& accounts)
{
    int accountsCounter = 0; 

    if (true == accounts.empty())
    {
        throw EmptyAccounts(__FILE__, __LINE__);
    }
    else 
    {
        oufile.open(directory, std::ios::trunc); 
        
        if (false == oufile.is_open())
        {
            throw OpenFileStorage(__FILE__, __LINE__); 
        }
        else 
        {
            for (auto account: accounts)
            {
                std::string fname = account.second.getFirstName(); 
                std::string lname = account.second.getLastName(); 
                long double balance = account.second.getBalance(); 
                
                oufile << accountsCounter << std::endl; 
                oufile << fname << std::endl; 
                oufile << lname << std::endl; 
                oufile << balance << std::endl;
    
                ++accountsCounter; 
            }
    
            oufile.close(); 
        }
    }
}

std::map<int, Account> Database::loadAccounts()
{
    std::map<int, Account> accounts;
    Account curAccount; 
    int accountsCounter = 0; 
    
    int id = 0; 
    std::string fname = ""; 
    std::string lname = ""; 
    long double  balance = 0.0L; 
    
    infile.open(directory); 
    
    if (false == infile.is_open())
    {
        throw OpenFileStorage(__FILE__, __LINE__); 
    }
    else 
    {
        int isEof = infile.peek(); 
    
        while (isEof != EOF)
        {
            infile >> id; 
            infile >> fname; 
            infile >> lname; 
            infile >> balance; 
            
            curAccount.setId(id); 
            curAccount.setFirstName(fname); 
            curAccount.setLastName(lname); 
            curAccount.setBalance(balance); 
    
            accounts.insert(std::pair<const int, Account&>(id, curAccount)); 
    
            ++accountsCounter; 
    
            isEof = infile.peek(); 
        }
    
        infile.close(); 
        Account::setAccountsCounter(accountsCounter); 
    }
    
    return accounts; 
}



void Database::update(std::map<int, Account>& accounts)
{
    if (true == accounts.empty())
    {
        throw EmptyAccounts(__FILE__, __LINE__); 
    }
    else 
    {
        try 
        {
            saveAccounts(accounts); 
            accounts.clear(); 
            accounts = loadAccounts(); 
        }
        catch (EmptyAccounts& err)
        {
            throw; 
        }
        catch (OpenFileStorage& err)
        {
            throw; 
        }
    }
}
