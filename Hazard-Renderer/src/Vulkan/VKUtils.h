#pragma once

#include "VulkanContext.h"

#ifdef HZR_INCLUDE_VULKAN

#include "UtilityCore.h"
#include "VulkanContext.h"

#include <vulkan/vulkan.h>
#include <MathCore.h>

inline PFN_vkSetDebugUtilsObjectNameEXT fpSetDebugUtilsObjectNameEXT;
inline PFN_vkCmdBeginDebugUtilsLabelEXT fpCmdBeginDebugUtilsLabelEXT;
inline PFN_vkCmdEndDebugUtilsLabelEXT fpCmdEndDebugUtilsLabelEXT;
inline PFN_vkCmdInsertDebugUtilsLabelEXT fpCmdInsertDebugUtilsLabelEXT;

namespace HazardRenderer::Vulkan::VkUtils
{
	static void LoadDebugUtilsExtensions(VkInstance instance)
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
	inline static const char* VkObjectTypeToString(const VkObjectType type)
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
	static std::string ResultToString(const VkResult& result)
	{
		switch (result)
		{
			TO_STRING(VK_SUCCESS);
			TO_STRING(VK_NOT_READY);
			TO_STRING(VK_TIMEOUT);
			TO_STRING(VK_EVENT_SET);
			TO_STRING(VK_EVENT_RESET);
			TO_STRING(VK_INCOMPLETE);
			TO_STRING(VK_ERROR_OUT_OF_HOST_MEMORY);
			TO_STRING(VK_ERROR_OUT_OF_DEVICE_MEMORY);
			TO_STRING(VK_ERROR_INITIALIZATION_FAILED);
			TO_STRING(VK_ERROR_DEVICE_LOST);
			TO_STRING(VK_ERROR_MEMORY_MAP_FAILED);
			TO_STRING(VK_ERROR_LAYER_NOT_PRESENT);
			TO_STRING(VK_ERROR_EXTENSION_NOT_PRESENT);
			TO_STRING(VK_ERROR_FEATURE_NOT_PRESENT);
			TO_STRING(VK_ERROR_INCOMPATIBLE_DRIVER);
			TO_STRING(VK_ERROR_TOO_MANY_OBJECTS);
			TO_STRING(VK_ERROR_FORMAT_NOT_SUPPORTED);
			TO_STRING(VK_ERROR_SURFACE_LOST_KHR);
			TO_STRING(VK_SUBOPTIMAL_KHR);
			TO_STRING(VK_ERROR_OUT_OF_DATE_KHR);
			TO_STRING(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
			TO_STRING(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
			TO_STRING(VK_ERROR_VALIDATION_FAILED_EXT);
			TO_STRING(VK_RESULT_MAX_ENUM);
		}
		HZR_ASSERT(false, "Undefined");
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

	static VkFormat VulkanImageFormat(const ImageFormat& format)
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
	static bool IsIntegratedBase(const ImageFormat& format)
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
	static VkPrimitiveTopology GetVulkanTopology(uint32_t flags)
	{
		if (flags & PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		if (flags & PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST)
			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		if (flags & PIPELINE_PRIMITIVE_TOPOLOGY_POINT_LIST)
			return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}
	static VkPolygonMode GetVulkanPolygonMode(uint32_t flags)
	{
		if (flags & PIPELINE_DRAW_FILL)
			return VK_POLYGON_MODE_FILL;
		if (flags & PIPELINE_DRAW_LINE)
			return VK_POLYGON_MODE_LINE;
		if (flags & PIPELINE_DRAW_POINT)
			return VK_POLYGON_MODE_POINT;

		return VK_POLYGON_MODE_MAX_ENUM;
	}
	static VkCullModeFlags GetVulkanCullMode(uint32_t flags)
	{
		if (flags & PIPELINE_CULL_FRONT_FACE)
			return VK_CULL_MODE_FRONT_BIT;
		if (flags & PIPELINE_CULL_BACK_FACE)
			return VK_CULL_MODE_BACK_BIT;
		return VK_CULL_MODE_NONE;
	}
	static VkCompareOp GetVulkanCompareOp(const DepthOp& op)
	{
		switch (op)
		{
			case DepthOp::None:					return VK_COMPARE_OP_MAX_ENUM;
			case DepthOp::Never:				return VK_COMPARE_OP_NEVER;
			case DepthOp::NotEqual:				return VK_COMPARE_OP_NOT_EQUAL;
			case DepthOp::Less:					return VK_COMPARE_OP_LESS;
			case DepthOp::LessOrEqual:			return VK_COMPARE_OP_LESS_OR_EQUAL;
			case DepthOp::Equal:                return VK_COMPARE_OP_EQUAL;
			case DepthOp::Greater:				return VK_COMPARE_OP_GREATER;
			case DepthOp::GreaterOrEqual:		return VK_COMPARE_OP_GREATER_OR_EQUAL;
			case DepthOp::Always:				return VK_COMPARE_OP_ALWAYS;
		}
	}
	static VkFormat ShaderDataTypeToVulkanType(const ShaderDataType& type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return VK_FORMAT_R32_SFLOAT;
			case ShaderDataType::Float2:   return VK_FORMAT_R32G32_SFLOAT;
			case ShaderDataType::Float3:   return VK_FORMAT_R32G32B32_SFLOAT;
			case ShaderDataType::Float4:   return VK_FORMAT_R32G32B32A32_SFLOAT;
			case ShaderDataType::Int:	   return VK_FORMAT_R32_SINT;
		}
		HZR_ASSERT(false, "Woop");
		return VK_FORMAT_MAX_ENUM;
	}
	static VkShaderStageFlags GetVulkanShaderStage(uint32_t stage)
	{
		VkShaderStageFlags flags = 0;

		if (stage & SHADER_STAGE_VERTEX_BIT)
			flags |= VK_SHADER_STAGE_VERTEX_BIT;
		if (stage & SHADER_STAGE_FRAGMENT_BIT)
			flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if (stage & SHADER_STAGE_GEOMETRY_BIT)
			flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
		if (stage & SHADER_STAGE_COMPUTE_BIT)
			flags |= VK_SHADER_STAGE_COMPUTE_BIT;
		if (stage & SHADER_STAGE_RAYGEN_BIT)
			flags |= VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		if (stage & SHADER_STAGE_MISS_BIT)
			flags |= VK_SHADER_STAGE_MISS_BIT_KHR;
		if (stage & SHADER_STAGE_CLOSEST_HIT_BIT)
			flags |= VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		if (stage & SHADER_STAGE_ANY_HIT_BIT)
			flags |= VK_SHADER_STAGE_ANY_HIT_BIT_KHR;

		return flags;
	}
	static uint32_t GetMipLevelCount(uint32_t width, uint32_t height)
	{
		return Math::GetBaseLog<uint32_t>(glm::min(width, height)) + 1;
	}
	static VkImageLayout GetVulkanImageLayout(uint32_t layout)
	{
		switch (layout)
		{
			case IMAGE_LAYOUT_UNDEFINED:					return VK_IMAGE_LAYOUT_UNDEFINED;
			case IMAGE_LAYOUT_GENERAL:						return VK_IMAGE_LAYOUT_GENERAL;
			case IMAGE_LAYOUT_COLOR_ATTACHMENT:				return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT:		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY:		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case IMAGE_LAYOUT_SHADER_READ_ONLY:				return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case IMAGE_LAYOUT_TRANSFER_SRC:					return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case IMAGE_LAYOUT_TRANSFER_DST:					return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case IMAGE_LAYOUT_DEPTH_READ_ONLY:				return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
			case IMAGE_LAYOUT_DEPTH_ATTACHMENT:				return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
		}
		return VK_IMAGE_LAYOUT_MAX_ENUM;
	}

	static VkAccessFlags GetVulkanAccessFlag(uint32_t layout)
	{
		switch (layout)
		{
			case IMAGE_LAYOUT_UNDEFINED:					return 0;
			case IMAGE_LAYOUT_GENERAL:						return 0;
			case IMAGE_LAYOUT_COLOR_ATTACHMENT:				return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			case IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT:		return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			case IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY:		return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			case IMAGE_LAYOUT_SHADER_READ_ONLY:				return VK_ACCESS_SHADER_READ_BIT;
			case IMAGE_LAYOUT_TRANSFER_SRC:					return VK_ACCESS_TRANSFER_READ_BIT;
			case IMAGE_LAYOUT_TRANSFER_DST:					return VK_ACCESS_TRANSFER_WRITE_BIT;
			case IMAGE_LAYOUT_DEPTH_READ_ONLY:				return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			case IMAGE_LAYOUT_DEPTH_ATTACHMENT:				return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

		}
		return 0;
	}

	static void InsertImageMemoryBarrier(VkCommandBuffer commandBuffer,
										 VkImage image,
										 uint32_t oldLayout,
										 uint32_t newLayout, VkImageSubresourceRange range)
	{
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		barrier.srcAccessMask = GetVulkanAccessFlag(oldLayout);
		barrier.dstAccessMask = GetVulkanAccessFlag(newLayout);
		barrier.oldLayout = GetVulkanImageLayout(oldLayout);
		barrier.newLayout = GetVulkanImageLayout(newLayout);
		barrier.image = image;
		barrier.subresourceRange = range;

		vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}

	static void InsertImageMemoryBarrier(VkCommandBuffer commandBuffer,
										 VkImage image,
										 VkAccessFlags srcAccessMask,
										 VkAccessFlags dstAccessMask,
										 VkImageLayout oldLayout,
										 VkImageLayout newLayout,
										 VkPipelineStageFlags srcStageMask,
										 VkPipelineStageFlags dstStageMask,
										 VkImageSubresourceRange subresourceRange)
	{
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.srcAccessMask = srcAccessMask;
		barrier.dstAccessMask = dstAccessMask;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.image = image;
		barrier.subresourceRange = subresourceRange;
		vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &barrier);
	}


	static void SetImageLayout(VkCommandBuffer commandBuffer,
							   VkImage image,
							   VkImageLayout oldLayout,
							   VkImageLayout newLayout,
							   VkImageSubresourceRange subresourceRange,
							   VkPipelineStageFlags srcStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
							   VkPipelineStageFlags dstStageMask = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT
	)
	{
		VkImageMemoryBarrier barrier = {};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.oldLayout = oldLayout;
		barrier.newLayout = newLayout;
		barrier.image = image;
		barrier.subresourceRange = subresourceRange;

		switch (oldLayout)
		{
			case VK_IMAGE_LAYOUT_UNDEFINED:							barrier.srcAccessMask = 0; break;
			case VK_IMAGE_LAYOUT_PREINITIALIZED:					barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:			barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:	barrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:				barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT; break;
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:				barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:			barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT; break;
			default: break;
		}

		switch (newLayout)
		{
			case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:					barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:					barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT; break;
			case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:				barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:		barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT; break;
			case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			{
				if (barrier.srcAccessMask == 0)
					barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;

				barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			}
		}

		vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0,
							 0, nullptr, 0, nullptr, 1, &barrier);
	}


	inline static void SetDebugUtilsObjectName(const VkDevice device, const VkObjectType type, const std::string& name, const void* handle)
	{
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
	/*static VkAccessFlags GetVulkanAccess(ImageLayout layout)
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
	*/
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

	static VkDeviceOrHostAddressConstKHR GetBufferAddress(VkBuffer buffer)
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();
		// YEET THIS
		//GET_DEVICE_PROC_ADDR(device, GetBufferDeviceAddressKHR);

		VkBufferDeviceAddressInfo bufferDeviceAddressInfo = {};
		bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		bufferDeviceAddressInfo.buffer = buffer;

		VkDeviceOrHostAddressConstKHR address = {};
		//address.deviceAddress = fpGetBufferDeviceAddressKHR(device, &bufferDeviceAddressInfo);

		return address;
	}
	static VkTransformMatrixKHR MatrixToKHR(const glm::mat4& matrix)
	{
		return VkTransformMatrixKHR
		{
			matrix[0][0],matrix[1][0],matrix[2][0],matrix[3][0],
			matrix[0][1],matrix[1][1],matrix[2][1],matrix[3][1],
			matrix[0][2],matrix[1][2],matrix[2][2],matrix[3][2]
		};
	}
}
#endif