#include "VkUtils.h"

namespace HazardRenderer::Vulkan::VkUtils {

	void LoadDebugUtilsExtensions(VkInstance instance)
	{
		fpSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)(vkGetInstanceProcAddr(instance, "vkSetDebugUtilsObjectNameEXT"));
		if (fpSetDebugUtilsObjectNameEXT == nullptr)
			fpSetDebugUtilsObjectNameEXT = [](VkDevice device, const VkDebugUtilsObjectNameInfoEXT* pNameInfo) { return VK_SUCCESS; };

		fpCmdBeginDebugUtilsLabelEXT = (PFN_vkCmdBeginDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdBeginDebugUtilsLabelEXT"));
		if (fpCmdBeginDebugUtilsLabelEXT == nullptr)
			fpCmdBeginDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {};

		fpCmdEndDebugUtilsLabelEXT = (PFN_vkCmdEndDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdEndDebugUtilsLabelEXT"));
		if (fpCmdEndDebugUtilsLabelEXT == nullptr)
			fpCmdEndDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer) {};

		fpCmdInsertDebugUtilsLabelEXT = (PFN_vkCmdInsertDebugUtilsLabelEXT)(vkGetInstanceProcAddr(instance, "vkCmdInsertDebugUtilsLabelEXT"));
		if (fpCmdInsertDebugUtilsLabelEXT == nullptr)
			fpCmdInsertDebugUtilsLabelEXT = [](VkCommandBuffer commandBuffer, const VkDebugUtilsLabelEXT* pLabelInfo) {};
	}
	VkFormat VulkanImageFormat(const ImageFormat& format)
	{
		switch (format)
		{
		case ImageFormat::RED32I:			return VK_FORMAT_R32_UINT;
		case ImageFormat::RED32F:			return VK_FORMAT_R32_SFLOAT;
		case ImageFormat::RGB:				return VK_FORMAT_R8G8B8_SNORM;
		case ImageFormat::RGB16F:			return VK_FORMAT_R16G16B16_SFLOAT;
		case ImageFormat::RGB32F:			return VK_FORMAT_R32G32B32_SFLOAT;
		case ImageFormat::RGBA:				return VK_FORMAT_R8G8B8A8_UNORM;
		case ImageFormat::RGBA16F:			return VK_FORMAT_R16G16B16A16_SFLOAT;
		case ImageFormat::RGBA32F:			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ImageFormat::RG16F:			return VK_FORMAT_R16G16_SFLOAT;
		case ImageFormat::RG32F:			return VK_FORMAT_R32G32_SFLOAT;
		case ImageFormat::SRGB:				return VK_FORMAT_R8G8B8A8_SRGB;

		case ImageFormat::DEPTH32F:			return VK_FORMAT_D32_SFLOAT;
		case ImageFormat::DEPTH24STENCIL8:	return VulkanContext::GetLogicalDevice()->GetPhysicalDevice().As<VulkanPhysicalDevice>()->GetDepthFormat();
		}
		HZR_ASSERT(false, "");
		return VK_FORMAT_UNDEFINED;
	}
	bool IsIntegratedBase(const ImageFormat& format)
	{
		switch (format)
		{
		case ImageFormat::RED32I:
			return true;
		case ImageFormat::DEPTH32F:
		case ImageFormat::RGBA32F:
		case ImageFormat::RG16F:
		case ImageFormat::RG32F:
		case ImageFormat::RED32F:
		case ImageFormat::RGB:
		case ImageFormat::RGBA:
		case ImageFormat::RGB32F:
		case ImageFormat::RGB16F:
		case ImageFormat::RGBA16F:
		case ImageFormat::SRGB:
		case ImageFormat::DEPTH24STENCIL8:
			return false;
		}
		HZR_ASSERT(false, "IsIntegratedBase undefined");
		return false;
	}
	void InsertImageMemoryBarrier(VkCommandBuffer commandBuffer, VkImage image, VkAccessFlags srcAccess, VkAccessFlags dstAccess, VkImageLayout oldLayout, VkImageLayout newLayout, VkPipelineStageFlags srcFlags, VkPipelineStageFlags dstFlags, VkImageSubresourceRange subresourceRange)
	{
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.srcAccessMask = srcAccess;
		barrier.dstAccessMask = dstAccess;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.image = image;
		barrier.subresourceRange = subresourceRange;

		vkCmdPipelineBarrier(commandBuffer, srcAccess, dstAccess, 0, 0, nullptr, 0, nullptr, 1, &barrier);


	}
}