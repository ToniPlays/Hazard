#pragma once

#ifdef HZR_PLATFORM_WINDOWS

#include "Backend/Core/AccelerationStructure/ShaderBindingTable.h"
#include "Backend/Vulkan/Pipeline/VulkanPipeline.h"
#include "vulkan/vulkan.h"
#include "vk_mem_alloc.h"

namespace HazardRenderer::Vulkan
{
	struct BindingTableBuffer
	{
		VkBuffer Buffer;
		VmaAllocation Allocation;
		VkStridedDeviceAddressRegionKHR StridedAddress;
	};

	class VulkanShaderBindingTable : public ShaderBindingTable
	{
	public:
		VulkanShaderBindingTable(ShaderBindingTableCreateInfo* createInfo);
		~VulkanShaderBindingTable() = default;

		VkStridedDeviceAddressRegionKHR GetRaygenTableAddress() { return m_BindingTableBuffers[ShaderStage::Raygen].StridedAddress; };
		VkStridedDeviceAddressRegionKHR GetMissTableAddress() { return m_BindingTableBuffers[ShaderStage::Miss].StridedAddress; };
		VkStridedDeviceAddressRegionKHR GetClosestHitTableAddress() { return m_BindingTableBuffers[ShaderStage::ClosestHit].StridedAddress; };
		VkStridedDeviceAddressRegionKHR GetCallableTableAddress() { return VkStridedDeviceAddressRegionKHR(); };


		void Invalidate_RT();

	private:
		void CreateBindingTableBuffers();

	private:
		Ref<VulkanPipeline> m_Pipeline;

		std::unordered_map<ShaderStage, BindingTableBuffer> m_BindingTableBuffers;

	};
}
#endif
