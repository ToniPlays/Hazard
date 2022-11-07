#pragma once

#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"
#include "Backend/Vulkan/Pipeline/VulkanVertexBuffer.h"
#include "Backend/Vulkan/Pipeline/VulkanIndexBuffer.h"

#include "Helpers.h"

namespace HazardRenderer::Vulkan
{

	class VulkanBottomLevelAS : public BottomLevelAS
	{
	public:
		VulkanBottomLevelAS(AccelerationStructureCreateInfo* info);
		~VulkanBottomLevelAS() = default;

		size_t GetCount() const { return 0; };
		void PushTransforms(const BufferCopyRegion& copyRegion) override;

		void Invalidate() override;
		void Build(const BuildType& type);
		//Vulkan specific
		void Build(VkCommandBuffer commandBuffer, const BuildType& type);
		void Invalidate_RT();

		AccelerationStructureBufferInfo CreateAccelerationStructureBuffer(VkBufferUsageFlagBits usage, const uint32_t& size);
		VulkanAccelStruct GetVulkanAccelerationStructure() { return m_StructureInfo; }

	private:
		void CreateAccelerationStructure();
		void CreateTransformBuffer();

	private:
		std::string m_DebugName;
		AccelerationStructureLevel m_Level;
		std::vector<AccelerationStructureGeometry> m_Geometries;
		std::vector<VkAccelerationStructureGeometryTrianglesDataKHR> m_GeometryData = {};

		AccelerationStructureBufferInfo m_StructureBuffer;
		AccelerationStructureBufferInfo m_ScratchBuffer;
		AccelerationStructureBufferInfo m_TransformBuffer;

		VulkanAccelStruct m_StructureInfo;
	};
}