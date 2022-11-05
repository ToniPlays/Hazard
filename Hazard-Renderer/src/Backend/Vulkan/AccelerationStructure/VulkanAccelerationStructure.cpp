

#include "VulkanAccelerationStructure.h"
#include <vulkan/vulkan.h>
#include "../VulkanContext.h"
#include "Backend/Core/Renderer.h"
#include "spdlog/fmt/fmt.h"

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
	static AccelerationStructureBufferInfo s_TransformBuffer;
	static AccelerationStructureBufferInfo s_InstanceBuffers;


	VulkanAccelerationStructure::VulkanAccelerationStructure(AccelerationStructureCreateInfo* info)
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
		m_VertexBuffer = info->VertexBuffer.As<VulkanVertexBuffer>();
		m_IndexBuffer = info->IndexBuffer.As<VulkanIndexBuffer>();

		m_BottomLevelAccelStruct = info->pAccelerationStructure.As<VulkanAccelerationStructure>();

		Ref<VulkanAccelerationStructure> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});


	}
	void VulkanAccelerationStructure::Invalidate_RT()
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		if (m_Level == AccelerationStructureLevel::Top)
			CreateTopLevel();
		else
			CreateBottomLevel();

		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, fmt::format("Acceleration structured {0}", m_DebugName), m_StructureInfo.AccelerationStructure);
	}
	AccelerationStructureBufferInfo VulkanAccelerationStructure::CreateAccelerationStructureBuffer(VkBufferUsageFlagBits usage, const uint32_t& size)
	{
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
	void VulkanAccelerationStructure::CreateTopLevel()
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
		instance.accelerationStructureReference = m_BottomLevelAccelStruct->GetVulkanAccelerationStructure().Address;

		VkBufferCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		instanceInfo.size = sizeof(VkAccelerationStructureInstanceKHR);
		instanceInfo.usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;

		s_InstanceBuffers.Allocation = allocator.AllocateBuffer(instanceInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, s_InstanceBuffers.Buffer);
		uint8_t* dest = allocator.MapMemory<uint8_t>(s_InstanceBuffers.Allocation);
		memcpy(dest, &instance, sizeof(VkAccelerationStructureInstanceKHR));
		allocator.UnmapMemory(s_InstanceBuffers.Allocation);

		s_InstanceBuffers.Address = VkUtils::GetBufferAddress(s_InstanceBuffers.Buffer).deviceAddress;

		VkDeviceOrHostAddressConstKHR instanceDataAddress = {};
		instanceDataAddress.deviceAddress = s_InstanceBuffers.Address;

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
	void VulkanAccelerationStructure::CreateBottomLevel()
	{
		auto& device = VulkanContext::GetInstance()->GetLogicalDevice();

		VkTransformMatrixKHR transformMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
		};

		VulkanAllocator allocator("VulkanAccelerationStructure");

		VkBufferCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		instanceInfo.size = sizeof(VkTransformMatrixKHR);
		instanceInfo.usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;

		s_TransformBuffer.Allocation = allocator.AllocateBuffer(instanceInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, s_TransformBuffer.Buffer);
		uint8_t* dest = allocator.MapMemory<uint8_t>(s_TransformBuffer.Allocation);
		memcpy(dest, &transformMatrix, sizeof(VkTransformMatrixKHR));
		allocator.UnmapMemory(s_TransformBuffer.Allocation);
		
		s_TransformBuffer.Address = VkUtils::GetBufferAddress(s_TransformBuffer.Buffer).deviceAddress;

		uint32_t stride = m_VertexBuffer->GetLayout().GetStride();

		VkAccelerationStructureGeometryTrianglesDataKHR data = {};
		data.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
		data.vertexFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
		data.vertexData = VkUtils::GetBufferAddress(m_VertexBuffer->GetVulkanBuffer());
		data.maxVertex = m_VertexBuffer->GetSize() / stride;
		data.vertexStride = stride;
		data.indexType = VK_INDEX_TYPE_UINT32;
		data.indexData = VkUtils::GetBufferAddress(m_IndexBuffer->GetVulkanBuffer());
		data.transformData.deviceAddress = 0;
		data.transformData.hostAddress = nullptr;

		VkAccelerationStructureGeometryKHR geometry = {};
		geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
		geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
		geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR; //Todo
		geometry.geometry.triangles = data;

		VkAccelerationStructureBuildGeometryInfoKHR buildInfo = {};
		buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		buildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
		buildInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
		buildInfo.geometryCount = 1;
		buildInfo.pGeometries = &geometry;

		const uint32_t numTriangles = m_IndexBuffer->GetCount() / 3;

		VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo = {};
		buildSizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
		fpGetAccelerationStructureBuildSizesKHR(device->GetVulkanDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &buildInfo, &numTriangles, &buildSizeInfo);

		m_StructureBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, buildSizeInfo.accelerationStructureSize);
		m_ScratchBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, buildSizeInfo.buildScratchSize);

		VkAccelerationStructureCreateInfoKHR structureCreateInfo = {};
		structureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
		structureCreateInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
		structureCreateInfo.type = buildInfo.type;
		structureCreateInfo.buffer = m_StructureBuffer.Buffer;
		structureCreateInfo.size = buildSizeInfo.accelerationStructureSize;

		fpCreateAccelerationStructureKHR(device->GetVulkanDevice(), &structureCreateInfo, nullptr, &m_StructureInfo.AccelerationStructure);

		buildInfo.dstAccelerationStructure = m_StructureInfo.AccelerationStructure;
		buildInfo.scratchData.deviceAddress = m_ScratchBuffer.Address;

		VkAccelerationStructureBuildRangeInfoKHR buildRange = {};
		buildRange.primitiveCount = numTriangles;
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
}
