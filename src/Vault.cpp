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
    
    auto& rng = Botan::system_rng();
    auto stmt = _db.createStatement("select * from users where email=?");
    SQLite3::error_code ec;
    stmt.bindParams(email);
    auto res = stmt(ec);
    // if user wasn't found, login failed
    if(ec == SQLite3::SQLite3Error::Done) return false;

    SQLite3::Blob iv = res.at<SQLite3::Blob>(3);
    std::vector<uint8_t> buf(email.size() + password.size());
    // concatenate the password and email
    auto it = std::copy(password.begin(), password.end(), buf.begin());
    std::copy(email.begin(), email.end(), it);
    auto argon2 = Botan::PasswordHashFamily::create("Argon2id")->from_params(0x20000, 1, 1);
    argon2->derive_key(_k, sizeof(_k), (const char*)buf.data(), buf.size(), iv.data(), iv.size());

    _authHash = std::string(res.at<std::string_view>(2));
    if(!checkPassword(password)) // wrong password
        return false;
    _userId = res.at<int>(0);
    _cipher->set_key(_k, sizeof(_k));
    _cipher->set_iv(iv.data(), iv.size());

    loadVault();

    return true;
}
bool Vault::checkPassword(std::string_view pwd) const {
    std::vector<uint8_t> buf(pwd.size() + sizeof(_k));
    auto it = std::copy(pwd.begin(), pwd.end(), buf.begin());
    std::copy(_k, _k+sizeof(_k), it);
    return Botan::argon2_check_pwhash((char*)buf.data(), buf.size(), _authHash);
}
void Vault::updateCredential(size_t index, const Credential& c){
    _contents[index] = c; // update the credential
    updateCredential(index); // update the database
}
void Vault::updateCredential(size_t i) {
    if(i >= _contents.size()) return;
    // find the keystream position
    size_t pos = 0;
    for(size_t j = 0; j < i; ++j)
        pos += _contents[j].size();
    _cipher->seek(pos);
    const auto& c = _contents[i]; 
    auto data = c.ciphered(_cipher);
    _cipher->seek(_pos); // reset the keystream positon to the last one
    SQLite3::error_code ec;
    auto stmt = _db.createStatement("update credentials"
    "set name=?, login=?, pwd=?, confirmPwd=?, url=? where id=?", ec);
    #if DEBUG
    if(ec){
        throw ec;
    }
    #endif
    stmt.bindParams(
        std::string_view{data.getName()}, 
        std::string_view{data.getLogin()}, 
        std::string_view{data.getPassword()},
        c.getConfirmPassword(),
        std::string_view{data.getUrl()}, c.getId());
    stmt(ec);
    if(ec != SQLite3::SQLite3Error::Done){
        throw ec;
    }

}
void Vault::saveVault(){
    _cipher->seek((_pos = 0));

    for(const Credential& c: _contents){
        auto enc = c.ciphered(_cipher);
        _pos += enc.size();
        auto stmt = _db.createStatement("update credentials set "
        "name=?, login=?, pwd=?, url=?, confirmPwd=? where id=?");
        stmt.bindParams(
            std::string_view{enc.getName()},
            std::string_view{enc.getLogin()},
            std::string_view{enc.getPassword()},
            std::string_view{enc.getUrl()},
            int(enc.getConfirmPassword()),
            enc.getId()
        );
        SQLite3::error_code ec;
        stmt(ec);
        if(ec != SQLite3::SQLite3Error::Done)
            throw ec;
    }
}
void Vault::loadVault() {
    SQLite3::error_code ec;
    auto stmt = _db.createStatement("select id, name, login, pwd, url, confirmPwd from "
    "credentials where user_id=?", ec);
    #if DEBUG
    if(ec){
        throw ec;
    }
    #endif
    stmt.bindParam(1, _userId);
    _cipher->seek(0);
    auto res = stmt(ec);
    for(; ec == SQLite3::SQLite3Error::Row; res = stmt(ec)){
        Credential c(res);
        _contents.emplace_back(
            std::move(c.cipher(_cipher))
        );
        _pos += c.size();
    }
    if(ec != SQLite3::SQLite3Error::Done){
        throw ec;
    }
}
void Vault::addCredential(Credential&& c) {
    SQLite3::error_code ec;
    auto stmt = _db.createStatement("insert into credentials "
    "(name, login, pwd, url, confirmPwd, user_id) values "
    "(?, ?, ?, ?, ?, ?)", ec);
    #if DEBUG
    if(ec){
        throw ec;
    }
    #endif
    auto data = c.ciphered(_cipher);
    stmt.bindParams(
        std::string_view{data.getName()}, 
        std::string_view{data.getLogin()}, 
        std::string_view{data.getPassword()}, 
        std::string_view{data.getUrl()}, 
        (int)c.getConfirmPassword(), _userId);
    stmt(ec); // insert the new credential in the database
    if(ec && ec != SQLite3::SQLite3Error::Done){
        DEBUG_LOG(ec.what() << '\n');
        _cipher->seek(_pos);
        return;
    }
    _pos += c.size(); // update the stream position
    // retrieve the id of the inserted credential
    stmt = _db.createStatement("select id from credentials order by id desc limit 1");
    auto res = stmt(ec); 
    if(ec != SQLite3::SQLite3Error::Row)
        throw ec;
    c.setId(res.at<int>(0));
    _contents.emplace_back(c);
}
void Vault::deleteCredential(size_t index) {
    auto it = _contents.begin() + index;
    auto stmt = _db.createStatement("delete from credentials where id=?");
    stmt.bindParam(1, it->getId());
    SQLite3::error_code ec;
    stmt(ec);
    if(ec != SQLite3::SQLite3Error::Done){
        throw ec;
    }
    // update the position in the keystream
    _pos -= it->size(); 
    _cipher->seek(_pos);
    _contents.erase(it);
}

Vault::Vault(): _db(vaultPath().u8string())
{
    _db.toggleForeignKeys(true);
    auto ec = 
    _db.execute("create table if not exists users "
        "(id integer primary key, "
        "email varchar unique not null, "
        "authHash text(128) not null,"
        "iv blob not null default x'0000000000000000');");
    if(ec) throw ec;
    ec = _db.execute("create table if not exists credentials("
        "id integer primary key,"
        "name blob not null,"
        "login blob,"
        "pwd blob,"
        "url blob,"
        "confirmPwd int default 0,"
        "user_id integer not null,"
        "foreign key(user_id) references users(id));");
    if(ec) throw ec;
    _cipher = Botan::StreamCipher::create("ChaCha(20)");
    #if DEBUG
    assert(_cipher);
    #endif
}
Vault::~Vault() = default;
bool Vault::isOpen() const { return _db.isOpen(); }
SQLite3::error_code Vault::getError() const { return _db.getError(); }