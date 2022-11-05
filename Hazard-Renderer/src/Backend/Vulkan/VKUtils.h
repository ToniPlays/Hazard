#pragma once

#include "VulkanContext.h"
#ifdef HZR_INCLUDE_VULKAN
#include "UtilityCore.h"
#include <vulkan/vulkan.h>

inline PFN_vkSetDebugUtilsObjectNameEXT fpSetDebugUtilsObjectNameEXT;
inline PFN_vkCmdBeginDebugUtilsLabelEXT fpCmdBeginDebugUtilsLabelEXT;
inline PFN_vkCmdEndDebugUtilsLabelEXT fpCmdEndDebugUtilsLabelEXT;
inline PFN_vkCmdInsertDebugUtilsLabelEXT fpCmdInsertDebugUtilsLabelEXT;

#define VK_TO_STRING(x) case x: return #x

namespace HazardRenderer::Vulkan::VkUtils
{
	void LoadDebugUtilsExtensions(VkInstance instance);

	inline const char* VkObjectTypeToString(const VkObjectType type)
	{
		switch (type)
		{
			VK_TO_STRING(VK_OBJECT_TYPE_COMMAND_BUFFER);
			VK_TO_STRING(VK_OBJECT_TYPE_PIPELINE);
			VK_TO_STRING(VK_OBJECT_TYPE_FRAMEBUFFER);
			VK_TO_STRING(VK_OBJECT_TYPE_IMAGE);
			VK_TO_STRING(VK_OBJECT_TYPE_QUERY_POOL);
			VK_TO_STRING(VK_OBJECT_TYPE_RENDER_PASS);
			VK_TO_STRING(VK_OBJECT_TYPE_COMMAND_POOL);
			VK_TO_STRING(VK_OBJECT_TYPE_PIPELINE_CACHE);
			VK_TO_STRING(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR);
			VK_TO_STRING(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV);
			VK_TO_STRING(VK_OBJECT_TYPE_BUFFER);
			VK_TO_STRING(VK_OBJECT_TYPE_BUFFER_VIEW);
			VK_TO_STRING(VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT);
			VK_TO_STRING(VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT);
			VK_TO_STRING(VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR);
			VK_TO_STRING(VK_OBJECT_TYPE_DESCRIPTOR_POOL);
			VK_TO_STRING(VK_OBJECT_TYPE_DESCRIPTOR_SET);
			VK_TO_STRING(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT);
			VK_TO_STRING(VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE);
			VK_TO_STRING(VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT);
			VK_TO_STRING(VK_OBJECT_TYPE_DEVICE);
			VK_TO_STRING(VK_OBJECT_TYPE_DEVICE_MEMORY);
			VK_TO_STRING(VK_OBJECT_TYPE_PIPELINE_LAYOUT);
			VK_TO_STRING(VK_OBJECT_TYPE_DISPLAY_KHR);
			VK_TO_STRING(VK_OBJECT_TYPE_DISPLAY_MODE_KHR);
			VK_TO_STRING(VK_OBJECT_TYPE_PHYSICAL_DEVICE);
			VK_TO_STRING(VK_OBJECT_TYPE_EVENT);
			VK_TO_STRING(VK_OBJECT_TYPE_FENCE);
			VK_TO_STRING(VK_OBJECT_TYPE_IMAGE_VIEW);
			VK_TO_STRING(VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV);
			VK_TO_STRING(VK_OBJECT_TYPE_INSTANCE);
			VK_TO_STRING(VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL);
			VK_TO_STRING(VK_OBJECT_TYPE_QUEUE);
			VK_TO_STRING(VK_OBJECT_TYPE_SAMPLER);
			VK_TO_STRING(VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION);
			VK_TO_STRING(VK_OBJECT_TYPE_SEMAPHORE);
			VK_TO_STRING(VK_OBJECT_TYPE_SHADER_MODULE);
			VK_TO_STRING(VK_OBJECT_TYPE_SURFACE_KHR);
			VK_TO_STRING(VK_OBJECT_TYPE_SWAPCHAIN_KHR);
			VK_TO_STRING(VK_OBJECT_TYPE_VALIDATION_CACHE_EXT);
			VK_TO_STRING(VK_OBJECT_TYPE_UNKNOWN);
			VK_TO_STRING(VK_OBJECT_TYPE_MAX_ENUM);
		}
		HZR_ASSERT(false, "Unknown");
		return "";
	}

