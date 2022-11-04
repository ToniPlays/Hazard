#pragma once

#include "Backend/Core/AccelerationStructure/ShaderBindingTable.h"

namespace HazardRenderer::Vulkan
{
	class VulkanShaderBindingTable : public ShaderBindingTable
	{
	public:
		VulkanShaderBindingTable(ShaderBindingTableCreateInfo* createInfo);
		~VulkanShaderBindingTable() = default;
	};
}