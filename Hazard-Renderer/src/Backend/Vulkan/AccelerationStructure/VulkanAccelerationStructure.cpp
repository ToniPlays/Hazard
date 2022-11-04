

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
		if (m_Level == AccelerationStructureLevel::Top)
		{

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



		}
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
