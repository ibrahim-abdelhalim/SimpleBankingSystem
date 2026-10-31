#ifndef  __INPUT_H__
#define  __INPUT_H__

#include <iostream>
#include <string>

class Input
{
    public: 
    template<typename type> 
    type get()
    { 
        type var;
        std::cin >> var; 
        return var;
    }

    void flush(); 
};

template<> 
std::string Input::get<std::string>();

#endif // #ifndef  __INPUT_H_