#pragma once

#include "hzrpch.h"
#include "InputBindings.h"
#include "Hazard/Events/Input.h"

namespace Hazard::Scripting::CSharp::Bindings {

    BindMap InputBindings::GetBindings()
    {
        BindMap result;
        result.emplace_back("Hazard.Input::Input_Key_Down_Native", InputBindings::KeyDown);
        result.emplace_back("Hazard.Input::Input_Key_Pressed_Native", InputBindings::KeyPressed);
        result.emplace_back("Hazard.Input::Input_Key_Released_Native", InputBindings::KeyReleased);
        return result;
    }
    bool InputBindings::KeyDown(uint16_t key)
    {
        return Input::IsKeyDown(key);
    }
    bool InputBindings::KeyPressed(uint16_t key)
    {
        return false;
    }
    bool InputBindings::KeyReleased(uint16_t key)
    {
        return false;
    }
}