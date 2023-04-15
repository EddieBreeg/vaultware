#pragma once

#include <inttypes.h>

/* A fast and reliable PRNG, using xoshiro256ss.This class is a singleton : 
the instance method should be used to access the actual generator object
*/
class RNG
{
private:
    uint64_t _state[4];
    RNG();
public:
    using result_type = uint64_t;
    // The minimum value returned by the generator
    static constexpr result_type min() { return 0; }
    // The maximum value returned by the generator
    static constexpr result_type max() { return (result_type)-1; }
    // Produces the next value in the sequence
    result_type operator()();

    // Reseeds the generator
    void seed(uint64_t s);

    RNG(const RNG&) = delete;
    RNG& operator=(const RNG&) =  delete;

    // Access the generator instance
    static RNG& instance();
        
    ~RNG() = default;
};    
