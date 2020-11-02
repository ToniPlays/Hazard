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
		~OpenGLContext();

		void Init() const override;
		void ClearFrame(Color clearColor) const override;
		void SetViewport(int x, int y, int w, int h) const override;
		void Draw(VertexArray* vertexArray, uint32_t indices, RenderType type) override;
		void DrawIndexed(VertexArray* vertexArray, uint32_t indices) override;
		std::string GetVersion() const override;
	private:
		GLFWwindow* window;
	};
}