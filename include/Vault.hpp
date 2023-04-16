#ifndef VAULT_H
#define VAULT_H

#include <string>
#include <iostream>
#include <User.hpp>
#include <Credential.hpp>
#include "SQLite3/Database.hpp"
#include <vector>
#include <botan/stream_cipher.h>

class Vault{
        SQLite3::Database _db;
        std::vector<Credential> _contents;
        std::unique_ptr<Botan::StreamCipher> _cipher;
        size_t _pos = 0; // the position in the key stream
        void saveVault();
        void loadVault(int userId);
    public:
        class iterator {
            const Credential* _ptr = nullptr;
            iterator() = default;
            iterator(const Credential* ptr): _ptr(ptr) {}
            friend class Vault;
        public:
            using iterator_category = std::random_access_iterator_tag;
            bool operator!=(const iterator& other) const { return _ptr == other._ptr; }
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
        Vault();
        ~Vault();
        iterator begin() { return iterator{_contents.data()}; }
        iterator end() { return begin() + _contents.size(); }
        const Credential& operator[](size_t index) const { return _contents[index]; }
        bool login(const std::string& email, const std::string& password);
        void exportVault();
        void importVault();

        void addCredential(Credential&& credential);
        void deleteCredential(size_t index);
        bool isOpen() const;
        SQLite3::error_code getError() const;
};

#endif