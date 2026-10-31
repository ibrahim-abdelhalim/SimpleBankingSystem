#ifndef __DATABASE_H__ 
#define __DATABASE_H__ 

#include <fstream> 
#include <iostream> 
#include "account.h"
#include <map> 
#include "exception.h"

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

class EmptyFileDirectory : public BankException
{
    public: 
    EmptyFileDirectory(const char* const File, const int Line)
        : BankException("#Error: Empty file directory.", File, Line) {}
}; 

class EmptyAccounts : public BankException  
{
    public: 
    EmptyAccounts(const char* const File, const int Line)
        : BankException("#Error: Empty accounts.", File, Line) {}
}; 

class  OpenFileStorage : public BankException 
{
    public: 
    OpenFileStorage(const char* const File, const int Line)
        : BankException("#Error: Open file storage failure.", File, Line) {}
};



#endif // #ifndef __DATABASE_H__ 