#include "Random.hpp"

namespace ant
{

    std::mt19937 Random::s_RandomEngine;
    std::uniform_int_distribution<std::mt19937::result_type> Random::s_Distribution;

    void Random::Init()
    {
        s_RandomEngine.seed(std::random_device()());
    }

    float Random::Float()
    {
        return ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint_fast32_t>::max());
    }   
    
}