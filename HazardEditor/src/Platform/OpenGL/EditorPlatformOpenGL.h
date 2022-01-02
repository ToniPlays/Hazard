#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"

#include <glfw/glfw3.h>

class EditorPlatformOpenGL : public EditorGuiPlatform {
public:

	EditorPlatformOpenGL(GLFWwindow* window);
	~EditorPlatformOpenGL();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;
};