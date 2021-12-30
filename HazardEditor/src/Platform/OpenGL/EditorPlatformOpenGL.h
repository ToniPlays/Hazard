#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"

class EditorPlatformOpenGL : public EditorGuiPlatform {
public:

	EditorPlatformOpenGL(GLFWwindow* window);
	~EditorPlatformOpenGL();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;
};