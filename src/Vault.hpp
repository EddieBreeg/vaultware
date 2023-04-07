#ifndef VAULT_H
#define VAULT_H

#include <string>
#include <iostream>
#include <User.hpp>
#include <Credential.hpp>

class Vault{
    public:
        Vault() = default;
        ~Vault() = default;
    private:
        User login(std::string email, std::string password);
        void save();
        void export();
        void import();
        void addCredential(Credential credential);
        void deleteCredential(std::string name);
};

#endif