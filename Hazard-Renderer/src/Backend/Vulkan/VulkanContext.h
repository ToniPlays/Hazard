#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/GraphicsContext.h"
#include "CommandQueue.h"
#include "Core/VulkanPhysicalDevice.h"
#include "Core/VulkanDevice.h"
#include "Core/VulkanSwapChain.h"
#include "VkUtils.h"

#include "GLFW/glfw3.h"
#include <vulkan/vulkan.h>


#if HZR_DEBUG
#define VK_CHECK_RESULT(result, x) if(result != VK_SUCCESS) {	\
	std::cout << VkUtils::ResultToString(result) << std::endl;	\
	HZR_ASSERT(false, x);										\
}									
#else
#define VK_CHECK_RESULT(result, x) result
#endif

// Macro to get a procedure address based on a vulkan instance
#define GET_INSTANCE_PROC_ADDR(inst, entrypoint)															\
{																											\
	fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetInstanceProcAddr(inst, "vk"#entrypoint));	\
	HZR_ASSERT(fp##entrypoint, "Instance Proc adress not found vk"#entrypoint);								\
}

// Macro to get a procedure address based on a vulkan device
#define GET_DEVICE_PROC_ADDR(dev, entrypoint)																\
{																											\
	fp##entrypoint = reinterpret_cast<PFN_vk##entrypoint>(vkGetDeviceProcAddr(dev, "vk"#entrypoint));		\
	HZR_ASSERT(fp##entrypoint, "Device Proc adress not found vk"#entrypoint);								\
}


namespace HazardRenderer::Vulkan {

	struct VulkanData
	{
		std::vector<VkDescriptorPool> DescriptorPools;
		std::vector<uint32_t> DescriptorPoolAllocationCount;
	};

	class VulkanContext : public GraphicsContext {

	public:
		VulkanContext(WindowProps* props);
		~VulkanContext();

		void Init(Window* window, HazardRendererCreateInfo* info) override;

		void SetClearColor(const glm::vec4& color) override;
		void BeginFrame() override;
		void Present() override;

		Ref<PhysicalDevice> GetDevice() override { return m_VulkanPhysicalDevice; };
		Ref<Swapchain> GetSwapchain() override { return m_Swapchain; };

		//Vulkan specific
		VkDescriptorSet RT_AllocateDescriptorSet(VkDescriptorSetAllocateInfo allocInfo);
		DefaultResources& GetDefaultResources() override { return m_DefaultResources; };

		static VulkanContext* GetInstance() { return s_Instance; }
		static VkInstance GetVulkanInstance() { return s_Instance->m_VulkanInstance; }
		static uint32_t GetImagesInFlight() { return  s_Instance->m_Swapchain->GetImageCount(); }
		static Ref<VulkanDevice> GetLogicalDevice() { return  s_Instance->m_VulkanDevice; }
		static VkPipelineCache GetPipelineCache() { return s_Instance->m_PipelineCache; }
		static uint32_t GetFrameIndex() { return s_Instance->m_Swapchain->GetCurrentBufferIndex(); }

		VkClearColorValue GetClearColorValue() const { return { m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a }; }

	private:
		void CreateDescriptorPools();
		std::vector<const char*> GetSupportedExtensions();

	private:
		inline static VulkanContext* s_Instance;
		inline static VulkanData* s_Data;

		Window* m_Window;
		glm::vec4 m_ClearColor = { 0, 0, 0, 1 };
		VkInstance m_VulkanInstance = nullptr;
		VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;

		VkSurfaceKHR m_WindowSurface = nullptr;
		VkPipelineCache m_PipelineCache = nullptr;
		//Device
		Ref<VulkanPhysicalDevice> m_VulkanPhysicalDevice;
		Ref<VulkanDevice> m_VulkanDevice;
		Ref<VulkanSwapchain> m_Swapchain;

		DefaultResources m_DefaultResources;
	};
}
#endif
