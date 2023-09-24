#pragma once

#include "VulkanContext.h"
#ifdef HZR_INCLUDE_VULKAN
#include "UtilityCore.h"
#include <vulkan/vulkan.h>

inline PFN_vkSetDebugUtilsObjectNameEXT fpSetDebugUtilsObjectNameEXT;
inline PFN_vkCmdBeginDebugUtilsLabelEXT fpCmdBeginDebugUtilsLabelEXT;
inline PFN_vkCmdEndDebugUtilsLabelEXT fpCmdEndDebugUtilsLabelEXT;
inline PFN_vkCmdInsertDebugUtilsLabelEXT fpCmdInsertDebugUtilsLabelEXT;

namespace HazardRenderer::Vulkan::VkUtils
{
	void LoadDebugUtilsExtensions(VkInstance instance);

	inline const char* VkObjectTypeToString(const VkObjectType type)
	{
		switch (type)
		{
			TO_STRING(VK_OBJECT_TYPE_COMMAND_BUFFER);
			TO_STRING(VK_OBJECT_TYPE_PIPELINE);
			TO_STRING(VK_OBJECT_TYPE_FRAMEBUFFER);
			TO_STRING(VK_OBJECT_TYPE_IMAGE);
			TO_STRING(VK_OBJECT_TYPE_QUERY_POOL);
			TO_STRING(VK_OBJECT_TYPE_RENDER_PASS);
			TO_STRING(VK_OBJECT_TYPE_COMMAND_POOL);
			TO_STRING(VK_OBJECT_TYPE_PIPELINE_CACHE);
			TO_STRING(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR);
			TO_STRING(VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV);
			TO_STRING(VK_OBJECT_TYPE_BUFFER);
			TO_STRING(VK_OBJECT_TYPE_BUFFER_VIEW);
			TO_STRING(VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT);
			TO_STRING(VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT);
			TO_STRING(VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR);
			TO_STRING(VK_OBJECT_TYPE_DESCRIPTOR_POOL);
			TO_STRING(VK_OBJECT_TYPE_DESCRIPTOR_SET);
			TO_STRING(VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT);
			TO_STRING(VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE);
			TO_STRING(VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT);
			TO_STRING(VK_OBJECT_TYPE_DEVICE);
			TO_STRING(VK_OBJECT_TYPE_DEVICE_MEMORY);
			TO_STRING(VK_OBJECT_TYPE_PIPELINE_LAYOUT);
			TO_STRING(VK_OBJECT_TYPE_DISPLAY_KHR);
			TO_STRING(VK_OBJECT_TYPE_DISPLAY_MODE_KHR);
			TO_STRING(VK_OBJECT_TYPE_PHYSICAL_DEVICE);
			TO_STRING(VK_OBJECT_TYPE_EVENT);
			TO_STRING(VK_OBJECT_TYPE_FENCE);
			TO_STRING(VK_OBJECT_TYPE_IMAGE_VIEW);
			TO_STRING(VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV);
			TO_STRING(VK_OBJECT_TYPE_INSTANCE);
			TO_STRING(VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL);
			TO_STRING(VK_OBJECT_TYPE_QUEUE);
			TO_STRING(VK_OBJECT_TYPE_SAMPLER);
			TO_STRING(VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION);
			TO_STRING(VK_OBJECT_TYPE_SEMAPHORE);
			TO_STRING(VK_OBJECT_TYPE_SHADER_MODULE);
			TO_STRING(VK_OBJECT_TYPE_SURFACE_KHR);
			TO_STRING(VK_OBJECT_TYPE_SWAPCHAIN_KHR);
			TO_STRING(VK_OBJECT_TYPE_VALIDATION_CACHE_EXT);
			TO_STRING(VK_OBJECT_TYPE_UNKNOWN);
			TO_STRING(VK_OBJECT_TYPE_MAX_ENUM);
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

		fpSetDebugUtilsObjectNameEXT(device, &nameInfo);
	}
	static std::string ImageLayoutToString(VkImageLayout layout)
	{
		switch (layout)
		{
			TO_STRING(VK_IMAGE_LAYOUT_UNDEFINED);
			TO_STRING(VK_IMAGE_LAYOUT_GENERAL);
			TO_STRING(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_PREINITIALIZED);
			TO_STRING(VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL);
			TO_STRING(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
			TO_STRING(VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR);
			TO_STRING(VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT);
			TO_STRING(VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR);
			TO_STRING(VK_IMAGE_LAYOUT_MAX_ENUM);
		}
		HZR_ASSERT(false, "What is this");
		return "";
	}

	static VkImageLayout GetVulkanImageLayout(ImageLayout layout)
	{
		switch (layout)
		{
		case ImageLayout::ImageLayout_General:			return VK_IMAGE_LAYOUT_GENERAL;
		case ImageLayout::ImageLayout_ShaderReadOnly:	return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		}
		return VK_IMAGE_LAYOUT_MAX_ENUM;
	}
	static VkFilter GetVulkanFilter(FilterMode mode)
	{
		switch (mode)
		{
		case FilterMode::Nearest:		return VK_FILTER_NEAREST;
		case FilterMode::NearestMip:	return VK_FILTER_NEAREST;
		case FilterMode::Linear:		return VK_FILTER_LINEAR;
		case FilterMode::LinearMip:		return VK_FILTER_LINEAR;
		}
		return VK_FILTER_MAX_ENUM;
	}
	static VkSamplerMipmapMode GetVulkanMipmapMode(FilterMode mode)
	{
		switch (mode)
		{
		case FilterMode::Nearest:		return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		case FilterMode::NearestMip:	return VK_SAMPLER_MIPMAP_MODE_NEAREST;
		case FilterMode::Linear:		return VK_SAMPLER_MIPMAP_MODE_LINEAR;
		case FilterMode::LinearMip:		return VK_SAMPLER_MIPMAP_MODE_LINEAR;
		}
		return VK_SAMPLER_MIPMAP_MODE_MAX_ENUM;
	}
	static VkAccessFlags GetVulkanAccess(ImageLayout layout)
	{
		switch (layout)
		{
		case ImageLayout::ImageLayout_General:			return VK_ACCESS_TRANSFER_READ_BIT;
		case ImageLayout::ImageLayout_ShaderReadOnly:	return VK_ACCESS_SHADER_READ_BIT;
		}
		return VK_ACCESS_FLAG_BITS_MAX_ENUM;
	}
	static VkAccessFlags GetVulkanStage(ImageLayout layout)
	{
		switch (layout)
		{
		case ImageLayout::ImageLayout_General:			return VK_PIPELINE_STAGE_TRANSFER_BIT;
		case ImageLayout::ImageLayout_ShaderReadOnly:	return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		return VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM;
	}
	static VkBufferUsageFlags GetUsageFlags(uint32_t flags)
	{
		uint32_t result = 0;
		if (flags & BUFFER_USAGE_VERTEX_BUFFER_BIT)
			result |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		if (flags & BUFFER_USAGE_INDEX_BUFFER_BIT)
			result |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if (flags & BUFFER_USAGE_UNIFORM_BUFFER_BIT)
			result |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		if (flags & BUFFER_USAGE_STORAGE_BUFFER_BIT)
			result |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		if (flags & BUFFER_USAGE_INDIRECT_BIT)
			result |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
		if (flags & BUFFER_USAGE_ACCELERATION_STRUCTURE)
			result |= VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR;

		return result;
	}

	static VkDescriptorType GetDescriptorType(DescriptorType type)
	{
		switch (type)
		{
		case DESCRIPTOR_TYPE_SAMPLER_2D:				return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		case DESCRIPTOR_TYPE_SAMPLER_CUBE:				return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		case DESCRIPTOR_TYPE_STORAGE_IMAGE:				return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		case DESCRIPTOR_TYPE_UNIFORM_BUFFER:			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case DESCRIPTOR_TYPE_STORAGE_BUFFER:			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		case DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE:	return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
		}
		HZR_ASSERT(false, "");
		return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}

	VkDeviceOrHostAddressConstKHR GetBufferAddress(VkBuffer buffer);
	VkTransformMatrixKHR MatrixToKHR(const glm::mat4& matrix);
}
#endif