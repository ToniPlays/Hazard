#pragma once
#include "Core/Core.h"
#include "Platform/GraphicsContext.h"
#include <GLFW/glfw3.h>

namespace Hazard {
	class HAZARD_API OpenGLContext : public GraphicsContext {

	public: 
		OpenGLContext(Window* window, WindowProps* props);
		void Init() const;
		void ClearFrame() const;
		void SetClearColor(Color color) const override;
		void SetViewport(int x, int y, int w, int h) const override;

		Color GetClearColor() const override;
		double GetMSTime() const { return glfwGetTime() * 1000.0f; }

		std::string GetVersion() const override;
	private:
		GLFWwindow* window;
	};
}
