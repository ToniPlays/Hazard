
#include "Input.h"

#include "GLFW/glfw3.h"
#include "Core/Events/Events.h"

#include "spdlog/fmt/fmt.h"
#include <Windows/WindowsWindow.h>

void Input::Init(HazardRenderer::Window& window)
{

	s_Window = &window;

	s_Gamepads.resize(16);

	for (uint32_t i = 0; i < s_Gamepads.size(); i++)
	{
		//if (glfwJoystickPresent(i))
		//	ConnectGamepad(i);
	}
}
bool Input::AnyKey()
{
	return s_KeysPressed > 0;
}
bool Input::IsKeyDown(const Key::KeyCode& key)
{
	if (key >= 385) return false;
	auto state = s_KeyStates[key];
	return state != (uint8_t)InputState::Released;
}
bool Input::IsKeyPressed(const Key::KeyCode& key)
{
	if (key >= 385) return false;
	auto state = s_KeyStates[key];
	return state == (uint8_t)InputState::Press;
}
bool Input::IsKeyReleased(const Key::KeyCode& key)
{
	if (key >= 385) return false;
	auto state = s_KeyStates[key];
	return state == (uint8_t)InputState::Released;
}
bool Input::IsMouseButtonDown(const Mouse::MouseCode& code)
{
	if (code >= 128) return false;
	auto state = s_MouseButtons[code];
	return state == (uint8_t)InputState::Press || state == (uint8_t)InputState::Down;
}
bool Input::IsMouseButtonPressed(const Mouse::MouseCode& code)
{
	if (code >= 128) return false;
	auto state = s_MouseButtons[code];
	return state == (uint8_t)InputState::Press;
}

glm::vec2 Input::GetMousePos()
{
	return s_MousePos;
}

void Input::Update()
{
	HZR_PROFILE_FUNCTION();

	for (uint32_t i = 0; i < 384; i++)
	{
		if (s_KeyStates[i] == GLFW_PRESS)
			UpdateKey(i, GLFW_REPEAT);
	}

	for (uint32_t g = 0; g < s_Gamepads.size(); g++)
	{
		auto& gamepad = s_Gamepads[g];
		if (!gamepad.Connected) continue;

        /*
		auto& info = s_Window->GetWindowInfo();
        
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
         */
	}
}

void Input::UpdateKey(uint32_t code, int state)
{
	if (code < 384)
		s_KeyStates[code] = state;
}
void Input::ConnectGamepad(int device)
{
    /*
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
    */
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

void Input::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(OnKeyPressed);
	dispatcher.Dispatch<KeyReleasedEvent>(OnKeyReleased);
	dispatcher.Dispatch<MouseButtonPressedEvent>(OnMouseButtonPressed);
	dispatcher.Dispatch<MouseButtonReleasedEvent>(OnMouseButtonReleased);
	dispatcher.Dispatch<MouseMovedEvent>(OnMouseMoved);
	dispatcher.Dispatch<MouseScrolledEvent>(OnMouseScrolled);
}

bool Input::OnKeyPressed(KeyPressedEvent& e)
{
	s_KeysPressed++;
	s_KeyStates[e.GetKeyCode()] = (uint8_t)InputState::Press;
	return false;
}

bool Input::OnKeyReleased(KeyReleasedEvent& e)
{
	s_KeysPressed--;
	s_KeyStates[e.GetKeyCode()] = (uint8_t)InputState::Released;
	return false;
}

bool Input::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	s_KeysPressed++;
	s_MouseButtons[e.GetMouseButton()] = (uint8_t)InputState::Press;
	return false;
}

bool Input::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	s_KeysPressed--;
	s_MouseButtons[e.GetMouseButton()] = (uint8_t)InputState::Released;
	return false;
}

bool Input::OnMouseMoved(MouseMovedEvent& e)
{
	s_MousePos = { e.GetX(), e.GetY() };
	return false;
}

bool Input::OnMouseScrolled(MouseScrolledEvent& e)
{
	return false;
}
