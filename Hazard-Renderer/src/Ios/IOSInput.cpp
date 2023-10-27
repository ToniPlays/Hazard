#ifdef HZR_PLATFORM_IOS

#include "Input.h"

void Input::Init(HazardRenderer::Window& window)
{
    
}
bool Input::AnyKey()
{
    return false;
}
bool Input::IsKeyDown(const Key::KeyCode& key)
{
    return false;
}
bool Input::IsKeyPressed(const Key::KeyCode& key)
{
    return false;
}
bool Input::IsKeyReleased(const Key::KeyCode& key)
{
    return false;
}
bool Input::IsMouseButtonDown(const Mouse::MouseCode& code)
{
    return false;
}
bool Input::IsMouseButtonPressed(const Mouse::MouseCode& code)
{
    return false;
}

glm::vec2 Input::GetMousePos()
{
    return { 0, 0 };
}

void Input::Update()
{
    
}

void Input::UpdateKey(uint32_t code, int state)
{
   
}
void Input::ConnectGamepad(int device)
{
    
}
void Input::DisconnectGamepad(int device)
{
    
}

bool Input::IsButtonDown(int device, const Gamepad::GamepadCode& code)
{
    return false;
}

float Input::GetAxis(int device, const Gamepad::GamepadCode& code)
{
    return 0.0f;
}
bool Input::IsAxis(int device, const Gamepad::GamepadCode& code)
{
    return false;
}
#endif
