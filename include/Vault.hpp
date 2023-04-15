#ifndef VAULT_H
#define VAULT_H

#include <string>
#include <iostream>
#include <User.hpp>
#include <Credential.hpp>
#include "SQLite3/Database.hpp"

class Vault{
        SQLite3::Database _db;
    public:
        Vault();
        ~Vault();

        User login(const std::string& email, const std::string& password);
        void saveVault();
        void exportVault();
        void importVault();
        void addCredential(Credential credential);
        void deleteCredential(const std::string& name);
        bool isOpen() const;
        SQLite3::error_code getError() const;
};

#endif