#pragma once
#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_OPENGL
#include "Backend/Core/GraphicsContext.h"
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
		void SetViewport(int x, int y, int w, int h) override;

		void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass);
		void EndRenderPass(Ref<RenderCommandBuffer> buffer);
		/*
		void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth);

		void SetErrorListener(const ErrorCallback& callback) override;
		*/
		PhysicalDevice& GetDevice() { return *m_PhysicalDevice;  };

	public:
		static Window* GetWindow() { return s_Instance->m_Window; }
		static glm::vec4 GetClearColor() { return s_Instance->m_ClearColor; }

	private:
		inline static OpenGLContext* s_Instance;
		Window* m_Window;
		OpenGLPhysicalDevice* m_PhysicalDevice;
		glm::vec4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}
#endif
