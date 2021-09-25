#pragma once

#include <hzrpch.h>
#include "Texture2D.h"
#include "../RenderCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"

namespace Hazard::Rendering 
{
    Texture2D* Texture2D::Create(Texture2DCreateInfo* info)
    {
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLTexture2D(info);
		//case RenderAPI::Vulkan: return new Vulkan::VulkanTexture2D(info);
		}
    }
}
