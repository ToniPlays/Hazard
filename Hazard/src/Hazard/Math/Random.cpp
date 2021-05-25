#pragma once
#include "hzrpch.h"
#include "Random.h"
#include <random>

namespace Hazard 
{
    float Random::Range(float min, float max)
    {
        std::uniform_real_distribution<float> distrib(min, max);
        auto& mt = Engine();
        return distrib(mt);
    }
}
