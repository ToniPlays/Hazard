#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/GraphicsContext.h"
#include "Core/WindowSurface.h"
#include "Core/VulkanDevice.h"
#include "Core/VulkanSwapChain.h"

#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>

#define VK_CHECK_RESULT(result) if(result != VK_SUCCESS) { ASSERT(false, "We failed"); }

namespace HazardRenderer::Vulkan {

	class VulkanContext : public GraphicsContext {

	public:
		VulkanContext(WindowProps* props);
		~VulkanContext();

		void Init(Window* window, HazardRendererCreateInfo* info) override;
		void SetClearColor(const glm::vec4& color) override { m_ClearColor = color; }
		void BeginFrame() override;
		void Present() override;

		void SetViewport(int x, int y, int w, int h) override;
		/*
		void AddResizeCallback(const ResizeCallback& callback) { m_SwapChain->AddResizeCallback(callback); };
		void SetErrorListener(const ErrorCallback& callback) override;
		PhysicalDevice& GetPhysicalDevice() const override;

		static VkInstance GetInstance() { return m_Instance; }
		static VkSurfaceKHR GetSurface() { return m_WindowSurface->GetVkSurface(); }
		static VulkanDevice* GetDevice() { return m_Device.get(); }
		static VkPipelineCache GetPipelineCache() { return m_Device->GetPipelineCache(); }
		inline static Ref<VulkanSwapChain> GetSwapchain() { return m_SwapChain; }

		static glm::vec4 GetClearColor() { return m_ClearColor; }
		static VkDescriptorSet RT_AllocateDescriptorSet(VkDescriptorSetAllocateInfo& allocInfo);
		*/

		void BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass) override;
		void EndRenderPass(Ref<RenderCommandBuffer> buffer) override;
		//void SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth);
	
		
		PhysicalDevice& GetDevice() { return *m_Device; }
		static VulkanDevice& GetPhysicalDevice() { return (VulkanDevice&)s_Instance->GetDevice(); }

		static VkInstance GetVulkanInstance() { return s_Instance->m_Instance; }
		static Ref<VulkanSwapchain> GetSwapchain() { return s_Instance->m_SwapChain; }

		static WindowSurface& GetWindowSurface() { return *s_Instance->m_WindowSurface; }
		static glm::vec4& GetClearColor() { return s_Instance->m_ClearColor; }

	private:

		inline static VulkanContext* s_Instance;

		Window* m_Window;
		glm::vec4 m_ClearColor = { 0, 0, 0, 1 };

		VkInstance m_Instance;
		Scope<WindowSurface> m_WindowSurface;
		Scope<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_SwapChain;

		//inline static std::unordered_map<uint32_t, uint32_t> m_DescriptorAllocations;
		
		//uint32_t m_CurrentBufferIndex = 0;
	};
}
#endif
