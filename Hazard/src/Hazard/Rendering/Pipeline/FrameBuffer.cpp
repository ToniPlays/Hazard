#pragma once
#include <hzrpch.h>
#include "FrameBuffer.h"
#include "../RenderCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"


namespace Hazard::Rendering 
{
	Ref<FrameBuffer> FrameBuffer::Create(FrameBufferCreateInfo* info)
	{
		switch (RenderCommand::GetAPI())
		{
		//case RenderAPI::OpenGL: return new OpenGL::OpenGLFrameBuffer(info);
		case RenderAPI::Vulkan: return new Vulkan::VulkanFrameBuffer(info);
		}
		return nullptr;
	}
}