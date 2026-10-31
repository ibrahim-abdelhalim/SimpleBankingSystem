#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <exception>
#include <string>
#include <iostream>

class BankException : public std::exception
{
    private:
    std::string message;

    public:
    BankException(std::string msg, const char* const File, const int Line)
        : message(msg)
    {
        std::cout<< "File: " << File << std::endl; 
        std::cout<< "Line: " << Line << std::endl;
    }

    const char* what() const noexcept override
    {
        return message.c_str(); 
    }
};

#endif // #ifndef __EXCEPTION_H__
