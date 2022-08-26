#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/GraphicsContext.h"
#include "CommandQueue.h"
#include "Core/VulkanDevice.h"
#include "Core/VulkanSwapChain.h"

#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>
#if HZR_DEBUG
#define VK_CHECK_RESULT(result, x) if(result != VK_SUCCESS) { HZR_ASSERT(false, x); }
#elif
#define VK_CHECK_RESULT(result, x) result
#endif

namespace HazardRenderer::Vulkan {

	class VulkanContext : public GraphicsContext {

	public:
		VulkanContext(WindowProps* props);
		~VulkanContext();

		void Init(Window* window, HazardRendererCreateInfo* info) override;

		void SetClearColor(const glm::vec4& color) override {};
		void BeginFrame() override {};
		void Present() override {};

		void SetViewport(int x, int y, int w, int h) override {};

		PhysicalDevice& GetDevice() override { return *m_VulkanDevice; };
		Ref<Swapchain> GetSwapchain() override { return nullptr; };

		void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override {};
		void EndRenderPass(Ref<RenderCommandBuffer> buffer) override {};

		static VulkanContext* GetInstance() { return s_Instance; }

	private:
		inline static VulkanContext* s_Instance;

		Window* m_Window;
		glm::vec4 m_ClearColor = { 0, 0, 0, 1 };
		VkInstance m_Instance = nullptr;
		VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;

		VkSurfaceKHR m_WindowSurface = nullptr;
		//Device
		VulkanDevice* m_VulkanDevice;
	};
}
#endif
