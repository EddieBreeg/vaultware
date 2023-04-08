#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <string>
#include <iostream>

class PasswordGenerator{
    public:
        PasswordGenerator() = default;
        ~PasswordGenerator() = default;
    private:
        std::string _symbols;
        int _passwordLength;
        int _minSpecialChars;
        std::string generate();
};

#endif