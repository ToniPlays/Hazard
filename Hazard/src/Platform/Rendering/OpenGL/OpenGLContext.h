#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"

#include "GLFW/glfw3.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLContext : public GraphicsContext {

	public:
		OpenGLContext(WindowProps* props);
		~OpenGLContext();

		void Init(Window* window, ApplicationCreateInfo* appInfo) override;
		void ClearFrame(glm::vec4 clearColor) const override;
		void SetViewport(int x, int y, int w, int h) const override;
		void SetDepthTest(DepthFunc type) const override;

		void DrawIndexed(VertexArray& array, uint32_t size) const override;

		void SetErrorListener(const ErrorCallback& callback) override;
		DeviceSpec GetDeviceSpec() const override;

	public:
		static void SendDebugMessage(const char* message, const char* code);
	private:
		GLFWwindow* m_Window;
		static ErrorCallback s_Callback;
	};
}