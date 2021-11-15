#pragma once

#include <hzrpch.h>
#include "VulkanAllocator.h"
#include "../VulkanContext.h"

#include "Hazard/Utils/StringUtil.h"
#include "../VKUtils.h"

namespace Hazard::Rendering::Vulkan {

	struct AllocatorData {
		VmaAllocator Allocator;
		size_t TotalAllocated;
	};
	static AllocatorData* s_Data = nullptr;

	VulkanAllocator::VulkanAllocator(const std::string& name) : m_Tag(name) {}

	VmaAllocation VulkanAllocator::AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer& outBuffer)
	{
		VmaAllocationCreateInfo createInfo = {};
		createInfo.usage = usage;

		VmaAllocation alloc;
		VK_CHECK_RESULT(vmaCreateBuffer(s_Data->Allocator, &bufferCreateInfo, &createInfo, &outBuffer, &alloc, nullptr));

		VmaAllocationInfo info = {};
		vmaGetAllocationInfo(s_Data->Allocator, alloc, &info);

		HZR_CORE_TRACE("VulkanAllocator ({0}) allocated {1}", m_Tag, StringUtil::BytesToString(info.size));
		s_Data->TotalAllocated += info.size;

		return alloc;
	}
	VmaAllocation VulkanAllocator::AllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage& outImage)
	{
		VmaAllocationCreateInfo createInfo = {};
		createInfo.usage = usage;

		VmaAllocation alloc;
		VK_CHECK_RESULT(vmaCreateImage(s_Data->Allocator, &imageCreateInfo, &createInfo, &outImage, &alloc, nullptr));

		VmaAllocationInfo info = {};
		vmaGetAllocationInfo(s_Data->Allocator, alloc, &info);

		HZR_CORE_TRACE("VulkanAllocator ({0}) allocated {1}", m_Tag, StringUtil::BytesToString(info.size));
		s_Data->TotalAllocated += info.size;

		return alloc;
	}
	void VulkanAllocator::Free(VmaAllocation allocation)
	{
		vmaFreeMemory(s_Data->Allocator, allocation);
	}
	void VulkanAllocator::DestroyBuffer(VkBuffer buffer, VmaAllocation allocation)
	{
		VmaAllocationInfo info = {};
		vmaGetAllocationInfo(s_Data->Allocator, allocation, &info);
		s_Data->TotalAllocated -= info.size;
		HZR_CORE_INFO("Total allocated {0}", StringUtil::BytesToString(s_Data->TotalAllocated));

		vmaDestroyBuffer(s_Data->Allocator, buffer, allocation);
	}
	void VulkanAllocator::DestroyImage(VkImage image, VmaAllocation allocation)
	{
		VmaAllocationInfo info = {};
		vmaGetAllocationInfo(s_Data->Allocator, allocation, &info);
		s_Data->TotalAllocated -= info.size;
		HZR_CORE_INFO("Total allocated {0}", StringUtil::BytesToString(s_Data->TotalAllocated));
		vmaDestroyImage(s_Data->Allocator, image, allocation);
	}
	void VulkanAllocator::UnmapMemory(VmaAllocation allocation)
	{
		vmaUnmapMemory(s_Data->Allocator, allocation);
	}
	void VulkanAllocator::GetStats()
	{

	}
	GPUMemoryStats VulkanAllocator::GetGPUStats()
	{
		return { 0, 0 };
	}
	void VulkanAllocator::Init()
	{
		s_Data = new AllocatorData();

		auto device = VulkanContext::GetDevice();

		VmaAllocatorCreateInfo createInfo = {};
		createInfo.vulkanApiVersion = VK_API_VERSION_1_2;
		createInfo.physicalDevice = device->GetPhysicalDevice();
		createInfo.device = device->GetDevice();
		createInfo.instance = VulkanContext::GetInstance();

		VK_CHECK_RESULT(vmaCreateAllocator(&createInfo, &s_Data->Allocator));
	}
	void VulkanAllocator::Shutdown()
	{
		vmaDestroyAllocator(s_Data->Allocator);

		HZR_CORE_ASSERT(s_Data->TotalAllocated == 0, "Failed to deallocate all");

		delete s_Data;
		s_Data = nullptr;
	}
	VmaAllocator VulkanAllocator::GetAllocator()
	{
		return s_Data->Allocator;
	}
}
