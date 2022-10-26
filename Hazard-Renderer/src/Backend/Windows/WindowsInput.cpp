
#include "Backend/Input.h"

#include "GLFW/glfw3.h"

GLFWwindow* s_Window = nullptr;

int Input::keyStates[384];
bool anyKey = false;

void Input::Init(HazardRenderer::Window& window)
{
	s_Window = (GLFWwindow*)window.GetNativeWindow();
}
bool Input::AnyKey()
{
	return anyKey;
}
bool Input::IsKeyDown(const Key::KeyCode key)
{
	auto state = keyStates[key];
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsKeyPressed(const Key::KeyCode key) 
{
	auto state = keyStates[key];
	return state == GLFW_PRESS;
}
bool Input::IsKeyReleased(const Key::KeyCode key) 
{
	auto state = keyStates[key];
	return state == GLFW_RELEASE;
}
bool Input::IsMouseButtonDown(const Mouse::MouseCode code) {
	auto state = glfwGetMouseButton(s_Window, static_cast<int32_t>(code));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}
bool Input::IsMouseButtonPressed(const Mouse::MouseCode code) {
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
	for (uint32_t i = 0; i < 384; i++) {
		if (keyStates[i] == GLFW_PRESS) 
		{
			anyKey = true;
			UpdateKey(i, GLFW_REPEAT);
		}
	}
}

void Input::UpdateKey(uint32_t code, int state)
{
	if (code < 384)
		keyStates[code] = state;
}
