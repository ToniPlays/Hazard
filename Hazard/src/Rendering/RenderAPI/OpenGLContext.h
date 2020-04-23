#pragma once
#include "GraphicsContext.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

namespace Hazard {
	class HAZARD_API OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(GLFWwindow* window) : windowHandle(window) {}
		virtual void Init() override;
		void ClearColor(Color color) override;
		virtual void SwapBuffers() override;

		virtual std::string Get(int value) const override;
		void SetViewport(int x, int y, int w, int h) override;

	private:
		GLFWwindow* windowHandle;
	};
}