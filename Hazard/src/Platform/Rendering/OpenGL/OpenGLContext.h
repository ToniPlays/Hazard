#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"

#include "GLFW/glfw3.h"

namespace Hazard {
	class HAZARD_API OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(Window* window, WindowProps* props);
		~OpenGLContext();

		void Init() const override;
		void ClearFrame(Color clearColor) const override;
		void SetViewport(int x, int y, int w, int h) const override;

		void DrawArray(VertexArray* array) const override;

		void SetErrorListener(const ErrorCallback& callback) override;
		std::string GetVersion() const override;
	private:
		static ErrorCallback callback;
		GLFWwindow* window;

		static void OnError(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};
}