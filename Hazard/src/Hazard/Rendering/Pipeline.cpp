#pragma once

#include <hzrpch.h>
#include "Pipeline.h"
#include "RenderCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering 
{
	Pipeline* Pipeline::Create(const PipelineSpecification& specs)
	{
		switch (RenderCommand::GetAPI()) {
		case RenderAPI::OpenGL: return nullptr;
		case RenderAPI::Vulkan: return new Vulkan::VulkanPipeline(specs);
		}
	}
}
