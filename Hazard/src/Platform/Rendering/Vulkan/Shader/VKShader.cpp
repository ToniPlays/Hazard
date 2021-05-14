#pragma once

#include <hzrpch.h>
#include "VKShader.h"

namespace Hazard::Rendering::Vulkan {

	VKShader::VKShader(std::string path)
	{
		m_File = path;
	}
	VKShader::~VKShader()
	{

	}
	void VKShader::Bind() const
	{

	}
	void VKShader::Unbind() const
	{

	}
	void VKShader::SetUniformInt(const std::string& name, int value)
	{

	}
	void VKShader::SetUniformIntArray(const std::string& name, int* value, uint32_t size)
	{

	}
	void VKShader::SetUniformFloat(const std::string& name, float value)
	{

	}
	void VKShader::SetUniformVec2(const std::string& name, glm::vec2 value)
	{

	}
	void VKShader::SetUniformVec3(const std::string& name, glm::vec3 value)
	{

	}
	void VKShader::SetUniformMat4(const std::string& name, glm::mat4 value)
	{

	}
	void VKShader::SetUniformColor(const std::string& name, Color color)
	{

	}
	void VKShader::SetUniformBool(const std::string& name, bool value)
	{

	}
}