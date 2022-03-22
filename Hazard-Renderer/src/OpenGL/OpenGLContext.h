#pragma once

#include "Core/GraphicsContext.h"
#include "OpenGLPhysicalDevice.h"
#include <GLFW/glfw3.h>

namespace HazardRenderer::OpenGL {

	class OpenGLContext : public GraphicsContext {

	public:
		OpenGLContext(WindowProps* props);
		~OpenGLContext();

		void Init(Window* window, HazardRendererCreateInfo* info) override;
		void BeginFrame() override;
		void Present() override;

		void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
		/*void SetViewport(int x, int y, int w, int h) override;
		void Close() override {};

		void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass);
		void EndRenderPass(Ref<RenderCommandBuffer> buffer);
		void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth);

		void SetErrorListener(const ErrorCallback& callback) override;
		PhysicalDevice& GetPhysicalDevice() const override;

		void AddResizeCallback(const ResizeCallback& callback) { m_ResizeCallback.push_back(callback); };
		*/

		PhysicalDevice& GetDevice() { return *m_PhysicalDevice;  };

	public:
		//static void SendDebugMessage(const char* message, const char* code);

	private:
		GLFWwindow* m_Window;
		OpenGLPhysicalDevice* m_PhysicalDevice;
		//PhysicalDevice* m_PhysicalDevice;
		glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		//static ErrorCallback s_Callback;
		//std::vector<ResizeCallback> m_ResizeCallback;
	};
}