#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"
#include "HazardRendererCore.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>

class EditorPlatformOpenGL : public EditorGuiPlatform {
public:

	EditorPlatformOpenGL(HazardRenderer::Window& window);
	~EditorPlatformOpenGL();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;

private:
	HazardRenderer::Window* m_Window;
	HazardRenderer::OpenGL::OpenGLContext* m_Context;
};
#endif
