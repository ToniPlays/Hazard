
#include "VKUtils.h"
#include "VulkanContext.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <set>

namespace HazardRenderer::Vulkan {

	std::vector<const char*> VKUtils::GetRequiredExtensions(bool validation)
	{
		uint32_t count = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
		std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);

		if (validation)
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

		return extensions;
	}
	VkPhysicalDevice VKUtils::GetVulkanCapableDevice(VkInstance instance, VkSurfaceKHR surface)
	{
		uint32_t deviceCount = 0;
		vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

		if (deviceCount == 0)
			return VK_NULL_HANDLE;

		std::vector<VkPhysicalDevice> devices(deviceCount);
		vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

		VkPhysicalDevice resultDevice;

		for (const auto& device : devices) {
			if (SuitableDevice(device, instance, surface)) {
				resultDevice = device;
				return resultDevice;
			}
		}
		ASSERT("Failed to find Vulkan capable device");
	}

	bool VKUtils::SuitableDevice(VkPhysicalDevice device, VkInstance instance, VkSurfaceKHR surface)
	{
		QueueFamilyIndices indices = GetQueueFamilyIndices(device, surface);

		bool extensionSupported = CheckDeviceExtensionSupport(device);
		bool swapchainAdequate = false;

		if (extensionSupported) {

			SwapChainSupportDetails details = GetSwapChainDetails(device, surface);
			swapchainAdequate = !details.formats.empty() && !details.presentModes.empty();
		}

		return indices.isComplete() && extensionSupported && swapchainAdequate;
	}

	bool VKUtils::CheckDeviceExtensionSupport(VkPhysicalDevice device)
	{
		uint32_t extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

		for (const auto& extension : availableExtensions) {
			requiredExtensions.erase(extension.extensionName);
		}

		return requiredExtensions.empty();
	}

	SwapChainSupportDetails VKUtils::GetSwapChainDetails(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		SwapChainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

		uint32_t formatCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

		if (formatCount != 0) {
			details.formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
		}

		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

		if (presentModeCount != 0) {
			details.presentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
		}

		return details;
	}

