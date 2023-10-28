#pragma once

#include "Core/Events/KeyCode.h"
#include "Core/Events/MouseCode.h"
#include "Core/Events/GamepadCodes.h"
#include "Core/Window.h"
#include "Core/Events/Events.h"
#include <glm/glm.hpp>

enum class InputState
{
	Released = 0,
	Press = 1,
	Down = 2,
	Up = 3
};

struct Joystick
{
	std::string Name;
	uint32_t Binding;
	std::vector<uint32_t> Buttons;
	std::vector<float> Axis;
	bool Connected = false;
};

class Input
{
public:
	static void Init(HazardRenderer::Window& window);
	static void Update();
	static void OnEvent(Event& e);
	static bool AnyKey();
	static bool IsKeyDown(const Key::KeyCode& key);
	static bool IsKeyPressed(const Key::KeyCode& key);
	static bool IsKeyReleased(const Key::KeyCode& key);

	static bool IsMouseButtonDown(const Mouse::MouseCode& code);
	static bool IsMouseButtonPressed(const Mouse::MouseCode& code);

	static glm::vec2 GetMousePos();
	static void UpdateKey(uint32_t code, int state);

	static void ConnectGamepad(int device);
	static void DisconnectGamepad(int device);
	static bool IsButtonDown(int device, const Gamepad::GamepadCode& code);
	static float GetAxis(int device, const Gamepad::GamepadCode& code);
	static bool IsAxis(int device, const Gamepad::GamepadCode& code);

	static const std::vector<Joystick>& GetGamepads() { return s_Gamepads; }

private:
	static bool OnKeyPressed(KeyPressedEvent& e);
	static bool OnKeyReleased(KeyReleasedEvent& e);

	static bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	static bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);

	static bool OnMouseMoved(MouseMovedEvent& e);
	static bool OnMouseScrolled(MouseScrolledEvent& e);

private:
    inline static glm::vec2 s_MousePos;
	inline static glm::vec2 s_PreviosMousePos;
	inline static uint8_t s_MouseButtons[128];
	inline static uint64_t s_KeysPressed = 0;
	inline static uint8_t s_KeyStates[384];
	inline static std::vector<Joystick> s_Gamepads;
	inline static HazardRenderer::Window* s_Window;
};