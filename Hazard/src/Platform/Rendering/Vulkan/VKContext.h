#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

namespace Hazard::Rendering::Vulkan {

	class VKContext : public GraphicsContext {

	public:
		VKContext(Window* window, WindowProps* props);
		~VKContext();

		void Init() const override;
		void ClearFrame(glm::vec4 clearColor) const override;
		void SetViewport(int x, int y, int w, int h) const override;
		void SetDepthTest(DepthTest type) const override;
		void Enable(Function fn) const;
		void Disable(Function fn) const;

		void DrawIndexed(VertexArray& array, uint32_t size) const override;

		void SetErrorListener(const ErrorCallback& callback) override;
		std::string GetVersion() const override;
		std::string GetDevice() const override;

	public:
		static void SendDebugMessage(const char* message, const char* code);
		static GLFWvkproc GetProc(const char* adress);

	private:
		static ErrorCallback s_Callback;

	private:
		GLFWwindow* m_Window;
	};
}