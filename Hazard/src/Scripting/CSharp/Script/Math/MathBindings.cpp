#pragma once

#include "hzrpch.h"
#include "MathBindings.h"
#include "Hazard/Events/Input.h"

namespace Hazard::Scripting::CSharp::Bindings {

    BindMap MathBindings::GetBindings()
    {
        BindMap result;
        result.reserve(5);
        result.emplace_back("Hazard.Math::Math_Random_Native", Math_Random_Native);
        result.emplace_back("Hazard.Math::Math_RandomRange_Native", Math_RandomRange_Native);
        result.emplace_back("Hazard.Math::Math_Pow_Native", Math_Pow_Native);
        result.emplace_back("Hazard.Math::Math_Min_Native", Math_Pow_Native);
        result.emplace_back("Hazard.Math::Math_Max_Native", Math_Pow_Native);
        return result;
    }
    float MathBindings::Math_Random_Native()
    {
        return Random::Range(0.0f, 1.0f);
    }
    float MathBindings::Math_RandomRange_Native(float min, float max)
    {
        return Random::Range(min, max);
    }

    float MathBindings::Math_Pow_Native(float value, float pow)
    {
        return Math::Pow(value, pow);
    }

    float MathBindings::Math_Sqrt_Native(float value)
    {
        return Math::Sqrt(value);
    }
    float MathBindings::Math_Min_Native(float first, float second)
    {
        return Math::Min(first, second);
    }
    float MathBindings::Math_Max_Native(float first, float second)
    {
        return Math::Max(first, second);
    }
}