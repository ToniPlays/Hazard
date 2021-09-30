#pragma once

#include <optional>
#include "Hazard/Rendering/Texture/FrameBuffer.h"

#include "vulkan/vulkan.h"
#include "Hazard/Rendering/Shader.h"

namespace Hazard::Rendering::Vulkan {

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
		static SwapChainSupportDetails GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface);
		static VkSurfaceFormatKHR ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats, VkFormat format = VK_FORMAT_A8B8G8R8_USCALED_PACK32, VkColorSpaceKHR space = VK_COLOR_SPACE_ADOBERGB_NONLINEAR_EXT);
		static VkPresentModeKHR ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR preferred = VK_PRESENT_MODE_IMMEDIATE_KHR, VkPresentModeKHR defaultMode = VK_PRESENT_MODE_FIFO_KHR);
		static VkExtent2D ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, int w, int h);
		static VkFormat FindSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		static VkAttachmentDescription CreateAttachmentDescription(FrameBufferAttachment& attachment);
		static std::vector<VkAttachmentReference> CreateColorRefs(std::vector<FrameBufferAttachment>& attachments);
		static std::vector<VkAttachmentReference> CreateDepthRefs(std::vector<FrameBufferAttachment>& attachments, uint32_t startIndex = 1);
		static VkFormat GetFormat(FrameBufferTextureFormat format);
		static VkFormat ShaderDataTypeToVkFormat(ShaderDataType type);
		static VkShaderStageFlagBits ShaderTypeToVulkanStage(ShaderType type);
		static ShaderType ShaderTypeFromVulkanStage(VkShaderStageFlagBits type);

		static VkShaderStageFlags ShaderUsageToVulkanUsage(uint32_t usage);

	private:
		static bool SuitableDevice(VkPhysicalDevice device, VkInstance instance, VkSurfaceKHR surface);
		static bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	};
}