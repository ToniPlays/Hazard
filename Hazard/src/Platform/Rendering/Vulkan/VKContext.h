#pragma once

#include "Hazard/Core/Core.h"
#include "Platform/Rendering/GraphicsContext.h"
#include "Platform/System/Window.h"

#include <optional>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan {

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;

		bool isComplete() {
			return graphicsFamily.has_value();
		}
	};

	struct VulkanData {
		VkInstance instance;
		VkDevice device;
		VkPhysicalDevice physicalDevice;
		uint32_t queueFamily;
		VkQueue queue;
		VkSwapchainKHR swapchain;

		VkPipelineCache pipelineCache;
		VkDescriptorPool descriptorPool;
		uint32_t minImageCount = 3;
		uint32_t imageCount = 3;
		VkSampleCountFlagBits MSAASamples = VK_SAMPLE_COUNT_1_BIT;

		VkRenderPass renderPass;
	};


	class VKContext : public GraphicsContext {

	public:
		VKContext(Window* window, WindowProps* props);
		~VKContext();

		void Init() const override;
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
		GLFWwindow* m_Window;
		VulkanData m_VulkanData;
	};
}