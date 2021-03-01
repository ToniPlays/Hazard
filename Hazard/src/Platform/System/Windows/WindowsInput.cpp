#pragma once
#include <hzrpch.h>
#include "Hazard/Events/Input.h"

#include "Hazard/Rendering/RenderContext.h"
#include "GLFW/glfw3.h"

namespace Hazard {

	GLFWwindow* window = nullptr;

	void Input::Init() {
		window = (GLFWwindow*)Core::HazardLoop::GetModule<Rendering::RenderContext>().GetWindow().GetNativeWindow();
	}
	bool Input::IsKeyDown(const Key::KeyCode key) { 
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsKeyPressed(const Key::KeyCode key) {
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}
}