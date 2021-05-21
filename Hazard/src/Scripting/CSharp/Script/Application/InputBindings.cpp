#pragma once

#include "hzrpch.h"
#include "InputBindings.h"
#include "Hazard/Events/Input.h"

namespace Hazard::Scripting::CSharp::Bindings {

    BindMap InputBindings::GetBindings()
    {
        BindMap result;
        result.emplace_back("Hazard.Input::Input_KeyDown_Native",      Input_KeyDown_Native);
        result.emplace_back("Hazard.Input::Input_KeyPressed_Native",   Input_KeyPressed_Native);
        result.emplace_back("Hazard.Input::Input_KeyReleased_Native",  Input_KeyReleased_Native);
        return result;
    }
    bool InputBindings::Input_KeyDown_Native(uint16_t key)
    {
        return Input::IsKeyDown(key);
    }
    bool InputBindings::Input_KeyPressed_Native(uint16_t key)
    {
        return Input::IsKeyPressed(key);
    }
    bool InputBindings::Input_KeyReleased_Native(uint16_t key)
    {
        return false;
    }
}