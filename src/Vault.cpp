#include <Vault.hpp>
#include <filesystem>
#include <stdlib.h>

#include "Log.h"
#include <cassert>
#include <botan/pwdhash.h>
#include <botan/argon2.h>
#include <botan/system_rng.h>
#include <string_view>

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

    auto& rng = Botan::system_rng();
    auto stmt = _db.createStatement("select * from users where email=?");
    SQLite3::error_code ec;
    stmt.bindParams(email);
    auto res = stmt(ec);
    // if user wasn't found, login failed
    if(ec == SQLite3::SQLite3Error::Done) return false;
    std::string h(res.at<std::string_view>(2));
    if(!Botan::argon2_check_pwhash((char*)buf.data(), buf.size(), h)) // wrong password
        return false;

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
    if(ec && ec != SQLite3::SQLite3Error::Done){
        DEBUG_LOG(ec.what() << '\n');
        return;
    }
    #endif
    stmt.bindParam(1, userId);
    _cipher->seek(0);
    auto res = stmt(ec);
    for(; ec == SQLite3::SQLite3Error::Row; res = stmt(ec)){
        Credential c(res);
        _pos += c.size();
        _contents.emplace_back(
            std::move(c.cipher(_cipher))
        );
    }
    if(ec != SQLite3::SQLite3Error::Done){
        DEBUG_LOG("Couldn't load vault: " << ec.what() << '\n');
    }
}
void Vault::addCredential(Credential&& c) {
    SQLite3::error_code ec;
    auto stmt = _db.createStatement("insert into credentials "
    "(name, login, pwd, url, confirmPwd) values "
    "?, ?, ?, ?, ?", ec);
    #if DEBUG
    if(ec){
        DEBUG_LOG(ec.what() << '\n');
        return;
    }
    #endif
    auto enc = c.ciphered(_cipher);
    stmt.bindParams(enc.getName(), enc.getLogin(), enc.getPassword(), enc.getUrl(), (int)enc.getConfirmPassword());
    stmt(ec); // insert the new credential in the database
    if(ec && ec != SQLite3::SQLite3Error::Done){
        DEBUG_LOG(ec.what() << '\n');
        _cipher->seek(_pos);
        return;
    }
    _pos += enc.size(); // update the stream position
    // retrieve the id of the inserted credential
    stmt = _db.createStatement("select id from credentials order by id desc limit 1");
    auto res = stmt(ec); 
    c.setId(res.at<int>(0));
    _contents.emplace_back(c);
}
void Vault::deleteCredential(size_t index) {
    auto it = _contents.begin() + index;
    auto stmt = _db.createStatement("delete from credentials where id=?");
    stmt.bindParam(1, it->getId());
    SQLite3::error_code ec;
    stmt(ec);
    if(ec){
        DEBUG_LOG(ec.what() << '\n');
    }
    // update the position in the keystream
    _pos -= it->size(); 
    _cipher->seek(_pos);
    _contents.erase(it);
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