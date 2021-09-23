#pragma once

#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

namespace Hazard::Rendering::Vulkan
{
	class VulkanAllocator
	{
	public:
		VulkanAllocator(const char* name);
		~VulkanAllocator();

		VmaAllocation AllocateBuffer(VkBufferCreateInfo* info, VmaMemoryUsage usage, VkBuffer* buffer);
		template<typename T>

		T* MapMemory(VmaAllocation alloc);
		void UnmapMemory(VmaAllocation alloc);

		void CopyBufferToImage(VkBuffer buffer, VkImage image, VkExtent2D extent);
		void TransitionImageLayuout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	private:
		VmaAllocator m_Allocator;
	};

	template<typename T>
	inline T* VulkanAllocator::MapMemory(VmaAllocation alloc)
	{
		static_assert(false);
	}
	template<>
	inline void* VulkanAllocator::MapMemory(VmaAllocation alloc) {
		void* data;
		vmaMapMemory(m_Allocator, alloc, &data);
		return data;
	}
}