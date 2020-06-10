#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"


namespace Hazard {
	class HAZARD_API OpenGLContext : public GraphicsContext {
	public:
		OpenGLContext(Window* window, WindowProps* props);

		void Init() const override;
		void ClearFrame() const override;
		void SetClearColor(Color color) override;
		void SetViewport(int x, int y, int w, int h) const override;
		void Draw(RenderType type, Mesh* mesh) override;
		Color GetClearColor() { return clearColor; }
		std::string GetVersion() const;
	private:
		Color clearColor;
		GLFWwindow* window;
	};
}