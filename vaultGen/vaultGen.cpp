
#include <iostream>
#include <Vault.hpp>

int main(int argc, const char **argv)
{
    if(argc < 3){
        std::cout << "Usage: ./vaultGen <login> <password>\n";
        return 1;
    }
    Vault v;
    if(!v.isOpen()){
        std::cerr << "Couldn't open vault: " << v.getError().what() << '\n';
        return 1;
    }
    try
    {
        v.createUser(argv[1], argv[2]);
    }
    catch(const SQLite3::error_code& e)
    {
        std::cerr << "Couldn't create user: " << e.what() << '\n';
        return 1;
    }
    std::cout << "User succesfully created!\n";
    return 0;
}