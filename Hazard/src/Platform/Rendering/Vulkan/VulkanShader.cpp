#pragma once

#include <hzrpch.h>
#include "VulkanShader.h"
#include "Hazard/File/File.h"

namespace Hazard::Rendering::Vulkan
{
	VulkanShader::VulkanShader(const std::string& file) : m_Path(file)
	{

	}
	VulkanShader::~VulkanShader()
	{

	}
	void VulkanShader::Reload()
	{
		auto shaderCode = File::ReadBinaryFileUint32(m_Path);

		VkShaderModuleCreateInfo moduleInfo = {};
		moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.codeSize = shaderCode.size();
		moduleInfo.pCode = shaderCode.data();
	}
	void VulkanShader::Bind()
	{

	}
}
