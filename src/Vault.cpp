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

    _cipher->set_key(k, sizeof(k));
    SQLite3::Blob iv = res.at<SQLite3::Blob>(3);
    _cipher->set_iv(iv.data(), iv.size());

    loadVault(res.at<int>(0));

    return true;
}
void Vault::saveVault(){
    _cipher->seek(0);
    SQLite3::error_code ec;
    auto stmt = _db.createStatement(
        "update credentials set name=?, login=?, pwd=?, url=?, confirmPwd=? where id=?;", ec
    );
    #if DEBUG
    if(ec){
        DEBUG_LOG(ec.what() << '\n');
        return;
    }
    #endif
    for(const Credential& c: _contents){
        Credential enc = c.ciphered(_cipher);
        stmt.bindParams(
            enc.getName(), enc.getLogin(), enc.getPassword(), enc.getUrl(), enc.getConfirmPassword(),
                enc.getId()
        );
        stmt(ec);
        if(ec){
            DEBUG_LOG(ec.what() << '\n');
        }
    }
}
void Vault::loadVault(int userId) {
    SQLite3::error_code ec;
    auto stmt = _db.createStatement("select (id, name, login, pwd, confirmPwd, url) from "
    "credentials where user_id=?", ec);
    #if DEBUG
    if(ec){
        DEBUG_LOG(ec.what() << '\n');
        return;
    }
    #endif
    stmt.bindParam(1, userId);
    _cipher->seek(0);
    auto res = stmt(ec);
    for(; ec == SQLite3::SQLite3Error::Row; res = stmt(ec)){
        Credential c(res.at<int>(0),
            res.at<std::string_view>(1),
            res.at<std::string_view>(2),
            res.at<std::string_view>(3),
            res.at<std::string_view>(4),
            res.at<int>(5));
        _contents.emplace_back(c.ciphered(_cipher));
    }
    if(ec != SQLite3::SQLite3Error::Done){
        DEBUG_LOG("Couldn't load vault: " << ec.what() << '\n');
    }
}
void Vault::addCredential(Credential&& c) {
    _contents.emplace_back(c);
    saveVault();
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
        "name varchar not null,"
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