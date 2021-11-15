#pragma once

#include <hzrpch.h>
#include "RenderPass.h"
#include "../RenderCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering {

    Ref<RenderPass> RenderPass::Create(RenderPassCreateInfo* info)
    {
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLRenderPass(info);
		case RenderAPI::Vulkan: return new Vulkan::VulkanRenderPass(info);
		}
    }
}
