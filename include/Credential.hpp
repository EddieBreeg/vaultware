#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <string>
#include <iostream>
#include <botan/stream_cipher.h>
#include <SQLite3/Statement.hpp>

class Credential{
    public:
        Credential();
        Credential(int id, std::string_view name, 
            std::string_view login, std::string_view pwd, std::string_view url,
            bool confirmPassword);
        Credential(SQLite3::QueryResult result);
        Credential ciphered(std::unique_ptr<Botan::StreamCipher>& cipher) const;
        Credential& cipher(std::unique_ptr<Botan::StreamCipher>& cipher);
        void setName(const std::string& name);
        void setLogin(const std::string& login);
        void setPassword(const std::string& pwd);
        void setUrl(const std::string& url);
        void setConfirmPassword(bool onOff);
        void setId(int id);
        const std::string& getName() const;
        const std::string& getLogin() const;
        const std::string& getPassword() const;
        const std::string& getUrl() const;
        bool getConfirmPassword() const;
        size_t size() const {
            return _name.size() + _password.size() + _login.size()
                + _url.size();
        }
        int getId() const;
        ~Credential() = default;
    private:
        int _id = 0;
        std::string _name;
        std::string _login;
        std::string _password;
        std::string _url;
        bool _confirmPassword = false;
};

#endif