#pragma once
#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_OPENGL
#include "CommandQueue.h"
#include "Backend/Core/GraphicsContext.h"
#include "OpenGLPhysicalDevice.h"
#include <GLFW/glfw3.h>

namespace HazardRenderer::OpenGL {

	class OpenGLSwapchain;

	class OpenGLContext : public GraphicsContext {

	public:
		OpenGLContext(WindowProps* props);
		~OpenGLContext();

		void Init(Window* window, HazardRendererCreateInfo* info) override;
		void BeginFrame() override;
		void Present() override;

		void SetClearColor(const glm::vec4& color) override;

		DefaultResources& GetDefaultResources() override { return m_DefaultResources; };

		Ref<PhysicalDevice> GetDevice() { return m_PhysicalDevice;  };
		Ref<Swapchain> GetSwapchain() { return m_Swapchain.As<Swapchain>(); }
		Window* GetWindow() { return m_Window; }
		glm::vec4 GetClearColor();

		static OpenGLContext& GetInstance() { return *s_Instance; }

	private:
		inline static OpenGLContext* s_Instance;
		Window* m_Window;
		Ref<OpenGLPhysicalDevice> m_PhysicalDevice;
		Ref<OpenGLSwapchain> m_Swapchain;
		DefaultResources m_DefaultResources;
	};
}
#endif
