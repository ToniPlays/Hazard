#pragma once

#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"
#include "Backend/Vulkan/Pipeline/VulkanVertexBuffer.h"
#include "Backend/Vulkan/Pipeline/VulkanIndexBuffer.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanAccelerationStructure : public AccelerationStructure
	{
	public:
		VulkanAccelerationStructure(AccelerationStructureCreateInfo* info);
		~VulkanAccelerationStructure() = default;

		void Invalidate_RT();

	private:
		VkDeviceOrHostAddressConstKHR GetBufferAddress(VkBuffer buffer);

	private:
		AccelerationStructureLevel m_Level;

		Ref<VulkanVertexBuffer> m_VertexBuffer;
		Ref<VulkanIndexBuffer> m_IndexBuffer;


	};
}