#include <RNG.hpp>
#include <random>

#define rotl(x, k) ( ((x) << (k)) | ((x) >> ( 64 - (k) )) )

    static inline uint64_t splitmix64(uint64_t& x) {
        x += 0x9e3779b97f4a7c15 ;             
        uint64_t z = x;                    
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return x = (z ^ (z >> 31));                   
    }
    RNG::RNG()
    {
        std::random_device rd;
        uint64_t s = std::uniform_int_distribution<uint64_t>{}(rd);
        seed(s);
    }
    void RNG::seed(uint64_t s){
        _state[0] = splitmix64(s);
        _state[1] = splitmix64(s);
        _state[2] = splitmix64(s);
        _state[3] = splitmix64(s);
    }
    RNG& RNG::instance() {
        static RNG rng;
        return rng;
    }

    uint64_t RNG::operator()(void) {
        const uint64_t result = rotl(_state[1] * 5, 7) * 9;

        const uint64_t t = _state[1] << 17;

        _state[2] ^= _state[0];
        _state[3] ^= _state[1];
        _state[1] ^= _state[2];
        _state[0] ^= _state[3];

        _state[2] ^= t;

        _state[3] = rotl(_state[3], 45);

        return result;
    }
