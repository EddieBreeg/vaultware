#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>

class User{
    public:
        User() = default;
        ~User() = default;
    private:
        std::string _email;
        std::string _passwordHash;
};

#endif