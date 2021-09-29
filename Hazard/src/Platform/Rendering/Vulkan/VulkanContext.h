#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"
#include "Core/VulkanDevice.h"
#include "Core/WindowSurface.h""
#include "Core/VulkanAllocator.h"
#include "Core/VulkanSwapChain.h"

#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {

	class VulkanContext : public GraphicsContext {

	public:
		VulkanContext(WindowProps* props);
		~VulkanContext();

		void Init(Window* window, ApplicationCreateInfo* appInfo) override;
		void SwapBuffers() override;
		void SetClearColor(const glm::vec4& color) override { clearColor = color; }
		void SetViewport(int x, int y, int w, int h) override;

		void SetErrorListener(const ErrorCallback& callback) override;
		DeviceSpec GetDeviceSpec() const override;

		static VkInstance GetInstance() { return m_Instance; }
		static VkSurfaceKHR GetSurface() { return m_WindowSurface->GetVkSurface(); }
		static VulkanDevice* GetDevice() { return m_Device.get(); }
		static VkPipelineCache GetPipelineCache() { return m_Device->GetPipelineCache(); }
		inline static Ref<VulkanSwapChain> GetSwapchain() { return m_SwapChain; }

		void BeginFrame();
		void Begin() override;
		void End();

	public:
		static void SendDebugMessage(const char* message, const char* code);
	private:

		Window* m_Window;
		inline static VkInstance m_Instance;
		inline static Scope<VulkanDevice> m_Device;
		inline static Scope<WindowSurface> m_WindowSurface;
		inline static VkPipelineCache m_PipelineCache;
		static inline Ref<VulkanSwapChain> m_SwapChain;

		glm::vec4 clearColor = { 0, 0, 0, 1 };
		
		uint32_t m_CurrentBufferIndex = 0;
		static ErrorCallback s_Callback;
	};
}