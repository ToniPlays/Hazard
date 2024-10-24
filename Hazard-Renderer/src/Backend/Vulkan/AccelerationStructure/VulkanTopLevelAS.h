#pragma once

#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"
#include "Backend/Vulkan/Pipeline/VulkanVertexBuffer.h"
#include "Backend/Vulkan/Pipeline/VulkanIndexBuffer.h"

#include "Helpers.h"

namespace HazardRenderer::Vulkan
{
	class VulkanBottomLevelAS;

	class VulkanTopLevelAS : public TopLevelAS
	{
	public:
		VulkanTopLevelAS(AccelerationStructureCreateInfo* info);
		~VulkanTopLevelAS() = default;

		size_t GetCount() const { return 0; };
		void PushInstances(const glm::mat4& transform, Ref<AccelerationStructure> accelerationStructure) override;

		void Build(const BuildType& type);
		void Build(VkCommandBuffer commandBuffer, const BuildType& type);

		void Invalidate() override;
		void Invalidate_RT();

		AccelerationStructureBufferInfo CreateAccelerationStructureBuffer(VkBufferUsageFlagBits usage, const uint32_t& size);
		VulkanAccelStruct GetVulkanAccelerationStructure() { return m_StructureInfo; }

	private:
		void CreateTopLevel();
		void CreateInstanceBuffer();

	private:
		std::string m_DebugName;
		AccelerationStructureLevel m_Level;

		std::vector<AccelerationStructureInstance> m_Instances;
		std::vector<VkAccelerationStructureGeometryKHR> m_Geometry;

		AccelerationStructureBufferInfo m_StructureBuffer;
		AccelerationStructureBufferInfo m_ScratchBuffer;
		AccelerationStructureBufferInfo m_InstanceBuffer;
		
		VulkanAccelStruct m_StructureInfo;
	};
}