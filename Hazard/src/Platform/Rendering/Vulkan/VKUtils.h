#pragma once

#include <optional>
#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "vulkan/vulkan.h"
#include "Hazard/Rendering/Pipeline/Shader.h"
#include "Hazard/Rendering/Pipeline/Pipeline.h"

namespace Hazard::Rendering::Vulkan 
{
	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily = 0;
		std::optional<uint32_t> presentFamily = 0;

		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VKUtils {
	public:
		static std::vector<const char*> GetRequiredExtensions(bool validation = false);
		static VkPhysicalDevice GetVulkanCapableDevice(VkInstance instance, VkSurfaceKHR surface);
		static QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface);
		static VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
		static SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
		static VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats, VkFormat format = VK_FORMAT_A8B8G8R8_USCALED_PACK32, VkColorSpaceKHR space = VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT);
		static VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR preferred = VK_PRESENT_MODE_IMMEDIATE_KHR, VkPresentModeKHR defaultMode = VK_PRESENT_MODE_FIFO_KHR);
		static VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, int w, int h);
		static VkFormat GetFormat(ImageFormat format);
		static VkFormat ShaderDataTypeToVkFormat(ShaderDataType type);
		static ShaderType ShaderTypeFromVulkanStage(VkShaderStageFlagBits type);

		static VkShaderStageFlags ShaderUsageToVulkanUsage(uint32_t usage);
		static bool IsDepth(ImageFormat format);
		static VkFormat GetImageFormat(ImageFormat format);
		static VkFilter GetSamplerFilter(const FilterMode& filter);
		static VkSamplerAddressMode GetSamplerWrap(const ImageWrap& wrap);
		static VkPolygonMode DrawTypeToVKType(const DrawType& type);
		static VkPrimitiveTopology DrawTypeToVKTopology(const DrawType& type);

		static void SetImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange, VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
			VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT);

		static void InsertImageMemoryBarrier(VkCommandBuffer cmdbuffer,
			VkImage image,
			VkAccessFlags srcAccessMask,
			VkAccessFlags dstAccessMask,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask,
			VkImageSubresourceRange subresourceRange);
		static std::string ResultToString(VkResult result);

	private:
		static bool SuitableDevice(VkPhysicalDevice device, VkInstance instance, VkSurfaceKHR surface);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	};
}