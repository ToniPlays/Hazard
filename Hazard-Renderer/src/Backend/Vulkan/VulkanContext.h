#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/GraphicsContext.h"
#include "CommandQueue.h"
#include "Core/VulkanPhysicalDevice.h"
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
		void BeginFrame() override;
		void Present() override;

		void SetViewport(int x, int y, int w, int h) override {};

		Ref<PhysicalDevice> GetDevice() override { return m_VulkanPhysicalDevice; };
		Ref<Swapchain> GetSwapchain() override { return m_Swapchain; };

		void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override {};
		void EndRenderPass(Ref<RenderCommandBuffer> buffer) override {};


		//Vulkan specific
		static VulkanContext* GetInstance() { return s_Instance; }
		static VkInstance GetVulkanInstance() { return s_Instance->m_VulkanInstance; }
		static uint32_t GetImagesInFlight() { return  s_Instance->m_ImagesInFlight; }
		static Ref<VulkanDevice> GetLogicalDevice() { return  s_Instance->m_VulkanDevice; }
		static VkPipelineCache GetPipelineCache() { return s_Instance->m_PipelineCache; }

	private:
		inline static VulkanContext* s_Instance;

		Window* m_Window;
		glm::vec4 m_ClearColor = { 0, 0, 0, 1 };
		uint32_t m_ImagesInFlight;
		VkInstance m_VulkanInstance = nullptr;
		VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;

		VkSurfaceKHR m_WindowSurface = nullptr;
		VkPipelineCache m_PipelineCache = nullptr;
		//Device
		Ref<VulkanPhysicalDevice> m_VulkanPhysicalDevice;
		Ref<VulkanDevice> m_VulkanDevice;
		Ref<VulkanSwapchain> m_Swapchain;
	};
}
#endif
