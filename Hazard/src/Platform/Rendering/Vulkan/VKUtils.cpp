#pragma once
#include <hzrpch.h>
#include "VKUtils.h"
#include "VulkanContext.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <set>

namespace Hazard::Rendering::Vulkan {

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
		HZR_THROW("Failed to find Vulkan capable device");
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

	VkSurfaceFormatKHR VKUtils::ChooseSwapChainFormat(const std::vector<VkSurfaceFormatKHR>& formats, VkFormat format, VkColorSpaceKHR space)
	{
		for (const auto& f : formats) {
			if (f.format == format && f.colorSpace == space)
				return f;
		}
		return formats[0];
	}

	VkPresentModeKHR VKUtils::ChooseSwapChainPresentMode(const std::vector<VkPresentModeKHR>& modes, VkPresentModeKHR preferred, VkPresentModeKHR defaultMode)
	{
		for (const auto& mode : modes) {
			if (mode == preferred)
				return mode;
		}
		return defaultMode;
	}

	VkExtent2D VKUtils::ChooseSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, int w, int h)
	{
		if (capabilities.currentExtent.width != UINT32_MAX)
			return capabilities.currentExtent;
		VkExtent2D actualExtent = { static_cast<uint32_t>(w),
									static_cast<uint32_t>(h) };

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
	VkFormat VKUtils::GetFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RED32F:          return VK_FORMAT_R32_SFLOAT;
		case ImageFormat::RG16F:		   return VK_FORMAT_R16G16_SFLOAT;
		case ImageFormat::RG32F:		   return VK_FORMAT_R32G32_SFLOAT;
		case ImageFormat::RGBA:            return VK_FORMAT_R8G8B8A8_UNORM;
		case ImageFormat::RGBA16F:         return VK_FORMAT_R16G16B16A16_SFLOAT;
		case ImageFormat::RGBA32F:         return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ImageFormat::DEPTH32F:        return VK_FORMAT_D32_SFLOAT;
		case ImageFormat::DEPTH24STENCIL8: return VulkanContext::GetDevice()->GetDepthFormat();
		}
		return VK_FORMAT_UNDEFINED;
		HZR_CORE_INFO("[VKUtils] Failed to convert format");
	}

	VkFormat VKUtils::ShaderDataTypeToVkFormat(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return VK_FORMAT_R32_SFLOAT;
		case ShaderDataType::Float2:   return VK_FORMAT_R32G32_SFLOAT;
		case ShaderDataType::Float3:   return VK_FORMAT_R32G32B32_SFLOAT;
		case ShaderDataType::Float4:   return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ShaderDataType::Mat3:     return VK_FORMAT_R32G32B32_SFLOAT;
		case ShaderDataType::Mat4:     return VK_FORMAT_UNDEFINED;
		case ShaderDataType::Int:      return VK_FORMAT_R32_UINT;
		case ShaderDataType::Int2:     return VK_FORMAT_R32G32_UINT;
		case ShaderDataType::Int3:     return VK_FORMAT_R32G32B32_UINT;
		case ShaderDataType::Int4:     return VK_FORMAT_R32G32B32A32_UINT;
		case ShaderDataType::Bool:     return VK_FORMAT_R8_UINT;
		}
		return VK_FORMAT_UNDEFINED;
	}

	ShaderType VKUtils::ShaderTypeFromVulkanStage(VkShaderStageFlagBits type)
	{
		switch (type)
		{
		case VK_SHADER_STAGE_VERTEX_BIT:	return Vertex;
		case VK_SHADER_STAGE_FRAGMENT_BIT:	return Fragment;
		case VK_SHADER_STAGE_COMPUTE_BIT:	return Compute;
		case VK_SHADER_STAGE_GEOMETRY_BIT:	return Geometry;
		}
	}

	VkShaderStageFlags VKUtils::ShaderUsageToVulkanUsage(uint32_t usage)
	{
		VkShaderStageFlags flags = 0;

		if (usage & ShaderType::Vertex)		flags |= VK_SHADER_STAGE_VERTEX_BIT;
		if (usage & ShaderType::Fragment)	flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if (usage & ShaderType::Compute)	flags |= VK_SHADER_STAGE_COMPUTE_BIT;
		if (usage & ShaderType::Geometry)	flags |= VK_SHADER_STAGE_GEOMETRY_BIT;
		return flags;
	}

	bool VKUtils::IsDepth(ImageFormat format)
	{
		if (format == ImageFormat::DEPTH24STENCIL8 || format == ImageFormat::DEPTH32F)
			return true;
		return false;
	}

	VkFormat VKUtils::GetImageFormat(ImageFormat format)
	{
		switch (format)
		{
		case ImageFormat::RED32F:          return VK_FORMAT_R32_SFLOAT;
		case ImageFormat::RG16F:		   return VK_FORMAT_R16G16_SFLOAT;
		case ImageFormat::RG32F:		   return VK_FORMAT_R32G32_SFLOAT;
		case ImageFormat::RGB:			   return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ImageFormat::RGBA:            return VK_FORMAT_R8G8B8A8_UNORM;
		case ImageFormat::RGBA16F:         return VK_FORMAT_R16G16B16A16_SFLOAT;
		case ImageFormat::RGBA32F:         return VK_FORMAT_R32G32B32A32_SFLOAT;
		case ImageFormat::DEPTH32F:        return VK_FORMAT_D32_SFLOAT;
		case ImageFormat::DEPTH24STENCIL8: return VulkanContext::GetDevice()->GetDepthFormat();
		}
		return VK_FORMAT_UNDEFINED;
	}

	VkFilter VKUtils::GetSamplerFilter(const FilterMode& filter)
	{
		switch (filter) {
		case FilterMode::Linear:		return VK_FILTER_LINEAR;
		case FilterMode::LinearMip:		return VK_FILTER_LINEAR;
		case FilterMode::Nearest:		return VK_FILTER_NEAREST;
		case FilterMode::NearestMip:	return VK_FILTER_NEAREST;
		}
	}

	VkSamplerAddressMode VKUtils::GetSamplerWrap(const ImageWrap& wrap)
	{
		switch (wrap) {
		case ImageWrap::ClampBorder:	return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		case ImageWrap::ClampEdge:		return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case ImageWrap::Repeat:			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case ImageWrap::RepeatMirror:	return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		}
	}

	void VKUtils::SetImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout,
		VkImageSubresourceRange subresourceRange, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask)
	{
		VkImageMemoryBarrier memoryBarrier = {};
		memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		memoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		memoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		memoryBarrier.oldLayout = oldImageLayout;
		memoryBarrier.newLayout = newImageLayout;
		memoryBarrier.image = image;
		memoryBarrier.subresourceRange = subresourceRange;

		switch (oldImageLayout) {
		case VK_IMAGE_LAYOUT_UNDEFINED:
			memoryBarrier.srcAccessMask = 0;
			break;
		case VK_IMAGE_LAYOUT_PREINITIALIZED:
			memoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL_KHR:
			memoryBarrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			memoryBarrier.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			memoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;
		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			memoryBarrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		}
		switch (newImageLayout)
		{
		case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
			memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
			memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			break;

		case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
			memoryBarrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
			memoryBarrier.dstAccessMask = memoryBarrier.dstAccessMask | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			break;

		case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
			if (memoryBarrier.srcAccessMask == 0)
			{
				memoryBarrier.srcAccessMask = VK_ACCESS_HOST_WRITE_BIT | VK_ACCESS_TRANSFER_WRITE_BIT;
			}
			memoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			break;
		default:
			break;
		}

		vkCmdPipelineBarrier(cmdbuffer, srcStageMask, dstStageMask, 0, 0, nullptr, 0, nullptr, 1, &memoryBarrier);

	}

	void VKUtils::InsertImageMemoryBarrier(VkCommandBuffer cmdbuffer, VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange)
	{
		VkImageMemoryBarrier imageMemoryBarrier{};
		imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		imageMemoryBarrier.srcAccessMask = srcAccessMask;
		imageMemoryBarrier.dstAccessMask = dstAccessMask;
		imageMemoryBarrier.oldLayout = oldImageLayout;
		imageMemoryBarrier.newLayout = newImageLayout;
		imageMemoryBarrier.image = image;
		imageMemoryBarrier.subresourceRange = subresourceRange;

		vkCmdPipelineBarrier(
			cmdbuffer,
			srcStageMask,
			dstStageMask,
			0,
			0, nullptr,
			0, nullptr,
			1, &imageMemoryBarrier);
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
	VkSurfaceCapabilitiesKHR VKUtils::GetSurfaceCapabilities(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
	{
		VkSurfaceCapabilitiesKHR surfCaps;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfCaps);
		return surfCaps;
	}
}