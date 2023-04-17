#ifndef PASSWORDGENERATOR_H
#define PASSWORDGENERATOR_H

#include <string>
#include <iostream>
#include <random>

template<class PRNG>
class PasswordGenerator{
    PRNG& _g;
public:
    PasswordGenerator(PRNG& prng): _g(prng) {}
    std::string operator()(size_t len, bool digits, size_t minSpecials){
        static constexpr char symbols[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
        "$@.?!*%";
        std::string pwd;
        size_t start = int(!digits) * 10, end = 61 + 7 * int(minSpecials > 0);
        pwd.reserve(len);
        minSpecials = minSpecials < len ? minSpecials : len;
        std::uniform_int_distribution<size_t> dist{start, end};
        size_t s = 0;
        for(size_t i = 0; i < len; ++i){
            int j = dist(_g);
            s += j > 62;
            pwd.push_back(symbols[dist(_g)]);
        }
        dist = decltype(dist){62, 68};
        while(s < minSpecials){
            size_t i  = decltype(dist){0, len-1}(_g);
            if(!isalnum(pwd[i])) continue;
            pwd[i] = symbols[dist(_g)];
            ++s;
        }
        return pwd;
    }
};

#endif