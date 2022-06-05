#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"
#include "HazardRendererCore.h"

#include <glfw/glfw3.h>

class EditorPlatformOpenGL : public EditorGuiPlatform {
public:

	EditorPlatformOpenGL(HazardRenderer::Window& window);
	~EditorPlatformOpenGL();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;
};