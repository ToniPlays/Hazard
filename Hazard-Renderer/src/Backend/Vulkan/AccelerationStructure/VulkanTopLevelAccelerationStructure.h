#pragma once

#include "Ref.h"

#include "Backend/Core/AccelerationStructure/TopLevelAccelerationStructure.h"

namespace HazardRenderer::Vulkan
{
	class VulkanTopLevelAccelerationStructure : public TopLevelAccelerationStructure
	{
	public:
		VulkanTopLevelAccelerationStructure(TopLevelAccelerationStructureCreateInfo* info);
		~VulkanTopLevelAccelerationStructure() = default;
	};
}