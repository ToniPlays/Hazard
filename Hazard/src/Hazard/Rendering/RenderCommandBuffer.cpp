#pragma once

#include <hzrpch.h>
#include "RenderCommandBuffer.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

#include "RenderCommand.h"

namespace Hazard::Rendering {

	Ref<RenderCommandBuffer> RenderCommandBuffer::Create(const std::string& debugName, uint32_t count)
	{
		switch (RenderCommand::GetAPI()) {
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLRenderCommandBuffer(count, debugName);
		case RenderAPI::Vulkan:		return new Vulkan::VulkanRenderCommandBuffer(count, debugName);
		}
		return nullptr;
	}

	Ref<RenderCommandBuffer> RenderCommandBuffer::CreateFromSwapchain(const std::string& debugName)
	{
		switch (RenderCommand::GetAPI()) {
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLRenderCommandBuffer(debugName, true);
		case RenderAPI::Vulkan:		return new Vulkan::VulkanRenderCommandBuffer(debugName, true);
		}
		return nullptr;
	}
}