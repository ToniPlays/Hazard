#pragma once

#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"
#include "Backend/Vulkan/Pipeline/VulkanVertexBuffer.h"
#include "Backend/Vulkan/Pipeline/VulkanIndexBuffer.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	struct AccelerationStructureBufferInfo
	{
		VkBuffer Buffer = VK_NULL_HANDLE;
		VmaAllocation Allocation = VK_NULL_HANDLE;
		VkDeviceAddress Address;
	};

	class VulkanAccelerationStructure : public AccelerationStructure
	{
	public:
		VulkanAccelerationStructure(AccelerationStructureCreateInfo* info);
		~VulkanAccelerationStructure() = default;

		void Invalidate_RT();
		AccelerationStructureBufferInfo CreateAccelerationStructureBuffer(VkBufferUsageFlagBits usage, const uint32_t& size);

	private:
		VkDeviceOrHostAddressConstKHR GetBufferAddress(VkBuffer buffer);

	private:
		AccelerationStructureLevel m_Level;
		Ref<VulkanVertexBuffer> m_VertexBuffer;
		Ref<VulkanIndexBuffer> m_IndexBuffer;

		AccelerationStructureBufferInfo m_StructureBuffer;
		AccelerationStructureBufferInfo m_ScratchBuffer;

		VkAccelerationStructureKHR m_AccelerationStructure = VK_NULL_HANDLE;
		VkAccelerationStructureDeviceAddressInfoKHR m_StructureInfo;
	};
}