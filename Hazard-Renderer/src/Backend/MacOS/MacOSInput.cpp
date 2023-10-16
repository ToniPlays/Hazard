
#include "Backend/Input.h"

#include "GLFW/glfw3.h"
#include "Backend/Core/Events/Events.h"

#include "spdlog/fmt/fmt.h"
#include <Backend/MacOS/MacOSWindow.h>
#ifdef HZR_PLATFORM_MACOS

GLFWwindow* s_Window = nullptr;
bool anyKey = false;

void Input::Init(HazardRenderer::Window& window)
{
    s_Window = (GLFWwindow*)window.GetNativeWindow();
    s_Gamepads.resize(GLFW_JOYSTICK_LAST);

    for (uint32_t i = 0; i < s_Gamepads.size(); i++)
    {
        if (glfwJoystickPresent(i))
            ConnectGamepad(i);
    }

}
bool Input::AnyKey()
{
    return anyKey;
}
bool Input::IsKeyDown(const Key::KeyCode& key)
{
    auto state = s_KeyStates[key];
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsKeyPressed(const Key::KeyCode& key)
{
    auto state = s_KeyStates[key];
    return state == GLFW_PRESS;
}
bool Input::IsKeyReleased(const Key::KeyCode& key)
{
    auto state = s_KeyStates[key];
    return state == GLFW_RELEASE;
}
bool Input::IsMouseButtonDown(const Mouse::MouseCode& code)
{
    auto state = glfwGetMouseButton(s_Window, static_cast<int32_t>(code));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsMouseButtonPressed(const Mouse::MouseCode& code)
{
    auto state = glfwGetMouseButton(s_Window, static_cast<int32_t>(code));
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePos()
{
    double xpos, ypos;
    glfwGetCursorPos(s_Window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

void Input::Update()
{
    HZR_PROFILE_FUNCTION();
    anyKey = false;
    for (uint32_t i = 0; i < 384; i++)
    {
        if (s_KeyStates[i] == GLFW_PRESS)
        {
            anyKey |= true;
            UpdateKey(i, GLFW_REPEAT);
        }
    }

    for (uint32_t g = 0; g < s_Gamepads.size(); g++)
    {
        auto& gamepad = s_Gamepads[g];
        if (!gamepad.Connected) continue;
        
        auto& info = HazardRenderer::MacOSWindow::s_CurrentWindow->GetWindowInfo();

        GLFWgamepadstate state = {};
        glfwGetGamepadState(g, &state);

        for (uint32_t i = Gamepad::AxisFirst; i <= Gamepad::AxisLast; i++)
        {
            float axisValue = state.axes[i - Gamepad::AxisFirst];

            if (glm::abs(axisValue - gamepad.Axis[i - Gamepad::AxisFirst]) <= 0.05f && axisValue != 0.0f)
            {
                axisValue = 0.0f;
                GamepadAxisMovedEvent axisEvent(g, (Gamepad::GamepadCode)i, axisValue);
                info.EventCallback(axisEvent);
                continue;
            }

            GamepadAxisMovedEvent axisEvent(g, (Gamepad::GamepadCode)i, axisValue);
            info.EventCallback(axisEvent);

            gamepad.Axis[i - Gamepad::AxisFirst] = axisValue;
        }
        for (uint32_t i = Gamepad::ButtonFirst; i < Gamepad::ButtonLast; i++)
        {
            int buttonState = state.buttons[i - Gamepad::ButtonFirst];
            int buttonLastState = gamepad.Buttons[i];

            if (buttonState == buttonLastState) continue;

            auto& info = HazardRenderer::MacOSWindow::s_CurrentWindow->GetWindowInfo();
            if (buttonState == GLFW_PRESS)
            {
                GamepadButtonPressedEvent event(g, i);
                info.EventCallback(event);
            }
            else
            {
                GamepadButtonReleasedEvent event(g, i);
                info.EventCallback(event);
            }

            gamepad.Buttons[i] = buttonState;
        }
    }
}

void Input::UpdateKey(uint32_t code, int state)
{
    if (code < 384)
        s_KeyStates[code] = state;
}
void Input::ConnectGamepad(int device)
{
    auto& gamepad = s_Gamepads[device];
    gamepad.Name = glfwGetGamepadName(device);
    gamepad.Binding = device;
    gamepad.Connected = true;

    if (glfwJoystickIsGamepad(device))
    {
        gamepad.Axis.push_back(0.0f);
        for (uint32_t i = Gamepad::AxisFirst; i <= Gamepad::AxisLast; i++)
            gamepad.Axis.push_back(0.0f);

        for (uint32_t i = Gamepad::ButtonFirst; i <= Gamepad::ButtonLast; i++)
            gamepad.Buttons.push_back(0);
    }
}
void Input::DisconnectGamepad(int device)
{
    auto& gamepad = s_Gamepads[device];
    gamepad.Connected = false;
}

bool Input::IsButtonDown(int device, const Gamepad::GamepadCode& code)
{
    auto& gamepad = s_Gamepads[device];
    return gamepad.Buttons[code];
}

float Input::GetAxis(int device, const Gamepad::GamepadCode& code)
{
    auto& gamepad = s_Gamepads[device];
    return gamepad.Axis[code - Gamepad::AxisFirst];
}
bool Input::IsAxis(int device, const Gamepad::GamepadCode& code)
{
    auto& axis = s_Gamepads[device].Axis[code - Gamepad::AxisFirst];
    return axis != 0.0f;
}
#endif
