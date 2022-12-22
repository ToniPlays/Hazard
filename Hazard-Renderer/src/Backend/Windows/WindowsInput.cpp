
#include "Backend/Input.h"

#include "GLFW/glfw3.h"
#include "Backend/Core/Events/Events.h"

#include "spdlog/fmt/fmt.h"
#include <Backend/Windows/WindowsWindow.h>
#ifdef HZR_PLATFORM_WINDOWS

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
bool Input::IsMouseButtonDown(const Mouse::MouseCode& code) {
	auto state = glfwGetMouseButton(s_Window, static_cast<int32_t>(code));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsMouseButtonPressed(const Mouse::MouseCode& code) {
	auto state = glfwGetMouseButton(s_Window, static_cast<int32_t>(code));
	return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePos() {
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

		GLFWgamepadstate state = {};
		glfwGetGamepadState(g, &state);

		for (uint32_t i = Gamepad::AxisFirst; i <= Gamepad::AxisLast; i++)
		{
			float axisX = state.axes[i * 2];
			float axisY = state.axes[i * 2 + 1];

			//Temporary deadzone
			constexpr float deadzone = 0.075f;
			axisX = glm::abs(axisX) > deadzone ? axisX : 0;
			axisY = glm::abs(axisY) > deadzone ? axisY : 0;

			if (gamepad.Axis[i].X != axisX || gamepad.Axis[i].Y != axisY)
			{
				auto& info = HazardRenderer::WindowsWindow::s_CurrentWindow->GetWindowInfo();
				GamepadAxisMovedEvent event(i, axisX, axisY);
				info.EventCallback(event);
			}

			gamepad.Axis[i].X = axisX;
			gamepad.Axis[i].Y = axisY;
		}
		for (uint32_t i = Gamepad::TriggerLeft; i <= Gamepad::TriggerRight; i++)
		{
			float axisX = state.axes[i + 2];
			if (gamepad.Axis[i].X != axisX)
			{
				auto& info = HazardRenderer::WindowsWindow::s_CurrentWindow->GetWindowInfo();
				GamepadAxisMovedEvent event(i, axisX, 0.0f);
				info.EventCallback(event);
			}

			gamepad.Axis[i].X = axisX;
		}
		for (uint32_t i = Gamepad::ButtonFirst; i <= Gamepad::ButtonLast; i++)
		{
			int buttonState = state.buttons[i];
			int buttonLastState = gamepad.Buttons[i];

			if (buttonState == buttonLastState) continue;

			auto& info = HazardRenderer::WindowsWindow::s_CurrentWindow->GetWindowInfo();
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
	gamepad.Name = glfwGetJoystickName(device);
	gamepad.Connected = true;

	if (glfwJoystickIsGamepad(device))
	{
		for (uint32_t i = Gamepad::AxisFirst; i <= Gamepad::AxisLast; i++)
			gamepad.Axis[i] = { 0.0f, 0.0f, 1.0f };

		for (uint32_t i = Gamepad::ButtonFirst; i < Gamepad::ButtonLast; i++)
			gamepad.Buttons[i] = 0;
	}
	else
	{
		HZR_ASSERT(false, "What is this gamepad {0}", gamepad.Name);
	}
}
void Input::DisconnectGamepad(int device)
{
	auto& gamepad = s_Gamepads[device];
	gamepad.Connected = false;
	gamepad.Buttons.clear();
	gamepad.Axis.clear();
}

bool Input::IsButtonDown(int device, const Gamepad::GamepadCode& code)
{
	auto& gamepad = s_Gamepads[device];
	return gamepad.Buttons[code];
}

Axis2D Input::GetAxis(int device, const Gamepad::GamepadCode& code)
{
	auto& gamepad = s_Gamepads[device];
	return gamepad.Axis[code];
}
bool Input::IsAxis(int device, const Gamepad::GamepadCode& code)
{
	auto& axis = s_Gamepads[device].Axis[code];
	
	constexpr float deadzone = 0.075f;
	float x = glm::abs(axis.X) > deadzone ? axis.X : 0;
	float y = glm::abs(axis.Y) > deadzone ? axis.Y : 0;

	return x != 0.0 || y != 0.0f;
}
#endif
