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
	struct VulkanAccelStruct
	{
		VkAccelerationStructureKHR AccelerationStructure = VK_NULL_HANDLE;
		uint64_t Address = 0;

	};

	class VulkanAccelerationStructure : public AccelerationStructure
	{
	public:
		VulkanAccelerationStructure(AccelerationStructureCreateInfo* info);
		~VulkanAccelerationStructure() = default;

		void Invalidate_RT();
		AccelerationStructureBufferInfo CreateAccelerationStructureBuffer(VkBufferUsageFlagBits usage, const uint32_t& size);
		VulkanAccelStruct GetVulkanAccelerationStructure() { return m_StructureInfo; }

	private:
		void CreateTopLevel();
		void CreateBottomLevel();

	private:
		std::string m_DebugName;
		AccelerationStructureLevel m_Level;
		Ref<VulkanVertexBuffer> m_VertexBuffer;
		Ref<VulkanIndexBuffer> m_IndexBuffer;

		Ref<VulkanAccelerationStructure> m_BottomLevelAccelStruct;

		AccelerationStructureBufferInfo m_StructureBuffer;
		AccelerationStructureBufferInfo m_ScratchBuffer;
		
		VulkanAccelStruct m_StructureInfo;
	};
}