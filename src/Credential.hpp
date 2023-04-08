#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <string>
#include <iostream>

class Credential{
    public:
        Credential() = default;
        ~Credential() = default;
    private:
        std::string _name;
        std::string _login;
        std::string _password;
        std::string _url;
        bool _confirmPassword;
};

#endif