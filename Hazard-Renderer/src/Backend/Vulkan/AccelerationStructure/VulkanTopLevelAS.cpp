
#include "VulkanTopLevelAS.h"
#include <vulkan/vulkan.h>
#include "../VulkanContext.h"
#include "Backend/Core/Renderer.h"
#include "spdlog/fmt/fmt.h"

#include "VulkanBottomLevelAS.h"

static PFN_vkCmdBuildAccelerationStructuresKHR fpCmdBuildAccelerationStructuresKHR;
static PFN_vkBuildAccelerationStructuresKHR fpBuildAccelerationStructuresKHR;
static PFN_vkCreateAccelerationStructureKHR fpCreateAccelerationStructureKHR;
static PFN_vkDestroyAccelerationStructureKHR fpDestroyAccelerationStructureKHR;
static PFN_vkGetAccelerationStructureBuildSizesKHR fpGetAccelerationStructureBuildSizesKHR;
static PFN_vkGetAccelerationStructureDeviceAddressKHR fpGetAccelerationStructureDeviceAddressKHR;
static PFN_vkCmdTraceRaysKHR fpCmdTraceRaysKHR;
static PFN_vkGetRayTracingShaderGroupHandlesKHR fpGetRayTracingShaderGroupHandlesKHR;
static PFN_vkCreateRayTracingPipelinesKHR fpCreateRayTracingPipelinesKHR;

