#pragma once

#include <hzrpch.h>
#include "VertexArray.h"
#include "../RenderCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"

namespace Hazard::Rendering
 {
    VertexArray* VertexArray::Create(const VertexArrayCreateInfo& info)
    {
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLVertexArray(info);
		//case RenderAPI::Vulkan: return new Vulkan::VulkanVertexArray(info);
		}
    }
}