//	VkSurfaceFormatKHR VKUtils::ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats, VkFormat format, VkColorSpaceKHR space)
//	{
//		for (const auto& f : formats) {
//			if (f.format == format && f.colorSpace == space)
//				return f;
//		}
//		return formats[0];
//	}
//
//	VkPresentModeKHR VKUtils::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR preferred, VkPresentModeKHR defaultMode)
//	{
//		for (const auto& mode : modes) {
//			if (mode == preferred)
//				return mode;
//		}
//		return defaultMode;
//	}
//
//	VkExtent2D VKUtils::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, int w, int h)
//	{
//		if (capabilities.currentExtent.width != UINT32_MAX)
//			return capabilities.currentExtent;
//		VkExtent2D actualExtent = { static_cast<uint32_t>(w),
//									static_cast<uint32_t>(h) };
//
//		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
//		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
//
//		return actualExtent;
//	}
//	VkFormat VKUtils::GetFormat(ImageFormat format)
//	{
//		switch (format)
//		{
//		case ImageFormat::RED32F:			return VK_FORMAT_R32_SFLOAT;
//		case ImageFormat::RG16F:			return VK_FORMAT_R16G16_SFLOAT;
//		case ImageFormat::RG32F:			return VK_FORMAT_R32G32_SFLOAT;
//		case ImageFormat::RGB:				return VK_FORMAT_R8G8B8_UNORM;
//		case ImageFormat::RGBA:				return VK_FORMAT_R8G8B8A8_UNORM;
//		case ImageFormat::RGBA16F:			return VK_FORMAT_R16G16B16A16_SFLOAT;
//		case ImageFormat::RGBA32F:			return VK_FORMAT_R32G32B32A32_SFLOAT;
//		case ImageFormat::DEPTH32F:			return VK_FORMAT_D32_SFLOAT;
//		case ImageFormat::RED32I:			return VK_FORMAT_R32_SINT;
//		case ImageFormat::DEPTH24STENCIL8:	return VulkanContext::GetDevice()->GetDepthFormat();
//		}
//		HZR_CORE_ASSERT(false, "[VKUtils] Failed to convert format");
//		return VK_FORMAT_UNDEFINED;
//	}
//
//	VkFormat VKUtils::ShaderDataTypeToVkFormat(ShaderDataType type)
//	{
//		switch (type)
//		{
//		case ShaderDataType::Float:    return VK_FORMAT_R32_SFLOAT;
//		case ShaderDataType::Float2:   return VK_FORMAT_R32G32_SFLOAT;
//		case ShaderDataType::Float3:   return VK_FORMAT_R32G32B32_SFLOAT;
//		case ShaderDataType::Float4:   return VK_FORMAT_R32G32B32A32_SFLOAT;
//		case ShaderDataType::Mat3:     return VK_FORMAT_R32G32B32_SFLOAT;
//		case ShaderDataType::Mat4:     return VK_FORMAT_UNDEFINED;
//		case ShaderDataType::Int:      return VK_FORMAT_R32_UINT;
//		case ShaderDataType::Int2:     return VK_FORMAT_R32G32_UINT;
//		case ShaderDataType::Int3:     return VK_FORMAT_R32G32B32_UINT;
//		case ShaderDataType::Int4:     return VK_FORMAT_R32G32B32A32_UINT;
//		case ShaderDataType::Bool:     return VK_FORMAT_R8_UINT;
//		}
//		return VK_FORMAT_UNDEFINED;
//	}
//
//	ShaderType VKUtils::ShaderTypeFromVulkanStage(VkShaderStageFlagBits type)
//	{
//		switch (type)
//		{
//		case VK_SHADER_STAGE_VERTEX_BIT:	return ShaderType::Vertex;
//		case VK_SHADER_STAGE_FRAGMENT_BIT:	return ShaderType::Fragment;
//		case VK_SHADER_STAGE_COMPUTE_BIT:	return ShaderType::Compute;
//		case VK_SHADER_STAGE_GEOMETRY_BIT:	return ShaderType::Geometry;
//		}
//		return ShaderType::None;
//	}
//
//	VkCullModeFlags VKUtils::CullModeToVKMode(CullMode mode)
//	{
//		switch (mode)
//		{
//		case CullMode::None:		return VK_CULL_MODE_NONE;
//		case CullMode::FrontFace:	return VK_CULL_MODE_FRONT_BIT;
//		case CullMode::BackFace:	return VK_CULL_MODE_BACK_BIT;
//		default:					return VK_CULL_MODE_NONE;
//		}
//		return VK_CULL_MODE_FLAG_BITS_MAX_ENUM;
//	}
//
//	VkShaderStageFlags VKUtils::ShaderUsageToVulkanUsage(uint32_t usage)
//	{
//		VkShaderStageFlags flags = 0;
//		if (usage & (uint32_t)ShaderType::Vertex)		flags |= VK_SHADER_STAGE_VERTEX_BIT;
//		if (usage & (uint32_t)ShaderType::Fragment)		flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
//		if (usage & (uint32_t)ShaderType::Compute)		flags |= VK_SHADER_STAGE_COMPUTE_BIT;
//		if (usage & (uint32_t)ShaderType::Geometry)		flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
//		return flags;
//	}
//
//	bool VKUtils::IsDepth(ImageFormat format)
//	{
//		if (format == ImageFormat::DEPTH24STENCIL8 || format == ImageFormat::DEPTH32F)
//			return true;
//		return false;
//	}
//
//	VkFilter VKUtils::GetSamplerFilter(const FilterMode& filter)
//	{
//		switch (filter) {
//		case FilterMode::Linear:		return VK_FILTER_LINEAR;
//		case FilterMode::LinearMip:		return VK_FILTER_LINEAR;
//		case FilterMode::Nearest:		return VK_FILTER_NEAREST;
//		case FilterMode::NearestMip:	return VK_FILTER_NEAREST;
//		}
//		return VK_FILTER_LINEAR;
//	}
//
//	VkSamplerAddressMode VKUtils::GetSamplerWrap(const ImageWrap& wrap)
//	{
//		switch (wrap) {
//		case ImageWrap::ClampBorder:	return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
//		case ImageWrap::ClampEdge:		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
//		case ImageWrap::Repeat:			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
//		case ImageWrap::RepeatMirror:	return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
//		}
//		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
//	}
//
//	VkPolygonMode VKUtils::DrawTypeToVKType(const DrawType& type)
//	{
//		switch (type)
//		{
//		case DrawType::Fill: return VK_POLYGON_MODE_FILL;
//		case DrawType::Line: return VK_POLYGON_MODE_LINE;
//		case DrawType::Point: return VK_POLYGON_MODE_POINT;
//		}
//		return VK_POLYGON_MODE_FILL;
//	}
//
	VkPrimitiveTopology VKUtils::DrawTypeToVKTopology(const DrawType& type)
	{
		switch (type) {
		case DrawType::Fill: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case DrawType::Line: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case DrawType::Point: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
		}
		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}
