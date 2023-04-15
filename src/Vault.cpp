#include <Vault.hpp>
#include <filesystem>
#include <stdlib.h>

#include "Log.h"
#include <cassert>

using std::filesystem::path;

static inline path vaultPath(){
    #if defined(_WIN32)
    size_t l = 256;
    char appdata[256];
    getenv_s(&l, appdata, "APPDATA");
    path r{appdata};
    r.append("Vaultware");
    #else
    size_t l = 256;
    char home[256];
    getenv_s(&l, home, "HOME");
    path r{home};
    r.append(".vaultware");
    #endif
    if(!std::filesystem::is_directory(r)){
        std::filesystem::create_directory(r);
    }
    r.append("vault.vw");
    return r;
}

Vault::Vault(): _db(vaultPath().u8string())
{
    _db.toggleForeignKeys(true);
    _db.execute("create table if not exists users "
        "(id integer primary key, "
        "email varchar unique not null, "
        "pwd text(128) not null);");
    _db.execute("create table if not exists credentials("
        "id integer primary key,"
        "name varchar unique not null,"
        "login varchar not null,"
        "pwd varchar not null,"
        "url varchar,"
        "confirmPwd int default 0,"
        "user_id integer not null,"
        "foreign key(user_id) references users(id));");
    _cipher = Botan::StreamCipher::create("ChaCha(20)");
    #if DEBUG
    assert(_cipher);
    #endif
}
Vault::~Vault() = default;
bool Vault::isOpen() const { return _db.isOpen(); }
SQLite3::error_code Vault::getError() const { return _db.getError(); }