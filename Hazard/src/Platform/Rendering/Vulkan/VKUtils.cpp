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

	VkFormat VKUtils::FindSupportedFormat(VkPhysicalDevice device, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(device, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
				return format;
			}
			else if (
				tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
				return format;
			}
		}
		HZR_THROW("Failed to find suported VkFormat!");
	}

	VkAttachmentDescription VKUtils::CreateAttachmentDescription(FrameBufferAttachment& attachment)
	{
		VkAttachmentDescription description;
		description.flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
		description.format = GetFormat(attachment.format);
		description.samples = VK_SAMPLE_COUNT_1_BIT;
		description.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		description.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		description.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		description.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

		description.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		description.finalLayout = !attachment.IsDepth() ? VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL : VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		return description;
	}

	std::vector<VkAttachmentReference> VKUtils::CreateColorRefs(std::vector<FrameBufferAttachment>& attachments)
	{
		uint32_t count = 0;
		std::vector<VkAttachmentReference> refs;

		for (auto& attachment : attachments) {
			if (attachment.IsDepth()) continue;
			refs.push_back({ count, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL });
		}
		return refs;
	}

	std::vector<VkAttachmentReference> VKUtils::CreateDepthRefs(std::vector<FrameBufferAttachment>& attachments, uint32_t startIndex)
	{
		uint32_t count = startIndex;
		std::vector<VkAttachmentReference> refs;

		for (auto& attachment : attachments) {
			if (!attachment.IsDepth()) continue;
			refs.push_back({ count, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL });
		}
		return refs;
	}

	VkFormat VKUtils::GetFormat(FrameBufferTextureFormat format)
	{
		switch (format)
		{
		case Hazard::Rendering::FrameBufferTextureFormat::RGBA8:			return VK_FORMAT_R8G8B8A8_SRGB;
		case Hazard::Rendering::FrameBufferTextureFormat::RGBA16F:			return VK_FORMAT_R16G16B16A16_SFLOAT;
		case Hazard::Rendering::FrameBufferTextureFormat::RGBA32F:			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case Hazard::Rendering::FrameBufferTextureFormat::RG32F:			return VK_FORMAT_R32G32_SFLOAT;
			/*case Hazard::Rendering::FrameBufferTextureFormat::DEPTH32_STENCIL8:
		case Hazard::Rendering::FrameBufferTextureFormat::DEPTH32F: {
			auto device = VulkanContext::GetDevice();
			return device->GetDepthFormat();
		}*/
		}
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

	VkShaderStageFlagBits VKUtils::ShaderTypeToVulkanStage(ShaderType type)
	{
		switch (type)
		{
		case Vertex:	return VK_SHADER_STAGE_VERTEX_BIT;
		case Fragment:	return VK_SHADER_STAGE_FRAGMENT_BIT;
		case Compute:	return VK_SHADER_STAGE_COMPUTE_BIT;
		case Geometry:	return VK_SHADER_STAGE_GEOMETRY_BIT;
		}
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
}