#pragma once

#include <hzrpch.h>
#include "Pipeline.h"
#include "../RenderCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering 
{
	Ref<Pipeline> Pipeline::Create(const PipelineSpecification& specs)
	{
		switch (RenderCommand::GetAPI()) {
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLPipeline>::Create(specs);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanPipeline>::Create(specs);
		}
	}
}
