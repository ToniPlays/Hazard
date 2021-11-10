#pragma once

#include <hzrpch.h>
#include "Shader.h"
#include "../RenderCommand.h"
#include "Hazard/Utils/StringUtil.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering 
{

    Ref<Shader> Shader::Create(const std::string& path)
    {
		HZR_PROFILE_FUNCTION();
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLShader>::Create(path);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanShader>::Create(path);
		}
    }
}
