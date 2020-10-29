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
		void ClearFrame(Color clearColor) const override;
		void SetViewport(int x, int y, int w, int h) const override;
		void Draw(VertexArray* vertexArray, uint32_t indices, RenderType type = RenderType::Default) override;
		std::string GetVersion() const;
		std::string GetError() const override;
	private:
		GLFWwindow* window;
	};
}