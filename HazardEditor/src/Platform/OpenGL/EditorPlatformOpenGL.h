#pragma once

#include "../EditorGuiPlatform.h"

class EditorPlatformOpenGL : public EditorGuiPlatform {
public:

	EditorPlatformOpenGL(GLFWwindow* window);
	~EditorPlatformOpenGL();

	void BeginFrame();
	void EndFrame();
};