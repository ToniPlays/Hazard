#pragma once

#include <hzrpch.h>
#include "Shader.h"
#include "RenderCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering 
{
    Shader* Shader::Create(const std::string& path)
    {
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLShader(path);
		case RenderAPI::Vulkan: return new Vulkan::VulkanShader(path);
		}
    }
	std::unordered_map<ShaderType, std::string> Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<ShaderType, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZR_CORE_ASSERT(eol != std::string::npos, "Syntax Error, check Shader");
			size_t begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);

			HZR_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type {0}", type);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			HZR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax Error, check Shader");

			pos = source.find(typeToken, nextLinePos);

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;

	}
}
