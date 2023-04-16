#include <Vault.hpp>
#include <filesystem>
#include <stdlib.h>

#include "Log.h"
#include <cassert>
#include <botan/pwdhash.h>
#include <botan/argon2.h>
#include <botan/system_rng.h>

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
bool Vault::login(const std::string& email, const std::string& password){
    std::vector<uint8_t> buf(email.size() + password.size());
    // concatenate the password and email
    auto it = std::copy(password.begin(), password.end(), buf.begin());
    std::copy(email.begin(), email.end(), it);
    auto pbkdf = Botan::PasswordHashFamily::create("PBKDF2(SHA-256)")->from_params(250000);
    uint8_t k[32];
    pbkdf->derive_key(k, sizeof(k), (const char*)buf.data(), buf.size(), nullptr, 0);
    buf.resize(sizeof(k) + password.size());
    std::copy(std::begin(k), std::end(k), it);
    std::string h = Botan::argon2_generate_pwhash((const char*)buf.data(),
        buf.size(), Botan::system_rng(), 1, 1<<20, 1);
    auto stmt = _db.createStatement("select * from users where email=? and authHash=?");
    SQLite3::error_code ec;
    stmt.bindParams(email, h);
    auto res = stmt();
    // if user wasn't found, login failed
    if(ec == SQLite3::SQLite3Error::Done) return false;
    return true;
}

Vault::Vault(): _db(vaultPath().u8string())
{
    _db.toggleForeignKeys(true);
    _db.execute("create table if not exists users "
        "(id integer primary key, "
        "email varchar unique not null, "
        "authHash text(128) not null,"
        "iv blob not null default x'0000000000000000');");
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