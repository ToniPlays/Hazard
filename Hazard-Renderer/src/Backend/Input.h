#pragma once

#include "Backend/Core/Events/KeyCode.h"
#include "Backend/Core/Events/MouseCode.h"
#include "Backend/Core/Events/GamepadCodes.h"
#include "Backend/Core/Window.h"
#include <glm/glm.hpp>

struct Axis2D
{
	float X;
	float Y;
	float Multiplier;
};

struct Joystick
{
	std::string Name;
	std::unordered_map<uint32_t, uint32_t> Buttons;
	std::unordered_map<uint32_t, Axis2D> Axis;
	bool Connected = false;
};

class Input
{
public:
	static void Init(HazardRenderer::Window& window);
	static void Update();
	static bool AnyKey();
	static bool IsKeyDown(const Key::KeyCode key);
	static bool IsKeyPressed(const Key::KeyCode key);
	static bool IsKeyReleased(const Key::KeyCode key);

	static bool IsMouseButtonDown(const Mouse::MouseCode code);
	static bool IsMouseButtonPressed(const Mouse::MouseCode code);

	static glm::vec2 GetMousePos();
	static void UpdateKey(uint32_t code, int state);

	static void ConnectGamepad(int device);
	static void DisconnectGamepad(int device);

	static const std::vector<Joystick>& GetGamepads() { return s_Gamepads; }

private:
	inline static int s_KeyStates[384];
	inline static std::vector<Joystick> s_Gamepads;
};