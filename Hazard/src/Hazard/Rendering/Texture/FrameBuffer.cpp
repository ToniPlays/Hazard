#pragma once
#include <hzrpch.h>
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "FrameBuffer.h"
#include "Hazard/Rendering/RenderCommand.h"

namespace Hazard::Rendering {
	FrameBuffer* FrameBuffer::Create(FrameBufferCreateInfo* info)
	{
		switch (RenderCommand::GetAPI())
		{
			//case RenderAPI::OpenGL: return new OpenGL::OpenGLFrameBuffer(info);
		//	case RenderAPI::Vulkan: return new Vulkan::VulkanFrameBuffer(info);
		}
		return nullptr;
	}
}
