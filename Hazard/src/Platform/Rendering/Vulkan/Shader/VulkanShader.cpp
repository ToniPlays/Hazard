#pragma once

#include <hzrpch.h>
#include "VulkanShader.h"

namespace Hazard::Rendering::Vulkan {

	VulkanShader::VulkanShader(const ShaderCreateInfo& info)
	{
		m_Info.shaderName = info.shaderName;
	}
	VulkanShader::~VulkanShader()
	{

	}
	void VulkanShader::Bind() const
	{

	}
	void VulkanShader::Unbind() const
	{

	}
	void VulkanShader::SetUniformInt(const std::string& name, int value)
	{

	}
	void VulkanShader::SetUniformIntArray(const std::string& name, int* value, uint32_t size)
	{

	}
	void VulkanShader::SetUniformFloat(const std::string& name, float value)
	{

	}
	void VulkanShader::SetUniformVec2(const std::string& name, glm::vec2 value)
	{

	}
	void VulkanShader::SetUniformVec3(const std::string& name, glm::vec3 value)
	{

	}
	void VulkanShader::SetUniformMat4(const std::string& name, glm::mat4 value)
	{

	}
	void VulkanShader::SetUniformColor(const std::string& name, Color color)
	{

	}
	void VulkanShader::SetUniformBool(const std::string& name, bool value)
	{

	}
}