namespace HazardRenderer::Vulkan
{
	VulkanTopLevelAS::VulkanTopLevelAS(AccelerationStructureCreateInfo* info)
	{
		auto& device = VulkanContext::GetInstance()->GetLogicalDevice();
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CmdBuildAccelerationStructuresKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), BuildAccelerationStructuresKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CreateAccelerationStructureKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), DestroyAccelerationStructureKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), GetAccelerationStructureBuildSizesKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), GetAccelerationStructureDeviceAddressKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CmdTraceRaysKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), GetRayTracingShaderGroupHandlesKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CreateRayTracingPipelinesKHR);

		m_DebugName = info->DebugName;
		m_Level = info->Level;
		pBottomLevel = (VulkanBottomLevelAS*)info->pBottomLevel;

		Ref<VulkanTopLevelAS> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	void VulkanTopLevelAS::PushInstances(const glm::mat4& transform, Ref<AccelerationStructure> accelerationStructure)
	{
		return;
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(accelerationStructure->GetLevel() == AccelerationStructureLevel::Bottom, "Acceleration structure must be BLAS");

		VkTransformMatrixKHR transformMatrix =
		{
				transform[0][0], transform[0][1], transform[0][2], transform[0][3],
				transform[1][0], transform[1][1], transform[1][2], transform[1][3],
				transform[2][0], transform[2][1], transform[2][2], transform[2][3]
		};
		Ref<VulkanTopLevelAS> instance = this;
		Ref<VulkanBottomLevelAS> structure = accelerationStructure.As<VulkanBottomLevelAS>();

		Renderer::Submit([instance, matrix = transformMatrix, structure]() mutable {

			VulkanAllocator allocator("VulkanAccelerationStructure");

			VkAccelerationStructureInstanceKHR instanceInfo = {};
			instanceInfo.transform = matrix;
			instanceInfo.instanceCustomIndex = 0;
			instanceInfo.mask = 0xFF;
			instanceInfo.instanceShaderBindingTableRecordOffset = 0;
			instanceInfo.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
			instanceInfo.accelerationStructureReference = structure->GetVulkanAccelerationStructure().Address;

			uint8_t* dest = allocator.MapMemory<uint8_t>(instance->m_InstanceBuffer.Allocation);
			memcpy(dest, &instanceInfo, sizeof(VkAccelerationStructureInstanceKHR));
			allocator.UnmapMemory(instance->m_InstanceBuffer.Allocation);
			});
	}

	void VulkanTopLevelAS::Invalidate_RT()
	{
		HZR_PROFILE_FUNCTION();
		auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		CreateInstanceBuffer();
		CreateTopLevel();

		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, fmt::format("Acceleration structure {0}", m_DebugName), m_StructureInfo.AccelerationStructure);
	}
	AccelerationStructureBufferInfo VulkanTopLevelAS::CreateAccelerationStructureBuffer(VkBufferUsageFlagBits usage, const uint32_t& size)
	{
		HZR_PROFILE_FUNCTION();
		VulkanAllocator allocator("VulkanAccelerationStructure");

		AccelerationStructureBufferInfo result;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
		result.Allocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_GPU_ONLY, result.Buffer);
		result.Address = VkUtils::GetBufferAddress(result.Buffer).deviceAddress;

		return result;
	}
	void VulkanTopLevelAS::CreateTopLevel()
	{
		auto& device = VulkanContext::GetInstance()->GetLogicalDevice();

		VkTransformMatrixKHR transformMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
		};

		VulkanAllocator allocator("VulkanAccelerationStructure");

		VkAccelerationStructureInstanceKHR instance = {};
		instance.transform = transformMatrix;
		instance.instanceCustomIndex = 0;
		instance.mask = 0xFF;
		instance.instanceShaderBindingTableRecordOffset = 0;
		instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
		instance.accelerationStructureReference = pBottomLevel->GetVulkanAccelerationStructure().Address;

		VkBufferCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		instanceInfo.size = sizeof(VkAccelerationStructureInstanceKHR);
		instanceInfo.usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;

		m_InstanceBuffer.Allocation = allocator.AllocateBuffer(instanceInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_InstanceBuffer.Buffer);
		uint8_t* dest = allocator.MapMemory<uint8_t>(m_InstanceBuffer.Allocation);
		memcpy(dest, &instance, sizeof(VkAccelerationStructureInstanceKHR));
		allocator.UnmapMemory(m_InstanceBuffer.Allocation);

		m_InstanceBuffer.Address = VkUtils::GetBufferAddress(m_InstanceBuffer.Buffer).deviceAddress;

		VkDeviceOrHostAddressConstKHR instanceDataAddress = {};
		instanceDataAddress.deviceAddress = m_InstanceBuffer.Address;

		VkAccelerationStructureGeometryKHR geometry = {};
		geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
		geometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR; //Todo
		geometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
		geometry.geometry.instances.arrayOfPointers = VK_FALSE;
		geometry.geometry.instances.data = instanceDataAddress;

		VkAccelerationStructureBuildGeometryInfoKHR buildInfo = {};
		buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		buildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
		buildInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
		buildInfo.geometryCount = 1;
		buildInfo.pGeometries = &geometry;

		uint32_t primitiveCount = 1;

		VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo = {};
		buildSizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
		fpGetAccelerationStructureBuildSizesKHR(device->GetVulkanDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &buildInfo, &primitiveCount, &buildSizeInfo);

		m_StructureBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, buildSizeInfo.accelerationStructureSize);

		VkAccelerationStructureCreateInfoKHR structureCreateInfo = {};
		structureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
		structureCreateInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
		structureCreateInfo.type = buildInfo.type;
		structureCreateInfo.buffer = m_StructureBuffer.Buffer;
		structureCreateInfo.size = buildSizeInfo.accelerationStructureSize;

		fpCreateAccelerationStructureKHR(device->GetVulkanDevice(), &structureCreateInfo, nullptr, &m_StructureInfo.AccelerationStructure);
		m_ScratchBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, buildSizeInfo.buildScratchSize);

		buildInfo.dstAccelerationStructure = m_StructureInfo.AccelerationStructure;
		buildInfo.scratchData.deviceAddress = m_ScratchBuffer.Address;

		VkAccelerationStructureBuildRangeInfoKHR buildRange = {};
		buildRange.primitiveCount = primitiveCount;
		buildRange.primitiveOffset = 0;
		buildRange.firstVertex = 0;
		buildRange.transformOffset = 0;

		std::vector<VkAccelerationStructureBuildRangeInfoKHR*> buildRanges = { &buildRange };

		VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);
		fpCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &buildInfo, buildRanges.data());
		device->FlushCommandBuffer(commandBuffer);

		VkAccelerationStructureDeviceAddressInfoKHR addressInfo = {};
		addressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
		addressInfo.accelerationStructure = m_StructureInfo.AccelerationStructure;

		m_StructureInfo.Address = fpGetAccelerationStructureDeviceAddressKHR(device->GetVulkanDevice(), &addressInfo);

		allocator.DestroyBuffer(m_ScratchBuffer.Buffer, m_ScratchBuffer.Allocation);
	}
	void VulkanTopLevelAS::CreateInstanceBuffer()
	{
		VulkanAllocator allocator("VulkanAccelerationStructure");

		VkBufferCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		instanceInfo.size = sizeof(VkTransformMatrixKHR);
		instanceInfo.usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;

		m_InstanceBuffer.Allocation = allocator.AllocateBuffer(instanceInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_InstanceBuffer.Buffer);
		m_InstanceBuffer.Address = VkUtils::GetBufferAddress(m_InstanceBuffer.Buffer).deviceAddress;
	}
}
