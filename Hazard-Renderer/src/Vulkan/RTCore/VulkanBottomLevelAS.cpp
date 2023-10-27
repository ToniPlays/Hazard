
#include "VulkanBottomLevelAS.h"

#ifdef HZR_INCLUDE_VULKAN

#include <vulkan/vulkan.h>
#include "../VulkanContext.h"
#include "Core/Renderer.h"
#include "../VKUtils.h"
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
	VulkanBottomLevelAS::VulkanBottomLevelAS(AccelerationStructureCreateInfo* info)
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice();
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


		m_Geometries.clear();
		m_Geometries.resize(info->GeometryCount);

		for (uint32_t i = 0; i < info->GeometryCount; i++)
			m_Geometries[i] = info->pGeometries[i];

		Invalidate();

		Ref<VulkanBottomLevelAS> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	void VulkanBottomLevelAS::PushTransforms(const BufferCopyRegion& copyRegion)
	{
		//TODO: remove this
	}
	void VulkanBottomLevelAS::Invalidate()
	{
        
	}
	void VulkanBottomLevelAS::Build(const BuildType& type)
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice();
		VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);
		Build(commandBuffer, BuildType::Build);
		device->FlushCommandBuffer(commandBuffer);
	}
	void VulkanBottomLevelAS::Build(VkCommandBuffer commandBuffer, const BuildType& type)
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice();

		std::vector<uint32_t> primitiveSizes(m_GeometryData.size());
		std::vector<VkAccelerationStructureGeometryKHR> geometries(m_GeometryData.size());
		std::vector<VkAccelerationStructureBuildRangeInfoKHR> buildRanges(m_GeometryData.size());

		for (uint64_t i = 0; i < geometries.size(); i++)
		{
			uint32_t primitives = m_Geometries[i].IndexBuffer->GetSize() / 3;

			auto& geometry = geometries[i];
			geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
			geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
			geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR; //Todo
			geometry.geometry.triangles = m_GeometryData[i];

			auto& buildRange = buildRanges[i];
			buildRange.primitiveCount = primitives;
			buildRange.primitiveOffset = 0;
			buildRange.firstVertex = 0;
			buildRange.transformOffset = i * sizeof(VkTransformMatrixKHR);

			primitiveSizes[i] = primitives;
		}

		VkAccelerationStructureBuildGeometryInfoKHR buildInfo = {};
		buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
		buildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
		buildInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_TRACE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR;
		buildInfo.mode = type == BuildType::Build ? VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR : VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
		buildInfo.geometryCount = geometries.size();
		buildInfo.pGeometries = geometries.data();

		VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo = {};
		buildSizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
		fpGetAccelerationStructureBuildSizesKHR(device->GetVulkanDevice(), VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR, &buildInfo, primitiveSizes.data(), &buildSizeInfo);

		m_StructureBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR, buildSizeInfo.accelerationStructureSize);
		m_ScratchBuffer = CreateAccelerationStructureBuffer(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, buildSizeInfo.buildScratchSize);

		VkAccelerationStructureCreateInfoKHR structureCreateInfo = {};
		structureCreateInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
		structureCreateInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
		structureCreateInfo.type = buildInfo.type;
		structureCreateInfo.buffer = m_StructureBuffer.Buffer;
		structureCreateInfo.size = buildSizeInfo.accelerationStructureSize;

		fpCreateAccelerationStructureKHR(device->GetVulkanDevice(), &structureCreateInfo, nullptr, &m_StructureInfo.AccelerationStructure);

		if (type == BuildType::Update)
			buildInfo.srcAccelerationStructure = m_StructureInfo.AccelerationStructure;

		buildInfo.dstAccelerationStructure = m_StructureInfo.AccelerationStructure;
		buildInfo.scratchData.deviceAddress = m_ScratchBuffer.Address;

		std::vector<VkAccelerationStructureBuildRangeInfoKHR*> pRanges(buildRanges.size());
		for (uint32_t i = 0; i < pRanges.size(); i++)
			pRanges[i] = &buildRanges[i];

		fpCmdBuildAccelerationStructuresKHR(commandBuffer, 1, &buildInfo, pRanges.data());

		VkAccelerationStructureDeviceAddressInfoKHR addressInfo = {};
		addressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
		addressInfo.accelerationStructure = m_StructureInfo.AccelerationStructure;

		m_StructureInfo.Address = fpGetAccelerationStructureDeviceAddressKHR(device->GetVulkanDevice(), &addressInfo);

		//VulkanAllocator allocator("VulkanAccelerationStructure");
		//allocator.DestroyBuffer(m_ScratchBuffer.Buffer, m_ScratchBuffer.Allocation);
	}
	void VulkanBottomLevelAS::Invalidate_RT()
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice()->GetVulkanDevice();

		CreateTransformBuffer();
		CreateAccelerationStructure();

		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_ACCELERATION_STRUCTURE_KHR, fmt::format("Acceleration structured {0}", m_DebugName), m_StructureInfo.AccelerationStructure);
	}
	AccelerationStructureBufferInfo VulkanBottomLevelAS::CreateAccelerationStructureBuffer(VkBufferUsageFlagBits usage, const uint32_t& size)
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
	void VulkanBottomLevelAS::CreateAccelerationStructure()
	{
		auto device = VulkanContext::GetInstance()->GetLogicalDevice();

		std::vector<VkTransformMatrixKHR> transforms;
		transforms.reserve(m_Geometries.size());
		m_GeometryData.reserve(m_Geometries.size());

		for (auto& geometry : m_Geometries) 
		{
			/*
			Ref<GPUBuffer> vertexBuffer = geometry.VertexBuffer.As<VulkanVertexBuffer>();
			Ref<GPUBuffer> indexBuffer = geometry.IndexBuffer.As<VulkanIndexBuffer>();

			uint32_t stride = geometry.VertexBuffer->GetLayout().GetStride();
			VkAccelerationStructureGeometryTrianglesDataKHR& data = m_GeometryData.emplace_back();

			data = {};
			data.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
			data.vertexFormat = VK_FORMAT_R32G32B32A32_SFLOAT;
			data.vertexData = VkUtils::GetBufferAddress(vertexBuffer->GetVulkanBuffer());
			data.maxVertex = vertexBuffer->GetSize() / stride;
			data.vertexStride = stride;
			data.indexType = VK_INDEX_TYPE_UINT32;
			data.indexData = VkUtils::GetBufferAddress(indexBuffer->GetVulkanBuffer());
			data.transformData.deviceAddress = m_TransformBuffer.Address;

			transforms.emplace_back(VkUtils::MatrixToKHR(geometry.Transform));
			*/
		}

		VulkanAllocator allocator("VulkanAccelerationStructure");

		uint8_t* dest = allocator.MapMemory<uint8_t>(m_TransformBuffer.Allocation);
		memcpy(dest, transforms.data(), transforms.size() * sizeof(VkTransformMatrixKHR));
		allocator.UnmapMemory(m_TransformBuffer.Allocation);

		Build(BuildType::Build);
	}
	void VulkanBottomLevelAS::CreateTransformBuffer()
	{
		VulkanAllocator allocator("VulkanAccelerationStructure");

		VkBufferCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		instanceInfo.size = sizeof(VkTransformMatrixKHR) * m_Geometries.size();
		instanceInfo.usage = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT | VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR;

		m_TransformBuffer.Allocation = allocator.AllocateBuffer(instanceInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_TransformBuffer.Buffer);
		m_TransformBuffer.Address = VkUtils::GetBufferAddress(m_TransformBuffer.Buffer).deviceAddress;
	}
}
#endif
