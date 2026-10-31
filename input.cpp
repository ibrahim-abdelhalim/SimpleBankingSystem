#include "input.h"

template<> 
std::string Input::get<std::string>()
{
    std::string var; 
    std::getline(std::cin, var); 
    return var; 
}

void Input::flush() 
{
    std::cin.ignore(); 
}