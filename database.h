#ifndef __DATABASE_H__ 
#define __DATABASE_H__ 

#include <fstream> 
#include <iostream> 
#include "account.h"
#include <map> 

class Database
{
    private:
    std::ifstream infile; 
    std::ofstream oufile;  
    std::string directory; 
    
    public:
    Database(std::string fileDirectory = "");
    void setDirectory(std::string fileDirectory); 
    void saveAccounts(std::map<int, Account>&  accounts); 
    std::map<int, Account> loadAccounts(void); 
    void update(std::map<int, Account>& accounts); 

};

class EmptyFileDirectory : public std::exception
{
    public: 
    const char* what(); 
}; 

class EmptyAccounts : public std::exception  
{
    public: 
    const char* what(); 
}; 

class  OpenFileStorage : public std::exception 
{
    public: 
    const char* what(); 
};



#endif // #ifndef __DATABASE_H__ 