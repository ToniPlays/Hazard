

#include "VulkanAccelerationStructure.h"
#include <vulkan/vulkan.h>
#include "../VulkanContext.h"
#include "Backend/Core/Renderer.h"

static PFN_vkGetBufferDeviceAddressKHR fpGetBufferDeviceAddressKHR;
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
	VulkanAccelerationStructure::VulkanAccelerationStructure(AccelerationStructureCreateInfo* info)
	{
		auto& device = VulkanContext::GetInstance()->GetLogicalDevice();

		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), GetBufferDeviceAddressKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CmdBuildAccelerationStructuresKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), BuildAccelerationStructuresKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CreateAccelerationStructureKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), DestroyAccelerationStructureKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), GetAccelerationStructureBuildSizesKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), GetAccelerationStructureDeviceAddressKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CmdTraceRaysKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), GetRayTracingShaderGroupHandlesKHR);
		GET_DEVICE_PROC_ADDR(device->GetVulkanDevice(), CreateRayTracingPipelinesKHR);

		m_Level = info->Level;

		m_VertexBuffer = info->VertexBuffer.As<VulkanVertexBuffer>();
		m_IndexBuffer = info->IndexBuffer.As<VulkanIndexBuffer>();

		Ref<VulkanAccelerationStructure> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});


	}
	void VulkanAccelerationStructure::Invalidate_RT()
	{
		auto& device = VulkanContext::GetInstance()->GetLogicalDevice();

		VkAccelerationStructureBuildGeometryInfoKHR buildInfo = {};
		buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		buildInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
		buildInfo.type = m_Level == AccelerationStructureLevel::Top ? VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR : VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;


		VkAccelerationStructureCreateInfoKHR structureCreateInfo = {};
		structureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
		structureCreateInfo.type = buildInfo.type;

		VkAccelerationStructureBuildRangeInfoKHR buildRange = {};
		buildRange.primitiveOffset = 0;
		buildRange.firstVertex = 0;
		buildRange.transformOffset = 0;

		std::vector<VkAccelerationStructureBuildRangeInfoKHR*> buildRanges = { &buildRange };




		if (m_Level == AccelerationStructureLevel::Top)
		{
			VkTransformMatrixKHR transformMatrix = {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
			};

			VkAccelerationStructureInstanceKHR instance = {};
			instance.transform = transformMatrix;
			instance.instanceCustomIndex = 0;
			instance.mask = 0xFF;
			instance.instanceShaderBindingTableRecordOffset = 0;
			instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_CULL_DISABLE_BIT_NV;
			instance.accelerationStructureReference = 0; //TODO

			VkDeviceOrHostAddressConstKHR dataAddress = {};
			dataAddress.deviceAddress = 0; //TODO

			VkAccelerationStructureGeometryKHR geometry = {};
			geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
			geometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
			geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
			geometry.geometry.instances.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
			geometry.geometry.instances.arrayOfPointers = VK_FALSE;
			geometry.geometry.instances.data = dataAddress; // TODO

			//Size info
			buildInfo.geometryCount = 1;
			buildInfo.pGeometries = &geometry;

			uint32_t primitiveCount = 1;

			VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo = {};
			buildSizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
			fpGetAccelerationStructureBuildSizesKHR(device->GetVulkanDevice(),
				VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
				&buildInfo, &primitiveCount, &buildSizeInfo);


			m_StructureBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, buildSizeInfo.accelerationStructureSize);
			structureCreateInfo.size = buildSizeInfo.accelerationStructureSize;

			structureCreateInfo.buffer = m_StructureBuffer.Buffer;
			fpCreateAccelerationStructureKHR(device->GetVulkanDevice(), &structureCreateInfo, nullptr, &m_AccelerationStructure);

			m_ScratchBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, buildSizeInfo.buildScratchSize);

			VkAccelerationStructureBuildGeometryInfoKHR geometryBuildInfo = {};
			geometryBuildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
			geometryBuildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
			geometryBuildInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR;
			geometryBuildInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
			geometryBuildInfo.dstAccelerationStructure = m_AccelerationStructure;
			geometryBuildInfo.geometryCount = 1;
			geometryBuildInfo.pGeometries = &geometry;
			geometryBuildInfo.scratchData.deviceAddress = m_ScratchBuffer.Address;

			buildRange.primitiveCount = primitiveCount;
		}
		else if (m_Level == AccelerationStructureLevel::Bottom)
		{
			uint32_t stride = m_VertexBuffer->GetLayout().GetStride();

			VkAccelerationStructureGeometryTrianglesDataKHR data = {};
			data.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
			data.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
			data.vertexData = GetBufferAddress(m_VertexBuffer->GetVulkanBuffer());
			data.maxVertex = m_VertexBuffer->GetSize() / stride;
			data.vertexStride = stride;
			data.indexType = VK_INDEX_TYPE_UINT32;
			data.indexData = GetBufferAddress(m_IndexBuffer->GetVulkanBuffer());
			data.transformData.deviceAddress = 0;
			data.transformData.hostAddress = nullptr;

			VkAccelerationStructureGeometryKHR geometry = {};
			geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
			geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;	//TODO
			geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR; //Todo
			geometry.geometry.triangles = data;

			buildInfo.geometryCount = 1;
			buildInfo.pGeometries = &geometry;

			const uint32_t numTriangles = m_IndexBuffer->GetCount() / 3;

			VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo = {};
			buildSizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
			fpGetAccelerationStructureBuildSizesKHR(device->GetVulkanDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &buildInfo, &numTriangles, &buildSizeInfo);


			m_StructureBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, buildSizeInfo.accelerationStructureSize);
			structureCreateInfo.buffer = m_StructureBuffer.Buffer;

			fpCreateAccelerationStructureKHR(device->GetVulkanDevice(), &structureCreateInfo, nullptr, &m_AccelerationStructure);
			m_ScratchBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, buildSizeInfo.buildScratchSize);

			buildInfo.scratchData.deviceAddress = m_ScratchBuffer.Address;
			buildInfo.dstAccelerationStructure = m_AccelerationStructure;
			buildRange.primitiveCount = numTriangles;
		}

		VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);
		fpCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &buildInfo, buildRanges.data());
		device->FlushCommandBuffer(commandBuffer);

		VulkanAllocator allocator("VulkanAccelerationStructure");
		allocator.DestroyBuffer(m_ScratchBuffer.Buffer, m_ScratchBuffer.Allocation);
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
		result.Address = GetBufferAddress(result.Buffer).deviceAddress;

		return result;
	}
	VkDeviceOrHostAddressConstKHR VulkanAccelerationStructure::GetBufferAddress(VkBuffer buffer)
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		VkBufferDeviceAddressInfo bufferDeviceAddressInfo = {};
		bufferDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		bufferDeviceAddressInfo.buffer = buffer;

		VkDeviceOrHostAddressConstKHR address = {};
		address.deviceAddress = fpGetBufferDeviceAddressKHR(device, &bufferDeviceAddressInfo);

		return address;
	}
}
