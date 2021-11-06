#pragma once

#include "Hazard/Core/Core.h"
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

namespace Hazard::Rendering::Vulkan
{
	struct GPUMemoryStats
	{
		uint64_t Used = 0;
		uint64_t Free = 0;
	};

	class VulkanAllocator {
	public:
		VulkanAllocator() = default;
		VulkanAllocator(const std::string& name);
		~VulkanAllocator();

		VmaAllocation AllocateBuffer(VkBufferCreateInfo bufferCreateInfo, VmaMemoryUsage usage, VkBuffer& outBuffer);
		VmaAllocation AllocateImage(VkImageCreateInfo imageCreateInfo, VmaMemoryUsage usage, VkImage& outImage);

		void Free(VmaAllocation allocation);
		void DestroyBuffer(VkBuffer buffer, VmaAllocation allocation);
		void DestroyImage(VkImage image, VmaAllocation allocation);
		
		template<typename T>
		T* MapMemory(VmaAllocation allocation) {
			T* mappedMemory;
			vmaMapMemory(GetAllocator(), allocation, (void**)&mappedMemory);
			return mappedMemory;
		}

		void UnmapMemory(VmaAllocation allocation);

		static void GetStats();
		static GPUMemoryStats GetGPUStats();

		static void Init();
		static void Shutdown();
		static VmaAllocator GetAllocator();
	private:
		std::string m_Tag;
	};
}