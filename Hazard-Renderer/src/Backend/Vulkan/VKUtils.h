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
		case VK_OBJECT_TYPE_COMMAND_BUFFER:						return "VK_OBJECT_TYPE_COMMAND_BUFFER";
		case VK_OBJECT_TYPE_PIPELINE:							return "VK_OBJECT_TYPE_PIPELINE";
		case VK_OBJECT_TYPE_FRAMEBUFFER:						return "VK_OBJECT_TYPE_FRAMEBUFFER";
		case VK_OBJECT_TYPE_IMAGE:								return "VK_OBJECT_TYPE_IMAGE";
		case VK_OBJECT_TYPE_QUERY_POOL:							return "VK_OBJECT_TYPE_QUERY_POOL";
		case VK_OBJECT_TYPE_RENDER_PASS:						return "VK_OBJECT_TYPE_RENDER_PASS";
		case VK_OBJECT_TYPE_COMMAND_POOL:						return "VK_OBJECT_TYPE_COMMAND_POOL";
		case VK_OBJECT_TYPE_PIPELINE_CACHE:						return "VK_OBJECT_TYPE_PIPELINE_CACHE";
		case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR:			return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR";
		case VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV:			return "VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_NV";
		case VK_OBJECT_TYPE_BUFFER:								return "VK_OBJECT_TYPE_BUFFER";
		case VK_OBJECT_TYPE_BUFFER_VIEW:						return "VK_OBJECT_TYPE_BUFFER_VIEW";
		case VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT:			return "VK_OBJECT_TYPE_DEBUG_REPORT_CALLBACK_EXT";
		case VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT:			return "VK_OBJECT_TYPE_DEBUG_UTILS_MESSENGER_EXT";
		case VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR:				return "VK_OBJECT_TYPE_DEFERRED_OPERATION_KHR";
		case VK_OBJECT_TYPE_DESCRIPTOR_POOL:					return "VK_OBJECT_TYPE_DESCRIPTOR_POOL";
		case VK_OBJECT_TYPE_DESCRIPTOR_SET:						return "VK_OBJECT_TYPE_DESCRIPTOR_SET";
		case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT:				return "VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT";
		case VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE:			return "VK_OBJECT_TYPE_DESCRIPTOR_UPDATE_TEMPLATE";
		case VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT:				return "VK_OBJECT_TYPE_PRIVATE_DATA_SLOT_EXT";
		case VK_OBJECT_TYPE_DEVICE:								return "VK_OBJECT_TYPE_DEVICE";
		case VK_OBJECT_TYPE_DEVICE_MEMORY:						return "VK_OBJECT_TYPE_DEVICE_MEMORY";
		case VK_OBJECT_TYPE_PIPELINE_LAYOUT:					return "VK_OBJECT_TYPE_PIPELINE_LAYOUT";
		case VK_OBJECT_TYPE_DISPLAY_KHR:						return "VK_OBJECT_TYPE_DISPLAY_KHR";
		case VK_OBJECT_TYPE_DISPLAY_MODE_KHR:					return "VK_OBJECT_TYPE_DISPLAY_MODE_KHR";
		case VK_OBJECT_TYPE_PHYSICAL_DEVICE:					return "VK_OBJECT_TYPE_PHYSICAL_DEVICE";
		case VK_OBJECT_TYPE_EVENT:								return "VK_OBJECT_TYPE_EVENT";
		case VK_OBJECT_TYPE_FENCE:								return "VK_OBJECT_TYPE_FENCE";
		case VK_OBJECT_TYPE_IMAGE_VIEW:							return "VK_OBJECT_TYPE_IMAGE_VIEW";
		case VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV:		return "VK_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NV";
		case VK_OBJECT_TYPE_INSTANCE:							return "VK_OBJECT_TYPE_INSTANCE";
		case VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL:	return "VK_OBJECT_TYPE_PERFORMANCE_CONFIGURATION_INTEL";
		case VK_OBJECT_TYPE_QUEUE:								return "VK_OBJECT_TYPE_QUEUE";
		case VK_OBJECT_TYPE_SAMPLER:							return "VK_OBJECT_TYPE_SAMPLER";
		case VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION:			return "VK_OBJECT_TYPE_SAMPLER_YCBCR_CONVERSION";
		case VK_OBJECT_TYPE_SEMAPHORE:							return "VK_OBJECT_TYPE_SEMAPHORE";
		case VK_OBJECT_TYPE_SHADER_MODULE:						return "VK_OBJECT_TYPE_SHADER_MODULE";
		case VK_OBJECT_TYPE_SURFACE_KHR:						return "VK_OBJECT_TYPE_SURFACE_KHR";
		case VK_OBJECT_TYPE_SWAPCHAIN_KHR:						return "VK_OBJECT_TYPE_SWAPCHAIN_KHR";
		case VK_OBJECT_TYPE_VALIDATION_CACHE_EXT:				return "VK_OBJECT_TYPE_VALIDATION_CACHE_EXT";
		case VK_OBJECT_TYPE_UNKNOWN:							return "VK_OBJECT_TYPE_UNKNOWN";
		case VK_OBJECT_TYPE_MAX_ENUM:							return "VK_OBJECT_TYPE_MAX_ENUM";
		}
		HZR_ASSERT(false, "Unknown");
		return "";
	}
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

	void InsertImageMemoryBarrier(VkCommandBuffer commandBuffer,
		VkImage image,
		VkAccessFlags srcAccessMask,
		VkAccessFlags dstAccessMask,
		VkImageLayout oldLayout,
		VkImageLayout newLayout,
		VkPipelineStageFlags srcStageMask,
		VkPipelineStageFlags dstStageMask,
		VkImageSubresourceRange subresourceRange);


	inline static void SetDebugUtilsObjectName(const VkDevice device, const VkObjectType type, const std::string& name, const void* handle) {
		VkDebugUtilsObjectNameInfoEXT nameInfo = {};
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = type;
		nameInfo.pObjectName = name.c_str();
		nameInfo.objectHandle = (uint64_t)handle;
		nameInfo.pNext = VK_NULL_HANDLE;

		VK_CHECK_RESULT(fpSetDebugUtilsObjectNameEXT(device, &nameInfo), "");
	}
}
#endif