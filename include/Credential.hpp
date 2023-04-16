#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <string>
#include <iostream>
#include <botan/stream_cipher.h>

class Credential{
    public:
        Credential();
        Credential(int id, std::string_view name, 
            std::string_view login, std::string_view pwd, std::string_view url,
            bool confirmPassword);
        Credential ciphered(std::unique_ptr<Botan::StreamCipher>& cipher) const;
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
        int getId() const;
        ~Credential() = default;
    private:
        int _id;
        std::string _name;
        std::string _login;
        std::string _password;
        std::string _url;
        bool _confirmPassword;
};

#endif