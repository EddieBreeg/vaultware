#ifndef VAULT_H
#define VAULT_H

#include <string>
#include <iostream>
#include <User.hpp>
#include <Credential.hpp>
#include "SQLite3/Database.hpp"
#include <vector>
#include <botan/stream_cipher.h>
#include <botan/pwdhash.h>

class Vault{
        SQLite3::Database _db;
        std::vector<Credential> _contents;
        std::unique_ptr<Botan::StreamCipher> _cipher;
        int _userId = 0;
        size_t _pos = 0; // the position in the key stream
        std::string _authHash;
        uint8_t _k[32];
        void loadVault();
        void updateCredential(size_t index);
        static constexpr struct Argon2Params_t {
            int M = 0x20000;
            int p = 1, t = 1;
        } _argon2Params {};
        void compute_key(std::string_view login, std::string_view pwd, Botan::PasswordHash& h,
            const uint8_t *iv, size_t iv_len);
    public:
        class iterator {
            const Credential* _ptr = nullptr;
            iterator() = default;
            iterator(const Credential* ptr): _ptr(ptr) {}
            friend class Vault;
        public:
            using iterator_category = std::random_access_iterator_tag;
            bool operator!=(const iterator& other) const { return _ptr != other._ptr; }
            iterator& operator++() { ++_ptr; return *this; }
            iterator operator++(int) { return iterator{_ptr++}; }
            const Credential* operator->() { return _ptr; }
            const Credential* operator->() const { return _ptr; }
            const Credential& operator*() { return *_ptr; }
            const Credential& operator*() const { return *_ptr; }
            iterator operator+(size_t n) const { return iterator{_ptr + n}; }
            iterator operator-(size_t n) const { return iterator{_ptr - n}; }
            size_t operator-(const iterator& other) { return _ptr - other._ptr; }
            iterator& operator+=(size_t n) { _ptr+=n; return *this; }
            iterator& operator-=(size_t n) { _ptr-=n; return *this; }
        };
        void saveVault();
        Vault();
        ~Vault();
        iterator begin() { return iterator{_contents.data()}; }
        iterator end() { return iterator{_contents.data() + _contents.size()}; }
        const Credential& operator[](size_t index) const { return _contents[index]; }
        bool login(const std::string& email, const std::string& password);
        bool checkPassword(std::string_view password) const;
        void exportVault();
        void importVault();
        void createUser(std::string_view user, std::string_view password);

        void addCredential(Credential&& credential);
        void deleteCredential(size_t index);
        void updateCredential(size_t index, const Credential& cred);
        bool isOpen() const;
        SQLite3::error_code getError() const;
};

#endif