#pragma once

#include <hzrpch.h>
#include "Image2D.h"
#include "../RenderCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering 
{
    Ref<Image2D> Image2D::Create(Image2DCreateInfo* info)
    {
		switch (RenderCommand::GetAPI())
		{
		//case RenderAPI::OpenGL: return new OpenGL::OpenGLImage2D(info);
		case RenderAPI::Vulkan: return new Vulkan::VulkanImage2D(info);
		}
    }
}
