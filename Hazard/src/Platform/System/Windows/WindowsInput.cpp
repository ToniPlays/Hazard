#pragma once
#include <hzrpch.h>
#include "Hazard/Events/Input.h"

#include "Hazard/RenderContext/RenderContext.h"
#include "GLFW/glfw3.h"

namespace Hazard {

	GLFWwindow* window = nullptr;

	void Input::Init() {
		window = (GLFWwindow*)Application::GetModule<Rendering::RenderContext>().GetWindow().GetNativeWindow();
	}
	bool Input::IsKeyDown(const Key::KeyCode key) { 
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsKeyPressed(const Key::KeyCode key) {
		auto state = glfwGetKey(window, static_cast<int32_t>(key));
		return state == GLFW_PRESS;
	}
	bool Input::IsMouseButtonDown(const Mouse::MouseCode code) {
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(code));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(const Mouse::MouseCode code) {
		auto state = glfwGetMouseButton(window, static_cast<int32_t>(code));
		return state == GLFW_PRESS;
	}

	glm::vec2 Input::GetMousePos() {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}
}