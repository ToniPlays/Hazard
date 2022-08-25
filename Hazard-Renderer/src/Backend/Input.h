#pragma once

#include "Backend/Core/Events/KeyCode.h"
#include "Backend/Core/Events/MouseCode.h"
#include "Backend/Core/Window.h"
#include <glm/glm.hpp>

class Input {

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
private:
	static int keyStates[384];
};