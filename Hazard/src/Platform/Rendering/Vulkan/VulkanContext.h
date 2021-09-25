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

	struct DepthStencil {
		VkImage image;
		VkImageView view;
		VkDeviceMemory mem;
	};
	struct VulkanSemaphore {
		VkSemaphore PresentComplete;
		VkSemaphore RenderComplete;
	};


	class VulkanContext : public GraphicsContext {

	public:
		VulkanContext(WindowProps* props);
		~VulkanContext();

		void Init(Window* window, ApplicationCreateInfo* appInfo) override;
		void SwapBuffers() override;
		void SetClearColor(glm::vec4 clearColor) override 
		{
			this->clearColor = clearColor;
		};
		void SetViewport(int x, int y, int w, int h) override;
		void DrawIndexed(VertexArray* array, uint32_t size) override;

		void SetErrorListener(const ErrorCallback& callback) override;
		DeviceSpec GetDeviceSpec() const override;

		static VkInstance GetInstance() { return m_Instance; }
		static VkSurfaceKHR GetSurface() { return m_WindowSurface->GetVkSurface(); }
		static VulkanDevice* GetDevice() { return m_Device.get(); }

	public:
		static void SendDebugMessage(const char* message, const char* code);
	private:

		void CreateDepthStencil();
		void CreateFrameBuffer();
		void AllocateCommandBuffers();
		void CreateDrawBuffers();

		void Render();

		Window* m_Window;
		inline static VkInstance m_Instance;
		inline static Scope<WindowSurface> m_WindowSurface;
		inline static Scope<VulkanDevice> m_Device;

		VkRenderPass m_RenderPass;
		VkPipelineCache m_PipelineCache;

		std::vector<VkCommandBuffer> m_DrawCommandBuffers;
		std::vector<VkFence> m_WaitFences;
		std::vector<VkFramebuffer> m_FrameBuffers;

		VulkanSemaphore m_Semaphores;
		VulkanAllocator m_Allocator;

		VkFormat m_DepthFormat;
		VkSubmitInfo m_SubmitInfo;

		VulkanSwapChain m_SwapChain;

		glm::vec4 clearColor = { 0, 0, 0, 1 };
		DepthStencil m_DepthStencil;

		uint32_t m_CurrentBufferIndex = 0;
		static ErrorCallback s_Callback;
	};
}