//
//	void VKUtils::SetImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
//		VkImageSubresourceRange subresourceRange, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask)
//	{
//		VkImageMemoryBarrier memoryBarrier = {};
//		memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//		memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//		memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//		memoryBarrier.oldLayout = oldImageLayout;
//		memoryBarrier.newLayout = newImageLayout;
//		memoryBarrier.image = image;
//		memoryBarrier.subresourceRange = subresourceRange;
//
//		switch (oldImageLayout) {
//		case VK_IMAGE_LAYOUT_UNDEFINED:
//			memoryBarrier.srcAccessMask = 0;
//			break;
//		case VK_IMAGE_LAYOUT_PREINITIALIZED:
//			memoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
//			break;
//#ifndef HZR_PLATFORM_MACOS // MVK
//		case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR:
//			memoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//			break;
//#endif
//		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
//			memoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//			break;
//		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
//			memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//			break;
//		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
//			memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//			break;
//		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
//			memoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
//			break;
//		}
//		switch (newImageLayout)
//		{
//		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
//			memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
//			break;
//
//		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
//			memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
//			break;
//
//		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
//			memoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
//			break;
//
//		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
//			memoryBarrier.dstAccessMask = memoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
//			break;
//
//		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
//			if (memoryBarrier.srcAccessMask == 0)
//			{
//				memoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
//			}
//			memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
//			break;
//		default:
//			break;
//		}
//
//		vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &memoryBarrier);
//
//	}
//
//	void VKUtils::InsertImageMemoryBarrier(VkCommandBuffer cmdbuffer, VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange)
//	{
//		VkImageMemoryBarrier imageMemoryBarrier{};
//		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
//		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
//
//		imageMemoryBarrier.srcAccessMask = srcAccessMask;
//		imageMemoryBarrier.dstAccessMask = dstAccessMask;
//		imageMemoryBarrier.oldLayout = oldImageLayout;
//		imageMemoryBarrier.newLayout = newImageLayout;
//		imageMemoryBarrier.image = image;
//		imageMemoryBarrier.subresourceRange = subresourceRange;
//
//		vkCmdPipelineBarrier(
//			cmdbuffer,
//			srcStageMask,
//			dstStageMask,
//			0,
//			0, nullptr,
//			0, nullptr,
//			1, &imageMemoryBarrier);
//	}
//
	std::string VKUtils::ResultToString(VkResult result)
	{
		switch (result)
		{
		case VK_SUCCESS:											return "VK_SUCCESS";
		case VK_NOT_READY:											return "VK_NOT_READY";
		case VK_TIMEOUT:											return "VK_TIMEOUT";
		case VK_EVENT_SET:											return "VK_EVENT_SET";
		case VK_EVENT_RESET:										return "VK_EVENT_RESET";
		case VK_INCOMPLETE:											return "VK_INCOMPLETE";
		case VK_ERROR_OUT_OF_HOST_MEMORY:							return "VK_ERROR_OUT_OF_HOST_MEMORY";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:							return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
		case VK_ERROR_INITIALIZATION_FAILED:						return "VK_ERROR_INITIALIZATION_FAILED";
		case VK_ERROR_DEVICE_LOST:									return "VK_ERROR_DEVICE_LOST";
		case VK_ERROR_MEMORY_MAP_FAILED:							return "VK_ERROR_MEMORY_MAP_FAILED";
		case VK_ERROR_LAYER_NOT_PRESENT:							return "VK_ERROR_LAYER_NOT_PRESENT";
		case VK_ERROR_EXTENSION_NOT_PRESENT:						return "VK_ERROR_EXTENSION_NOT_PRESENT";
		case VK_ERROR_FEATURE_NOT_PRESENT:							return "VK_ERROR_FEATURE_NOT_PRESENT";
		case VK_ERROR_INCOMPATIBLE_DRIVER:							return "VK_ERROR_INCOMPATIBLE_DRIVER";
		case VK_ERROR_TOO_MANY_OBJECTS:								return "VK_ERROR_TOO_MANY_OBJECTS";
		case VK_ERROR_FORMAT_NOT_SUPPORTED:							return "VK_ERROR_FORMAT_NOT_SUPPORTED";
		case VK_ERROR_FRAGMENTED_POOL:								return "VK_ERROR_FRAGMENTED_POOL";
		case VK_ERROR_UNKNOWN:										return "VK_ERROR_UNKNOWN";
		case VK_ERROR_OUT_OF_POOL_MEMORY:							return "VK_ERROR_OUT_OF_POOL_MEMORY";
		case VK_ERROR_INVALID_EXTERNAL_HANDLE:						return "VK_ERROR_INVALID_EXTERNAL_HANDLE";
		case VK_ERROR_FRAGMENTATION:								return "VK_ERROR_FRAGMENTATION";
		case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:				return "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS";
		case VK_ERROR_SURFACE_LOST_KHR:								return "VK_ERROR_SURFACE_LOST_KHR";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:						return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
		case VK_SUBOPTIMAL_KHR:										return "VK_SUBOPTIMAL_KHR";
		case VK_ERROR_OUT_OF_DATE_KHR:								return "VK_ERROR_OUT_OF_DATE_KHR";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:						return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
		case VK_ERROR_VALIDATION_FAILED_EXT:						return "VK_ERROR_VALIDATION_FAILED_EXT";
		case VK_ERROR_INVALID_SHADER_NV:							return "VK_ERROR_INVALID_SHADER_NV";
		case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT:	return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT";
		case VK_ERROR_NOT_PERMITTED_EXT:							return "VK_ERROR_NOT_PERMITTED_EXT";
		case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:			return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT";
		case VK_THREAD_IDLE_KHR:									return "VK_THREAD_IDLE_KHR";
		case VK_THREAD_DONE_KHR:									return "VK_THREAD_DONE_KHR";
		case VK_OPERATION_DEFERRED_KHR:								return "VK_OPERATION_DEFERRED_KHR";
		case VK_OPERATION_NOT_DEFERRED_KHR:							return "VK_OPERATION_NOT_DEFERRED_KHR";
		case VK_PIPELINE_COMPILE_REQUIRED_EXT:						return "VK_PIPELINE_COMPILE_REQUIRED_EXT";
		}
		return "VK_UNKNONW_ERROR";
	}

	QueueFamilyIndices VKUtils::GetQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface)
	{
		QueueFamilyIndices indices = {};

		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		int i = 0;
		for (const auto& queueFamily : queueFamilies) {
			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			}

			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (presentSupport) {
				indices.presentFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}
			i++;
		}

		return indices;
	}
//	VkSurfaceCapabilitiesKHR VKUtils::GetSurfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
//	{
//		VkSurfaceCapabilitiesKHR surfCaps;
//		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfCaps);
//		return surfCaps;
//	}
}