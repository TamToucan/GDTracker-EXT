#ifndef RAND_SIMPLE_H
#define RAND_SIMPLE_H

#include "I_Random.h"

namespace RNG {

class RandSimple : public I_Random
{
public:
    // constructor
    RandSimple(unsigned int seed) : I_Random()
    {
        initialise(seed);
    }

    // destructor
    virtual ~RandSimple() { }

    // re-seed
    virtual void initialise(unsigned int seed) { m_seed = (seed&0xffffffff); }

    // seed by more than 32 bits
    virtual void initialise(unsigned int seeds[], int length)
    {
        initialise(seeds[0]);
    }

    // output random float
    virtual double getFloat()
    {
        double r;
        I_Random::uint32 x = (m_seed<<13) ^ m_seed;
        r = ((x * (x*x*15731 +789221) +1376312589) & 0x7fffffff) / double(0x7fffffff);
        m_seed += static_cast<I_Random::uint32>(double(0xffffffff)*r);
        return r;
    }

private:
    I_Random::uint32 m_seed;
};

} // namespace

#endif

