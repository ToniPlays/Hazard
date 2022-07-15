
#include "Backend/Input.h"

#include "GLFW/glfw3.h"

namespace HazardRenderer {

	GLFWwindow* window = nullptr;

	int Input::keyStates[384];

	void Input::Init() 
	{
		//if (!Application::HasModule<Rendering::RenderContext>()) return;
		//Rendering::RenderContext* context = &Application::GetModule<Rendering::RenderContext>();
		//window = (GLFWwindow*)context->GetWindow().GetNativeWindow();
	}
	bool Input::AnyKey()
	{
		//TODO: Make work
		return false;
	}
	bool Input::IsKeyDown(const Key::KeyCode key) { 
		auto state = keyStates[key];
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsKeyPressed(const Key::KeyCode key) {
		auto state = keyStates[key];
		return state == GLFW_PRESS;
	}
	bool Input::IsKeyReleased(const Key::KeyCode key) {
		auto state = keyStates[key];
		return state == GLFW_RELEASE;
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

	void Input::Update() 
	{
		for (uint32_t i = 0; i < 384; i++) {
			if (keyStates[i] == GLFW_PRESS)  
 				UpdateKey(i, GLFW_REPEAT);
		}
	}

	void Input::UpdateKey(uint32_t code, int state) 
	{
		if (code < 384) 
			keyStates[code] = state;
	}
}
