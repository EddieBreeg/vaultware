#include <Credential.hpp>

Credential Credential::ciphered(std::unique_ptr<Botan::StreamCipher>& cipher) const {
    Credential c = *this;
    return c.cipher(cipher);
}
Credential& Credential::cipher(std::unique_ptr<Botan::StreamCipher>& cipher){
    cipher->cipher1((uint8_t*)_name.data(), _name.size());
    cipher->cipher1((uint8_t*)_login.data(), _login.size());
    cipher->cipher1((uint8_t*)_password.data(), _password.size());
    cipher->cipher1((uint8_t*)_url.data(), _url.size());
    return *this;
}
Credential::Credential(int id, std::string_view name, 
    std::string_view login, std::string_view pwd, std::string_view url,
    bool confirmPassword): _id(id), _name(name), _login(login),
    _password(pwd), _url(url), _confirmPassword(confirmPassword)
{}
Credential::Credential(SQLite3::QueryResult res):
    _id(res.at<int>(0)), 
    _name(res.at<std::string_view>(1)),
    _login(res.at<std::string_view>(2)),
    _password(res.at<std::string_view>(3)),
    _url(res.at<std::string_view>(4)),
    _confirmPassword(res.at<int>(5))
{}
Credential::Credential() = default; 

void Credential::setName(const std::string& name){
    _name = name;
}
void Credential::setLogin(const std::string& login){
    _login = login;
}
void Credential::setPassword(const std::string& pwd){
    _password = pwd;
}
void Credential::setUrl(const std::string& url){
    _url = url;
}
void Credential::setConfirmPassword(bool onOff){
    _confirmPassword = onOff;
}
const std::string& Credential::getName() const {
    return _name;
}
const std::string& Credential::getLogin() const {
    return _login;
}
const std::string& Credential::getPassword() const {
    return _password;
}
const std::string& Credential::getUrl() const {
    return _url;
}
bool Credential::getConfirmPassword() const {
    return _confirmPassword;
}
void Credential::setId(int id) { _id = id; }
int Credential::getId() const { return _id; }