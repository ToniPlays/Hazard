#include "VkUtils.h"
#ifdef HZR_INCLUDE_VULKAN

#include "VulkanContext.h"

namespace HazardRenderer::Vulkan::VkUtils 
{
	static PFN_vkGetBufferDeviceAddressKHR fpGetBufferDeviceAddressKHR;

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
	std::string ResultToString(const VkResult& result)
	{
		switch (result)
		{
			VK_TO_STRING(VK_SUCCESS);
			VK_TO_STRING(VK_NOT_READY);
			VK_TO_STRING(VK_TIMEOUT);
			VK_TO_STRING(VK_EVENT_SET);
			VK_TO_STRING(VK_EVENT_RESET);
			VK_TO_STRING(VK_INCOMPLETE);
			VK_TO_STRING(VK_ERROR_OUT_OF_HOST_MEMORY);
			VK_TO_STRING(VK_ERROR_OUT_OF_DEVICE_MEMORY);
			VK_TO_STRING(VK_ERROR_INITIALIZATION_FAILED);
			VK_TO_STRING(VK_ERROR_DEVICE_LOST);
			VK_TO_STRING(VK_ERROR_MEMORY_MAP_FAILED);
			VK_TO_STRING(VK_ERROR_LAYER_NOT_PRESENT);
			VK_TO_STRING(VK_ERROR_EXTENSION_NOT_PRESENT);
			VK_TO_STRING(VK_ERROR_FEATURE_NOT_PRESENT);
			VK_TO_STRING(VK_ERROR_INCOMPATIBLE_DRIVER);
			VK_TO_STRING(VK_ERROR_TOO_MANY_OBJECTS);
			VK_TO_STRING(VK_ERROR_FORMAT_NOT_SUPPORTED);
			VK_TO_STRING(VK_ERROR_SURFACE_LOST_KHR);
			VK_TO_STRING(VK_SUBOPTIMAL_KHR);
			VK_TO_STRING(VK_ERROR_OUT_OF_DATE_KHR);
			VK_TO_STRING(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
			VK_TO_STRING(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
			VK_TO_STRING(VK_ERROR_VALIDATION_FAILED_EXT);
			VK_TO_STRING(VK_RESULT_MAX_ENUM);
		}
		HZR_ASSERT(false, "Undefined");
		return "";
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
	VkPrimitiveTopology GetVulkanTopology(const DrawType& type)
	{
		switch (type)
		{
		case DrawType::Fill:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case DrawType::Line:	return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case DrawType::Point:	return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		}
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}
	VkPolygonMode GetVulkanPolygonMode(const DrawType& type)
	{
		switch (type)
		{
		case DrawType::Fill:	return VK_POLYGON_MODE_FILL;
		case DrawType::Line:	return VK_POLYGON_MODE_LINE;
		case DrawType::Point:	return VK_POLYGON_MODE_POINT;
		}
		return VK_POLYGON_MODE_MAX_ENUM;
	}
	VkCullModeFlags GetVulkanCullMode(const CullMode& type)
	{
		switch (type)
		{
		case CullMode::None:		return VK_CULL_MODE_NONE;
		case CullMode::FrontFace:	return VK_CULL_MODE_FRONT_BIT;
		case CullMode::BackFace:	return VK_CULL_MODE_BACK_BIT;
		}
		return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
	}
	VkCompareOp GetVulkanCompareOp(const DepthOp& op)
	{
		switch (op)
		{
		case DepthOp::None:					return VK_COMPARE_OP_MAX_ENUM;
		case DepthOp::Never:				return VK_COMPARE_OP_NEVER;
		case DepthOp::NotEqual:				return VK_COMPARE_OP_NOT_EQUAL;
		case DepthOp::Less:					return VK_COMPARE_OP_LESS;
		case DepthOp::LessOrEqual:			return VK_COMPARE_OP_LESS_OR_EQUAL;
		case DepthOp::Greater:				return VK_COMPARE_OP_GREATER;
		case DepthOp::GreaterOrEqual:		return VK_COMPARE_OP_GREATER_OR_EQUAL;
		case DepthOp::Always:				return VK_COMPARE_OP_ALWAYS;
		}
	}

	void InsertImageMemoryBarrier(VkCommandBuffer commandBuffer,
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
	void SetImageLayout(VkCommandBuffer commandBuffer, VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageSubresourceRange subresourceRange,
		VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask)
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
			{
				barrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;

			}
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		}
		}

		vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, 0,
			0, nullptr, 0, nullptr, 1, &barrier);
	}
	VkDeviceOrHostAddressConstKHR GetBufferAddress(VkBuffer buffer)
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();
		// YEET THIS
		GET_DEVICE_PROC_ADDR(device, GetBufferDeviceAddressKHR);

		VkBufferDeviceAddressInfo bufferDeviceAddressInfo = {};
		bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		bufferDeviceAddressInfo.buffer = buffer;

		VkDeviceOrHostAddressConstKHR address = {};
		address.deviceAddress = fpGetBufferDeviceAddressKHR(device, &bufferDeviceAddressInfo);

		return address;
	}
	VkTransformMatrixKHR MatrixToKHR(const glm::mat4& matrix)
	{
		return VkTransformMatrixKHR 
		{
			matrix[0][0],matrix[1][0],matrix[2][0],matrix[3][0],
			matrix[0][1],matrix[1][1],matrix[2][1],matrix[3][1],
			matrix[0][2],matrix[1][2],matrix[2][2],matrix[3][2]
		};
	}
	VkShaderStageFlags GetVulkanShaderStage(uint32_t stage)
	{
		VkShaderStageFlags flags = 0;

		if (stage & (uint32_t)ShaderStage::Vertex)
			flags |= VK_SHADER_STAGE_VERTEX_BIT;
		if (stage & (uint32_t)ShaderStage::Fragment)
			flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if (stage & (uint32_t)ShaderStage::Geometry)
			flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
		if (stage & (uint32_t)ShaderStage::Compute)
			flags |= VK_SHADER_STAGE_COMPUTE_BIT;
		if (stage & (uint32_t)ShaderStage::Raygen)
			flags |= VK_SHADER_STAGE_RAYGEN_BIT_KHR;
		if (stage & (uint32_t)ShaderStage::Miss)
			flags |= VK_SHADER_STAGE_MISS_BIT_KHR;
		if (stage & (uint32_t)ShaderStage::ClosestHit)
			flags |= VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
		if (stage & (uint32_t)ShaderStage::AnyHit)
			flags |= VK_SHADER_STAGE_ANY_HIT_BIT_KHR;

		return flags;
	}
	uint32_t GetMipLevelCount(uint32_t width, uint32_t height)
	{
		return (uint32_t)std::floor(std::log2(glm::min(width, height))) + 1;
	}
	VkFormat ShaderDataTypeToVulkanType(const ShaderDataType& type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return VK_FORMAT_R32_SFLOAT;
		case ShaderDataType::Float2:   return VK_FORMAT_R32G32_SFLOAT;
		case ShaderDataType::Float3:   return VK_FORMAT_R32G32B32_SFLOAT;
		case ShaderDataType::Float4:   return VK_FORMAT_R32G32B32A32_SFLOAT;
		}
		HZR_ASSERT(false, "Woop");
		return VK_FORMAT_MAX_ENUM;
	}
}
#endif