	std::string ResultToString(const VkResult& result);

	constexpr const char* VkDebugUtilsMessageType(const VkDebugUtilsMessageTypeFlagsEXT type)
	{
		switch (type)
		{
		case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:		return "General";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:	return "Validation";
		case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:	return "Performance";
		default:												return "Unknown";
		}
	}
	constexpr const char* VkDebugUtilsMessageSeverity(const VkDebugUtilsMessageSeverityFlagBitsEXT severity)
	{
		switch (severity)
		{
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:		return "error";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:	return "warning";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:		return "info";
		case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:	return "verbose";
		default:												return "unknown";
		}
	}

	VkFormat VulkanImageFormat(const ImageFormat& format);
	bool IsIntegratedBase(const ImageFormat& format);
	VkPrimitiveTopology GetVulkanTopology(const DrawType& type);
	VkPolygonMode GetVulkanPolygonMode(const DrawType& type);
	VkCullModeFlags GetVulkanCullMode(const CullMode& type);
	VkCompareOp GetVulkanCompareOp(const DepthOp& op);
	VkFormat ShaderDataTypeToVulkanType(const ShaderDataType& type);
	VkShaderStageFlags GetVulkanShaderStage(uint32_t stage);

	uint32_t GetMipLevelCount(uint32_t width, uint32_t height);

	void InsertImageMemoryBarrier(VkCommandBuffer commandBuffer,
		VkImage image,
		VkAccessFlags srcAccessMask,
		VkAccessFlags dstAccessMask,
		VkImageLayout oldLayout,
		VkImageLayout newLayout,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		VkImageSubresourceRange subresourceRange);
	void SetImageLayout(VkCommandBuffer commandBuffer,
		VkImage image,
		VkImageLayout oldLayout,
		VkImageLayout newLayout,
		VkImageSubresourceRange subresourceRange,
		VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
		VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
	);


	inline static void SetDebugUtilsObjectName(const VkDevice device, const VkObjectType type, const std::string& name, const void* handle) {
		VkDebugUtilsObjectNameInfoEXT nameInfo = {};
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = type;
		nameInfo.pObjectName = name.c_str();
		nameInfo.objectHandle = (uint64_t)handle;
		nameInfo.pNext = VK_NULL_HANDLE;
	}
	static std::string ImageLayoutToString(VkImageLayout layout)
	{
		switch (layout)
		{
			VK_TO_STRING(VK_IMAGE_LAYOUT_UNDEFINED);
			VK_TO_STRING(VK_IMAGE_LAYOUT_GENERAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_PREINITIALIZED);
			VK_TO_STRING(VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL);
			VK_TO_STRING(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
			VK_TO_STRING(VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR);
			VK_TO_STRING(VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT);
			VK_TO_STRING(VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR);
			VK_TO_STRING(VK_IMAGE_LAYOUT_MAX_ENUM);
		}
		HZR_ASSERT(false, "What is this");
		return "";
	}

	static VkImageLayout GetVulkanImageLayout(const ImageLayout& layout)
	{
		switch (layout)
		{
		case ImageLayout::ImageLayout_General:			return VK_IMAGE_LAYOUT_GENERAL;
		case ImageLayout::ImageLayout_ShaderReadOnly:	return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}
		return VK_IMAGE_LAYOUT_MAX_ENUM;
	}
	static VkAccessFlags GetVulkanAccess(const ImageLayout& layout)
	{
		switch (layout)
		{
		case ImageLayout::ImageLayout_General:			return VK_ACCESS_TRANSFER_READ_BIT;
		case ImageLayout::ImageLayout_ShaderReadOnly:	return VK_ACCESS_SHADER_READ_BIT;
		}
		return VK_ACCESS_FLAG_BITS_MAX_ENUM;
	}
	static VkAccessFlags GetVulkanStage(const ImageLayout& layout)
	{
		switch (layout)
		{
		case ImageLayout::ImageLayout_General:			return VK_PIPELINE_STAGE_TRANSFER_BIT;
		case ImageLayout::ImageLayout_ShaderReadOnly:	return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		return VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM;
	}
	VkDeviceOrHostAddressConstKHR GetBufferAddress(VkBuffer buffer);
}
#endif