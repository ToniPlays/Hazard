
#include "VulkanTopLevelAS.h"
#include <vulkan/vulkan.h>
#include "../VulkanContext.h"
#include "Backend/Core/Renderer.h"
#include "spdlog/fmt/fmt.h"

#include "VulkanBottomLevelAS.h"
#include <MathCore.h>

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

		pBottomLevel = info->pBottomLevelAS;
		Ref<VulkanTopLevelAS> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});

		//Invalidate();
	}
	void VulkanTopLevelAS::PushInstances(const glm::mat4& transform, Ref<AccelerationStructure> accelerationStructure)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(accelerationStructure->GetLevel() == AccelerationStructureLevel::Bottom, "Acceleration structure must be BLAS");


		Ref<VulkanTopLevelAS> instance = this;
		Ref<VulkanBottomLevelAS> structure = accelerationStructure.As<VulkanBottomLevelAS>();
	}

	void VulkanTopLevelAS::Build(const BuildType& type)
	{
		auto& device = VulkanContext::GetInstance()->GetLogicalDevice();
		VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);
		Build(commandBuffer, type);
		device->FlushCommandBuffer(commandBuffer);
	}

	void VulkanTopLevelAS::Build(VkCommandBuffer commandBuffer, const BuildType& type)
	{
		auto& device = VulkanContext::GetInstance()->GetLogicalDevice();

		glm::mat4 transform = Math::ToTransformMatrix({ 2.0f, 0.0f, 0.0f }, glm::quat({ glm::radians(0.0f), glm::radians(0.0f), 0.0f }));

		VulkanAllocator allocator("VulkanAccelerationStructure");

		VkAccelerationStructureInstanceKHR instanceInfo = {};
		instanceInfo.transform = VkUtils::MatrixToKHR(transform);
		instanceInfo.instanceCustomIndex = 0;
		instanceInfo.mask = 0xFF;
		instanceInfo.instanceShaderBindingTableRecordOffset = 0;
		instanceInfo.flags = VK_GEOMETRY_INSTANCE_FORCE_OPAQUE_BIT_KHR;
		instanceInfo.accelerationStructureReference = ((VulkanBottomLevelAS*)pBottomLevel)->GetVulkanAccelerationStructure().Address;

		std::vector<VkAccelerationStructureInstanceKHR> instances(2);
		instances[0] = instanceInfo;
		transform = Math::ToTransformMatrix({ -2.0f, -0.25f, 0.0f }, glm::quat({ glm::radians(0.0f), glm::radians(25.0f), 0.0f }));
		instances[1] = instanceInfo;
		instances[1].transform = VkUtils::MatrixToKHR(transform);

		uint8_t* dest = allocator.MapMemory<uint8_t>(m_InstanceBuffer.Allocation);
		memcpy(dest, instances.data(), instances.size() * sizeof(VkAccelerationStructureInstanceKHR));
		allocator.UnmapMemory(m_InstanceBuffer.Allocation);

		VkAccelerationStructureBuildGeometryInfoKHR buildInfo = {};
		buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		buildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
		buildInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
		buildInfo.mode = type == BuildType::Build ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
		buildInfo.srcAccelerationStructure = m_StructureInfo.AccelerationStructure;
		buildInfo.geometryCount = 1;
		buildInfo.pGeometries = &m_Geometry;

		uint32_t primitiveCount = 2; // Instance count

		if (type == BuildType::Build)
		{
			VulkanAllocator allocator("VulkanAccelerationStructure");
			if (m_StructureBuffer.Buffer)
			{
				allocator.Free(m_StructureBuffer.Allocation);
				m_StructureBuffer.Buffer = VK_NULL_HANDLE;
			}
			if (m_ScratchBuffer.Buffer)
			{
				allocator.Free(m_ScratchBuffer.Allocation);
				m_ScratchBuffer.Buffer = VK_NULL_HANDLE;
			}

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

			VkAccelerationStructureDeviceAddressInfoKHR addressInfo = {};
			addressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
			addressInfo.accelerationStructure = m_StructureInfo.AccelerationStructure;
			m_StructureInfo.Address = fpGetAccelerationStructureDeviceAddressKHR(device->GetVulkanDevice(), &addressInfo);
		}

		VkAccelerationStructureBuildRangeInfoKHR buildRange = {};
		buildRange.primitiveCount = primitiveCount;
		buildRange.primitiveOffset = 0;
		buildRange.firstVertex = 0;
		buildRange.transformOffset = 0;
		buildInfo.dstAccelerationStructure = m_StructureInfo.AccelerationStructure;
		buildInfo.scratchData.deviceAddress = m_ScratchBuffer.Address;

		std::vector<VkAccelerationStructureBuildRangeInfoKHR*> buildRanges = { &buildRange };

		fpCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &buildInfo, buildRanges.data());
	}

	void VulkanTopLevelAS::Invalidate()
	{
		Ref<VulkanTopLevelAS> instance = this;
		if (m_StructureInfo.AccelerationStructure)
		{
			auto structure = m_StructureInfo.AccelerationStructure;
			Renderer::SubmitResourceFree([structure]() mutable {
				auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();
				fpDestroyAccelerationStructureKHR(device, structure, nullptr);
				});
		}
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

		VulkanAllocator allocator("VulkanAccelerationStructure");

		VkDeviceOrHostAddressConstKHR instanceDataAddress = {};
		instanceDataAddress.deviceAddress = m_InstanceBuffer.Address;

		m_Geometry = {};
		m_Geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		m_Geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
		m_Geometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR; //Todo
		m_Geometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
		m_Geometry.geometry.instances.arrayOfPointers = VK_FALSE;
		m_Geometry.geometry.instances.data = instanceDataAddress;

		Build(BuildType::Build);

		allocator.DestroyBuffer(m_ScratchBuffer.Buffer, m_ScratchBuffer.Allocation);
		m_ScratchBuffer.Buffer = VK_NULL_HANDLE;
	}
	void VulkanTopLevelAS::CreateInstanceBuffer()
	{
		VulkanAllocator allocator("VulkanAccelerationStructure");

		VkBufferCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		instanceInfo.size = sizeof(VkAccelerationStructureInstanceKHR) * 2;
		instanceInfo.usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;

		m_InstanceBuffer.Allocation = allocator.AllocateBuffer(instanceInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_InstanceBuffer.Buffer);
		m_InstanceBuffer.Address = VkUtils::GetBufferAddress(m_InstanceBuffer.Buffer).deviceAddress;
	}
}
