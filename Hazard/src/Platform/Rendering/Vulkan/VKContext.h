#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vulkan/vulkan.h>


namespace Hazard::Rendering::Vulkan {

	struct VulkanData 
	{

		GLFWwindow* m_Window;

		VkInstance instance;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		uint32_t queueFamily;
		VkQueue queue;
		VkSurfaceKHR vkSurface;
		VkSwapchainKHR swapchain;

		VkFormat swapchainImageFormat;
		VkExtent2D swapchainExtent;

		VkPipelineCache pipelineCache;
		VkDescriptorPool descriptorPool;
		uint32_t minImageCount = 3;
		uint32_t imageCount = 3;
		VkSampleCountFlagBits MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		VkRenderPass renderPass;

		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
	};


	class VKContext : public GraphicsContext {

	public:
		VKContext(WindowProps* props);
		~VKContext();

		void Init(Window* window) override;
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
		static ErrorCallback s_Callback;

		VulkanData& GetVulkanData() { return m_VulkanData; }

	private:
		bool CreateInstance() const;
		bool CreateImageViews();

	private:
		VulkanData m_VulkanData;
	};
}